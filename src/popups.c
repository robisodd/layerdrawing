#include <pebble.h>
#include "popups.h"
static uint8_t popupLayerCount = 0;

void layer_popup_auto_update() {
  // sets up timer until no more layers are left to update
}

void layer_popup_update(uint8_t layerID) {
  if(layerID<popupLayerCount) {
    
  }
}
  
uint8_t layer_popup(GContext *ctx, Layer *popuplayer, uint16_t popup_type) {
  uint8_t layerID;
  layerID = popupLayerCount; popupLayerCount++;

  
  return layerID;
}


uint8_t layer_popout(GContext *ctx, Layer *popuplayer, uint16_t popout_type) {
  uint8_t layerID;
  layerID = popupLayerCount; popupLayerCount++;
  
  
  return layerID;
}

