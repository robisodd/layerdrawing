#include "pebble.h"
#include "layerdrawing.h"

uint8_t shadowtable[4][64];
void build_shadow_table() {
  uint8_t r, g, b;
  for(uint8_t i=0; i<64; i++) {
    shadowtable[0][i]=0;
    shadowtable[3][i]=i;
    r=(i>>4)&3;
    g=(i>>2)&3;
    b=i&3;
    if(r>0) r--;
    if(g>0) g--;
    if(b>0) b--;
    shadowtable[2][i]=0b11000000 + (r<<4) + (g<<2) + b;
    if(r>0) r--;
    if(g>0) g--;
    if(b>0) b--;
    shadowtable[1][i]=0b11000000 + (r<<4) + (g<<2) + b;
  }
}

void layer_solid_fill(GContext *ctx, Layer *layer, uint8_t color) {
//   graphics_context_set_fill_color(ctx, GColorBlack);
  #ifdef PBL_COLOR
    uint8_t fg_opacity = (color>>6)&3;
    uint8_t bg_opacity = (~fg_opacity)&3;
    color = color & 63;
    uint8_t* screen = ((uint8_t*)*(uint32_t*)ctx);
    for(uint16_t y_addr=layer->frame.origin.y*144, row=0; row<layer->frame.size.h; y_addr+=144, row++)
      for(uint16_t x_addr=layer->frame.origin.x, x=0; x<layer->frame.size.w; x_addr++, x++)
        screen[y_addr+x_addr] = shadowtable[bg_opacity][screen[y_addr+x_addr]&63] + shadowtable[fg_opacity][color];
  #else
    GRect rect;  rect=layer->frame;  rect.origin.x = 0; rect.origin.y = 0;
    graphics_context_set_fill_color(ctx, color);
    graphics_fill_rect(ctx, rect, 0, GCornerNone);
  #endif
}
#define SOLIDBORDER 0
#define DASHBORDER 1
void layer_draw_border(GContext *ctx, Layer *layer, uint8_t style, uint8_t color) {
//   graphics_context_set_stroke_color(ctx, GColorWhite);
  #ifdef PBL_COLOR
    graphics_context_set_stroke_color(ctx, (GColor){.argb=color});
  #else
    graphics_context_set_stroke_color(ctx, color);
  #endif
  GRect rect;  rect=layer->frame;  rect.origin.x = 0; rect.origin.y = 0;
  //graphics_draw_rect(ctx, layer->frame);
//   graphics_draw_rect(ctx, layer->bounds);
  graphics_draw_rect(ctx, rect);
}

#define SOLIDSHADOW 0
#define CHECKERBOARDSHADOW 1
void layer_draw_shadow(GContext *ctx, Layer *layer, uint8_t style, uint8_t distance, uint8_t color) {

}


//Simple Shadow: Just masks either the highbit or lowbit of each
//     uint8_t* screen = ((uint8_t*)*(uint32_t*)ctx);
//     for(uint16_t y_addr=layer->frame.origin.y*144, row=0; row<layer->frame.size.h; y_addr+=144, row++)
//       for(uint16_t x_addr=layer->frame.origin.x, x=0; x<layer->frame.size.w; x_addr++, x++)
//         screen[y_addr+x_addr] &= 0b11010101;  // dark shadow
//       //screen[y_addr+x_addr] &= 0b11101010;  // light shadow
