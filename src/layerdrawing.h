#pragma once
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

void layer_draw_border(GContext *ctx, Layer *layer, uint8_t style, uint8_t color);
void layer_solid_fill (GContext *ctx, Layer *layer, uint8_t color);
void layer_draw_shadow(GContext *ctx, Layer *layer, uint8_t style, uint8_t distance, uint8_t color);
void build_shadow_table();