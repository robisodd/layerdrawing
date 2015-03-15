#include <pebble.h>
#include "mathfunc.h"
#include "animate.h"
/*
int16_t vel = 16;   // Start negative so it's not moving
int16_t pos = 0;    // Position=0 means at start/stop, so not moving
  
bool order = true;  // Which square is on top

//two ways to timer
//  user calls update callback
//  set timer to call update callback

static void timer_callback(void *data) {
  if(pos != 0){
    vel -= 2 * sign16(pos); // Accelerate/Decelerate
    pos += vel;         // Move it!
    if(vel == 0)
      order = (!order); // Switch who's on top when furthest away
  }
  layer_mark_dirty(root_layer);  // Schedule redraw of screen
}

void swap_layers() {
  if(pos == 0)
    pos += vel;

}

static void draw_view1(GContext *ctx) {
  GRect tempview;
  tempview=view1;
  tempview.origin.x -= pos;
  tempview.origin.y -= pos;
  graphics_context_set_fill_color(ctx, 0); graphics_fill_rect(ctx, tempview, 0, GCornerNone);  //B Filled Rectangle
  graphics_context_set_stroke_color(ctx, 1); graphics_draw_rect(ctx, tempview);                //W Rectangle Border

  snprintf(text, sizeof(text), "\n\nGoodbye!");  // What text to draw
  graphics_context_set_text_color(ctx, 1);  // Text Color
  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14), tempview, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);  //Write Text
}

static void draw_view2(GContext *ctx) {
  GRect tempview;
  tempview=view2;
  tempview.origin.x += pos;
  tempview.origin.y += pos;
  graphics_context_set_fill_color(ctx, 0); graphics_fill_rect(ctx, tempview, 0, GCornerNone);  //B Filled Rectangle
  graphics_context_set_stroke_color(ctx, 1); graphics_draw_rect(ctx, tempview);                //W Rectangle Border

  tempview.origin.x += 10;
  tempview.origin.y += 10;
  tempview.size.w -= 20;
  tempview.size.h -= 20;  
  graphics_context_set_fill_color(ctx, 1); graphics_fill_rect(ctx, tempview, 0, GCornerNone);  //W Filled Rectangle
  
  snprintf(text, sizeof(text), "\n\nHello!");  // What text to draw
  graphics_context_set_text_color(ctx, 0);  // Text Color
  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14), tempview, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);  //Write Text
}

  if(order) {
    draw_view1(ctx);
    draw_view2(ctx);
  } else {
    draw_view2(ctx);
    draw_view1(ctx);
  }
*/