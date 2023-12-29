#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mem/mem.h"
#include "web/html_document.h"
#include "renderer/renderer.h"
#include "dlist.h"
#include "renderer/component/circle.h"
#include "../../../src/engine/renderer/backend/sdl2/backend_sdl2.h"

int main(int argc, char* argv[]) {
    RenderNode *rootNode = (RenderNode*)mem_alloc(sizeof(RenderNode));

    Circle *circle = circle_create();
    circle->renderNode.pos.x = 200;
    circle->renderNode.pos.y = 200;
    circle->renderNode.size.width = 80;
    circle->renderNode.size.height = 80;
    circle->renderNode.radius.topleft = 50;
    circle->renderNode.radius.topright= 50;
    circle->renderNode.radius.bottomleft = 50;
    circle->renderNode.radius.bottomright = 50;
    circle->renderNode.backgroundColor.r = 0;
    circle->renderNode.backgroundColor.g = 255;
    circle->renderNode.backgroundColor.b = 0;
    circle->renderNode.backgroundColor.a = 0;

    rootNode->children = &circle->renderNode;

    Renderer *renderer = render_create();
    RenderBackend* sdl2 = backend_sdl2_create();
    renderer->registerBackend(renderer, sdl2);
    renderer->setRootRenderNode(renderer, rootNode);

    const char *winTitle = "EyeTrainer";
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t width = 1024;
    uint32_t height = 960;
    renderer->init(renderer, winTitle, 1, x, y, width, height);

    while (renderer->runningState != RENDERER_STATE_STOP) {
        renderer->processEvent(renderer);
        renderer->render(renderer);
    }
    renderer->destroy(renderer);

    return 0;
}