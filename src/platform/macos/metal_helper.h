#pragma once

#include <lib/Metal.hpp>

CA::MetalDrawable* next_drawable(void* layer);
void assign_device(void* layer, MTL::Device* device);