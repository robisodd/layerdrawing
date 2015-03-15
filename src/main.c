//56b37c6f-792a-480f-b962-9a0db8c32aa4
#include "pebble.h"
#include "mathfunc.h"
#include "animate.h"
#include "patterns.h"
#include "layerdrawing.h"
// #include "layerdrawing.c"
#define UPDATE_MS 50
  
static Window *main_window;
static Layer *root_layer;
static Layer *layer1, *layer2;
uint8_t *pattern;
uint8_t brickpattern[] = {160, 81,162, 69,138, 21, 10, 68, 0b11110000, 0b11000000};  //8 bytes, white, black
//#define LAYER_LIST_LENGTH 10
//static Layer *layerlist[LAYER_LIST_LENGTH];

//static char text[40];  //Buffer to hold text

// ------------------------------------------------------------------------ //
//  Timer Functions
// ------------------------------------------------------------------------ //
static void move_background_timer_callback(void *data) {
  modify_pattern(pattern, 2, 0, 0);
  layer_mark_dirty(root_layer);  // Schedule redraw of screen
  app_timer_register(UPDATE_MS, move_background_timer_callback, NULL); // Schedule a callback
}

// ------------------------------------------------------------------------ //
//  Button Functions
// ------------------------------------------------------------------------ //
void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {}
void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  //swap_layers();
  modify_pattern(pattern, 0, 0, 255);  // invert
  layer_mark_dirty(root_layer);  // Schedule redraw of screen

}
void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {}
  
void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}


// ------------------------------------------------------------------------ //
//  Drawing Functions
// ------------------------------------------------------------------------ //
static void layer1_update(Layer *me, GContext *ctx) {
  layer_solid_fill(ctx, me, 0b01000011);
  layer_draw_border(ctx, me, 1, 0b11001100);
  layer_draw_shadow(ctx, me, 0, 3, 0b11000000);
}

static void root_layer_update(Layer *me, GContext *ctx) {
  //graphics_context_set_fill_color(ctx, 0); graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);  //Clear the screen
   //fill_window(ctx, (uint8_t []){187, 95,174, 93,186,117,234,245});    // Fill window with brick
  
  //uint8_t pattern[] = {187, 95,174, 93,186,117,234,245};  // Brick
  
  //GBitmap *framebuffer = graphics_capture_frame_buffer(ctx);
  //graphics_release_frame_buffer(ctx, framebuffer);
  //if (!framebuffer) return;
  //fill_bitmap(framebuffer, pattern);  // Fill window with Brick, inverted and shifted
  
  //graphics_release_frame_buffer(ctx, graphics_capture_frame_buffer(ctx));
  fill_window(ctx, pattern);  // Fill window with Brick, inverted and shifted
}
  
// ------------------------------------------------------------------------ //
//  Main Functions
// ------------------------------------------------------------------------ //
static void main_window_load(Window *window) {
  root_layer = window_get_root_layer(window);
  layer_set_update_proc(root_layer, root_layer_update);

  layer1 = layer_create(GRect(40, 40, 64, 88));
  //layer2 = layer_create(GRect(40, 40, 64, 88));
  layer_set_update_proc(layer1, layer1_update);
  layer_add_child(root_layer, layer1);
}

static void main_window_unload(Window *window) {
  layer_destroy(layer1);
}

static void init(void) {
  // Set up and push main window
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load   = main_window_load,
    .unload = main_window_unload
  });
  window_set_click_config_provider(main_window, click_config_provider);
  window_set_fullscreen(main_window, true);
  
  build_shadow_table();
  pattern = brickpattern;
  //pattern_offset(pattern, 0, 4, 255); // invert brick pattern

  //Begin
  window_stack_push(main_window, true); // Display window (true = animated)
  app_timer_register(UPDATE_MS, move_background_timer_callback, NULL); // Schedule a callback
}
  
static void deinit(void) {
  window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
