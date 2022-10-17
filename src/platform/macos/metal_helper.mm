#include "metal_helper.h"

CA::MetalDrawable* next_drawable(void* layer)
{
  CAMetalLayer* metalLayer = (CAMetalLayer*) layer;
  id <CAMetalDrawable> metalDrawable = [metalLayer nextDrawable];
  CA::MetalDrawable* pMetalCppDrawable = ( __bridge CA::MetalDrawable*) metalDrawable;
  return pMetalCppDrawable;
}