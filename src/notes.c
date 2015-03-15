/* ------------------------------------------------------------------------ //
//  Notes
// ------------------------------------------------------------------------ //

#define FONT_KEY_FONT_FALLBACK "RESOURCE_ID_FONT_FALLBACK"
#define FONT_KEY_GOTHIC_14 "RESOURCE_ID_GOTHIC_14"
#define FONT_KEY_GOTHIC_14_BOLD "RESOURCE_ID_GOTHIC_14_BOLD"
#define FONT_KEY_GOTHIC_18 "RESOURCE_ID_GOTHIC_18"
#define FONT_KEY_GOTHIC_18_BOLD "RESOURCE_ID_GOTHIC_18_BOLD"
#define FONT_KEY_GOTHIC_24 "RESOURCE_ID_GOTHIC_24"
#define FONT_KEY_GOTHIC_24_BOLD "RESOURCE_ID_GOTHIC_24_BOLD"
#define FONT_KEY_GOTHIC_28 "RESOURCE_ID_GOTHIC_28"
#define FONT_KEY_GOTHIC_28_BOLD "RESOURCE_ID_GOTHIC_28_BOLD"
#define FONT_KEY_BITHAM_30_BLACK "RESOURCE_ID_BITHAM_30_BLACK"
#define FONT_KEY_BITHAM_42_BOLD "RESOURCE_ID_BITHAM_42_BOLD"
#define FONT_KEY_BITHAM_42_LIGHT "RESOURCE_ID_BITHAM_42_LIGHT"
#define FONT_KEY_BITHAM_42_MEDIUM_NUMBERS "RESOURCE_ID_BITHAM_42_MEDIUM_NUMBERS"
#define FONT_KEY_BITHAM_34_MEDIUM_NUMBERS "RESOURCE_ID_BITHAM_34_MEDIUM_NUMBERS"
#define FONT_KEY_BITHAM_34_LIGHT_SUBSET "RESOURCE_ID_BITHAM_34_LIGHT_SUBSET"
#define FONT_KEY_BITHAM_18_LIGHT_SUBSET "RESOURCE_ID_BITHAM_18_LIGHT_SUBSET"
#define FONT_KEY_ROBOTO_CONDENSED_21 "RESOURCE_ID_ROBOTO_CONDENSED_21"
#define FONT_KEY_ROBOTO_BOLD_SUBSET_49 "RESOURCE_ID_ROBOTO_BOLD_SUBSET_49"
#define FONT_KEY_DROID_SERIF_28_BOLD "RESOURCE_ID_DROID_SERIF_28_BOLD"


Different layer animation types:
------------------------------------
  Switch between layers: (no difference between 2 or many)
    (An array of pointers for order of layers)
    [FB]=FrameBuffer: Achieved by drawing 1st layer, reading framebuffer, altering framebuffer, drawing 2nd layer
                  Needs to interject it's drawing function between the 2 layers
    Fill with color:
      Fill with [color] then next layer
      Dissolve
        Size of dots or squares
        Random or gridlike
      Drip current layer down

  2D
      Swipe left/right/up/down
      Shuffle (like a deck of cards)
        Shuffle direction (over top, underneath)
      [FB] Shrink, fling away
      
    3D
      
      Rotate around
      flip vertically
        For(amount=1,amount<10,amount++) Shrink(Layer1,amount)
        
------------------------------------
  Popups
    Pop up from bottom
    Swing in from side/top/bottom
    Swing out from other direction


Notes:
  Switch: 
  Layers need to all be same size and position
  Popup:  layer duration, speed, position, animation method

------------------------------------ */



/*
Old popup program

#include "pebble.h"
#include "layerpopup.h"

//56b37c6f-792a-480f-b962-9a0db8c32aa4

#define UPDATE_MS 30 // Refresh rate in milliseconds
static void sky_layer_update(Layer *me, GContext *ctx);
static void popup_layer_update(Layer *me, GContext *ctx);
//I don't know why I need to specify the Layer structure here; something changed in 2.6!
typedef struct Layer
{
  GRect bounds;
  GRect frame;
  bool clips : 1;
  bool hidden : 1;
  struct Layer *next_sibling;
  struct Layer *parent;
  struct Layer *first_child;
  struct Window *window;
  LayerUpdateProc update_proc;
} Layer;

static Window *main_window;
static Layer *sky_layer, *popup_layer;
typedef struct popupStruct {
  uint8_t pos;
  uint8_t maxpos;
  uint8_t speed;
  uint8_t time;
  uint8_t duration;
  char *msg;
} popupStruct;
popupStruct popup = {0, 30, 1, 0, 0, NULL}; // pos, maxpos, speed, time, duration, msg

typedef struct PointStruct {
  int16_t dec;   // declination
  int16_t ra;    // right ascension
} PointStruct;
PointStruct dots[1000];

typedef struct CamStruct {
  int16_t pitch;   // up/down [-16384, 16384]
  int16_t roll;    // aileron roll [-32768, 32768]
  int16_t yaw;     // direction facing [-32768, 32768]
} CamStruct;
CamStruct cam;

int32_t T0=0, T1=0, T2=0, T3=0, T4=0, T5=0;
GPoint vel = {1,1};

uint16_t maxdots=0;
// ------------------------------------------------------------------------ //
//  Helper Functions
// ------------------------------------------------------------------------ //
#define root_depth 20          // How many iterations square root function performs
int32_t sqrt32(int32_t a) {int32_t b=a; for(uint8_t i=0; i<root_depth; i++) b=(b+(a/b))/2; return b;} // Square Root 32bit
 int8_t   abs8( int8_t x) {return (x ^ (x >>  7)) - (x >>  7);}
int16_t  abs16(int16_t x) {return (x ^ (x >> 15)) - (x >> 15);}
int32_t  abs32(int32_t x) {return (x ^ (x >> 31)) - (x >> 31);}

// ------------------------------------------------------------------------ //
//  Program Functions
// ------------------------------------------------------------------------ //
void update_camera() {
  if(sky_layer->frame.origin.x + sky_layer->frame.size.w + vel.x >143 || sky_layer->frame.origin.x + vel.x < 0) vel.x *= -1;
  if(sky_layer->frame.origin.y + sky_layer->frame.size.h + vel.y >257 || sky_layer->frame.origin.y + vel.y < 0) vel.y *= -1;
  //sky_layer->frame.origin.x += vel.x;  sky_layer->frame.origin.y += vel.y;  // move layer
}

void popout_message() {
  
}

void popup_message(char *msg, uint8_t maxpos, uint8_t duration, uint8_t speed) {
  popup.time     = 0;
  popup.msg      = msg;
  popup.speed    = speed;
  popup.maxpos   = maxpos;
  popup.duration = duration;  // duration=255 for permenant hold
  if(popup.pos == 0) {
    popup.pos    = speed;
    layer_set_frame(popup_layer, GRect(0, 168-popup.pos, 144, popup.maxpos));  // Move to visible position, else drawing callback isn't called.
  }
}

void popup_done() {
  popup.time = popup.duration; // if duration=255, stays up until popup_done is called
}

// ------------------------------------------------------------------------ //
//  Timer Functions
// ------------------------------------------------------------------------ //
static void timer_callback(void *data) {
  update_camera();
  layer_mark_dirty(sky_layer);  // Schedule redraw of screen
  layer_mark_dirty(popup_layer);  // Schedule redraw of screen
  app_timer_register(UPDATE_MS, timer_callback, NULL); // Schedule a callback

}
// ------------------------------------------------------------------------ //
//  Button Functions
// ------------------------------------------------------------------------ //
void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  popup_done();
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  popup_message("hello, this is a very long message to display on such a little lcd screen", 30, 20, 3); // msg, height, duration, speed
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
      
}
  
void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

// ------------------------------------------------------------------------ //
//  Drawing Functions
// ------------------------------------------------------------------------ //
static void sky_layer_update(Layer *me, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorWhite);  graphics_draw_rect(ctx, me->bounds); //White Border Around Frame
  
  static char textbuffer[100];  //Buffer to hold text
  GRect textframe = GRect(0, 0, 144, 30);  // Text Box Position and Size: x, y, w, h
  //GRect textframe = GRect(2, 2, 30, 30);  // Text Box Position and Size: x, y, w, h
  //GRect textframe = me->bounds;
  //snprintf(textbuffer, sizeof(textbuffer), " Hello 0x%x", (unsigned int)ctx);  // What text to draw
  //snprintf(textbuffer, sizeof(textbuffer), "%ld %ld %ld %ld", CAM_PITCH, CAM_ROLL, CAM_YAW);  // What text to draw
  T0=popup.time;
  T1=popup.pos;
  T2=popup.duration;
  T3=popup.speed;
  T4=popup.maxpos;
  T5 = popup.time + popup.pos;
  
  //T0 = popup_layer->bounds.origin.x;
  //T1 = popup_layer->bounds.origin.y;
  //T2 = popup_layer->bounds.size.h;
  //T3 = popup_layer->bounds.size.w;
  //T0=0; T1=0; T2=0; T3=0;  T4=0; T5=0;
  
  //snprintf(textbuffer, sizeof(textbuffer), "%+06ld %+06ld %+06ld %+06ld %+06ld %+06ld", T0, T1, T2, T3, T4, T5);  // What text to draw
    snprintf(textbuffer, sizeof(textbuffer), "%ld %ld %ld %ld %ld %ld", T0, T1, T2, T3, T4, T5);  // What text to draw
  //snprintf(textbuffer, sizeof(textbuffer), " Hello 0x%x", (unsigned int)ctx);  // What text to draw
  graphics_context_set_fill_color(ctx, GColorBlack); graphics_fill_rect(ctx, textframe, 0, GCornerNone);  //Black Filled Rectangle
  graphics_context_set_stroke_color(ctx, GColorWhite); graphics_draw_rect(ctx, textframe);                //White Rectangle Border
  graphics_context_set_text_color(ctx, GColorWhite);  // Text Color
  graphics_draw_text(ctx, textbuffer, fonts_get_system_font(FONT_KEY_GOTHIC_14), textframe, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
  //}
}

#define popup_bg_color 0
static void popup_layer_update(Layer *me, GContext *ctx) {
  if(popup.time + popup.pos > 0) {
    graphics_context_set_fill_color(ctx, GColorBlack); graphics_fill_rect(ctx, GRect(1, 1, 142, popup.maxpos-2), 0, GCornerNone);  //filled rectangle background
    
    // white rounded-top borders:
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_draw_line(ctx, GPoint(2, 0), GPoint(141, 0)); // top edge
    graphics_draw_pixel(ctx, GPoint(1, 1)); // upper-left corner
    graphics_draw_pixel(ctx, GPoint(142, 1)); // upper-right corner
    graphics_draw_line(ctx, GPoint(0, 2), GPoint(0, popup.maxpos)); // left edge
    graphics_draw_line(ctx, GPoint(143, 2), GPoint(143, popup.maxpos)); // right edge
    //graphics_draw_line(ctx, GPoint(0, popup.maxpos-1), GPoint(143, popup.maxpos-1)); // bottom edge
    graphics_draw_line(ctx, GPoint(0, popup.pos-1), GPoint(143, popup.pos-1)); // bottom edge
    
    //graphics_context_set_text_color(ctx, 1 - popup_bg_color);  // Text Color
    graphics_context_set_text_color(ctx, GColorWhite);  // Text Color    
    graphics_draw_text(ctx, popup.msg, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(1, -2, 142, popup.maxpos), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);  //Write Text
    
    // Update positioning
    if(popup.time == 0) {
      if(popup.pos < popup.maxpos)
        popup.pos += popup.speed;    // move up phase
      else 
        popup.time++;                // begin hold phase
    } else if(popup.time == popup.duration) {
      if(popup.pos > 0)
        popup.pos -= popup.speed;    // move down phase. stops when out of view.
    } else {
      popup.time=(popup.time+1)%255; // continue hold phase (255 = permenant hold)
      //popup.time++;                // continue hold phase
    }
    layer_set_frame(popup_layer, GRect(0, 168-popup.pos, 144, popup.maxpos)); // update position
  }
}
  
// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  //sky_layer = layer_create(layer_get_frame(window_layer));
  //sky_layer = layer_create(GRect(0, 0, 144, 168));
  sky_layer = layer_create(GRect(0, 0, 80, 60));
  layer_set_update_proc(sky_layer, sky_layer_update);
  layer_add_child(window_layer, sky_layer);

  popup_layer = layer_create(GRect(0, 167, 144, 30));
  layer_set_update_proc(popup_layer, popup_layer_update);
  layer_add_child(window_layer, popup_layer);
  
  app_timer_register(UPDATE_MS, timer_callback, NULL); // Schedule a callback

}

static void main_window_unload(Window *window) {
  layer_destroy(sky_layer);
  layer_destroy(popup_layer);
  // Destroy all layers of the window here
}

static void init(void) {
  // Set up and push main window
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {.load = main_window_load, .unload = main_window_unload});
  window_set_click_config_provider(main_window, click_config_provider);
  window_set_background_color(main_window, GColorBlack);
  window_set_fullscreen(main_window, true);

  //Set up other stuff
  srand(time(NULL));  // Seed randomizer
  accel_data_service_subscribe(0, NULL);  // We will be using the accelerometer
  
  //Begin
  window_stack_push(main_window, true); // Display window.  Callback will be called once layer is dirtied then written
}


*/