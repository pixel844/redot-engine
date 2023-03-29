#include "CitroRenderer.h"
#include "servers/rendering/dummy/rasterizer_dummy.h"

CitroRenderer::CitroRenderer() {
    printf("Starting Citro3D Renderer!\n");
}

CitroRenderer::~CitroRenderer() {
    
}

void CitroRenderer::make_current() {
    ::RasterizerDummy::make_current();
}

CitroRenderer *new_CitroRenderer() {
    return new CitroRenderer();
}