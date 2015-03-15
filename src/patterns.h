#pragma once
#include <pebble.h>
#include "patterns.h"
  
#define PATTERN_INVERT 255
#define PATTERN_DONT_INVERT 0
  
//uint8_t* pattern_offset  (uint8_t *data, int8_t x_offset, int8_t y_offset, uint8_t invert);  // returns pointer to new pattern
   void modify_pattern   (uint8_t *data, int8_t x_offset, int8_t y_offset, uint8_t invert);    // updates pattern
   void  fill_window     (GContext *ctx, uint8_t *data);
   void  fill_framebuffer(GBitmap *fb, uint8_t *data);
   void  fill_rect       (GContext *ctx, GRect rect, uint8_t *data, bool invert, uint8_t x_offset, uint8_t y_offset);

/*
  To Call:
           fill_window(ctx, (uint8_t []){170, 85,170, 85,170, 85,170, 85});    // Fill window with 50% Gray
  Or:
           uint8_t pattern[] = {  0,  0, 84,124,124, 56,146,124};  // 50% Gray
           fill_window(ctx, pattern);                              // Fill window with 50% Gray
           
  To use pattern_offset:
           fill_window(ctx, pattern_offset((uint8_t []){187, 95,174, 93,186,117,234,245}, 0, 0, 255));
  Or:
           uint8_t pattern[] = {  0,  0, 84,124,124, 56,146,124};  // 50% Gray
           fill_window(ctx, pattern_offset(pattern[], 3, -5, 255));
           
 Pattern examples:           
 (uint8_t []){  0,  0, 84,124,124, 56,146,124} // "Tulip"
 (uint8_t []){170, 85,170, 85,170, 85,170, 85} // "50% Gray"
 (uint8_t []){127, 65, 65, 65, 65, 65,127,  0} // "Boxes"
 (uint8_t []){  0, 80,114, 32,  0,  5, 39,  2} // "Critters"
 (uint8_t []){ 32, 80,136, 80, 32,  0,  0,  0} // "Diamonds"
 (uint8_t []){  2,  7,  7,  2, 32, 80, 80, 32} // "Paisley"
 (uint8_t []){224,128,142,136,234, 10, 14,  0} // "Pattern"
 (uint8_t []){130, 68, 40, 17, 40, 68,130,  1} // "Quilt"
 (uint8_t []){ 64,192,200,120,120, 72,  0,  0} // "Scottie"
 (uint8_t []){ 20, 12,200,121,158, 19, 48, 40} // "Spinner"
 (uint8_t []){248,116, 34, 71,143, 23, 34,113} // "Thatches"
 (uint8_t []){  0,  0,  0,  0,128,128,128,240} // "Waffle"
 (uint8_t []){136, 84, 34, 69,136, 21, 34, 81} // "Weave"
 (uint8_t []){187, 95,174, 93,186,117,234,245} // "Bricks"
 (uint8_t []){170,125,198, 71,198,127,190, 85} // "Buttons"
 (uint8_t []){120, 49, 19,135,225,200,140, 30} // "Cargo Net"
 (uint8_t []){ 82, 41,132, 66,148, 41, 66,132} // "Circuits"
 (uint8_t []){ 40, 68,146,171,214,108, 56, 16} // "Cobblestones"
 (uint8_t []){130,  1,  1,  1,171, 85,170, 85} // "Colosseum"
 (uint8_t []){ 30,140,216,253,191, 27, 49,120} // "Daisies"
 (uint8_t []){ 62,  7,225,  7, 62,112,195,112} // "Dizzy"
 (uint8_t []){ 86, 89,166,154,101,149,106,169} // "Field Effect"
 (uint8_t []){254,  2,250,138,186,162,190,128} // "Key"
 (uint8_t []){239,239, 14,254,254,254,224,239} // "Live Wire"
 (uint8_t []){240,240,240,240,170, 85,170, 85} // "Plaid"
 (uint8_t []){215,147, 40,215, 40,147,213,215} // "Rounder"
 (uint8_t []){225, 42, 37,146, 85,152, 62,247} // "Scales"
 (uint8_t []){174, 77,239,255,  8, 77,174, 77} // "Stone"
 (uint8_t []){248,116, 34, 71,143, 23, 34,113} // "Thatches"
 (uint8_t []){ 69,130,  1,  0,  1,130, 69,170} // "Tile"
 (uint8_t []){135,  7,  6,  4,  0,247,231,199} // "Triangles"
 (uint8_t []){ 77,154,  8, 85,239,154, 77,154} // "Waffle's Revenge"
 (uint8_t []){  0, 16, 68, 40,130, 40, 68, 16} // "Pattern 1"
 (uint8_t []){  4, 28,228, 66, 66, 39, 56, 32} // "Pattern 2"
*/