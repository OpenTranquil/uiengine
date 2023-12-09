#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/mem/mem.h"
#include "include/web/html_document.h"
#include "include/renderer/renderer.h"
#include "include/renderer/component/button.h"
#include "src/renderer/backend/sdl2/backend_sdl2.h"

typedef struct Offset {
    int x_off;
    int y_off;
} Offset;

Offset offsets[51];

static void test_node(Button *bro) {
    size_t i = 0;
    for (i = 0; i < 50; i++) {
        Button *button = button_create();
        button->renderNode.pos.x = (rand() % 500) + 5;
        button->renderNode.pos.y = (rand() % 400) + 5;
        button->renderNode.size.width = (rand() % 60) + 5;
        button->renderNode.size.height = (rand() % 60) + 5;
        button->renderNode.backgroundColor.r = (rand() % 200) + 20;
        button->renderNode.backgroundColor.g = (rand() % 200) + 20;
        button->renderNode.backgroundColor.b = (rand() % 200) + 20;
        button->renderNode.backgroundColor.a = 0;
        dlist_insert(&bro->renderNode.node, &button->renderNode.node);

        offsets[i].x_off = 1;
        offsets[i].y_off = 1;
    }
    offsets[i].x_off = 1;
    offsets[i].y_off = 1;
}

static int collect(DListNode *da, DListNode *db, int index) {
    RenderNode *a = ContainerOf(da, RenderNode, node);
    RenderNode *b = ContainerOf(db, RenderNode, node);
    if ((
            (a->pos.x + a->size.width >= b->pos.x) && 
            (a->pos.x + a->size.width <= b->pos.x + b->size.width)
        ) &&
        (abs(a->pos.y - b->pos.y) <= (a->size.height + b->size.height))) {
        offsets[index].x_off = -offsets[index].x_off;
    }

    if ((
            (a->pos.x >= b->pos.x) && 
            (a->pos.x <= b->pos.x + b->size.width)
        ) && 
        (abs(a->pos.y - b->pos.y) <= (a->size.height + b->size.height))) {
        offsets[index].x_off = -offsets[index].x_off;
    }

    if ((
            (a->pos.y >= b->pos.y) && 
            (a->pos.y <= b->pos.y + b->size.height)
        ) && 
        (abs(a->pos.x - b->pos.x) <= (a->size.width + b->size.width))) {
        offsets[index].y_off = -offsets[index].y_off;
    }

    if ((
            (a->pos.y + a->size.height >= b->pos.y) && 
            (a->pos.y + a->size.height <= b->pos.y + b->size.height)
        ) && 
        (abs(a->pos.x - b->pos.x) <= (a->size.width + b->size.width))) {
        offsets[index].y_off = -offsets[index].y_off;
    }
}

int main(int argc, char* argv[]) {
    RenderNode *rootNode = (RenderNode*)mem_alloc(sizeof(RenderNode));

    Button *button = button_create();
    button->renderNode.pos.x = 10;
    button->renderNode.pos.y = 10;
    button->renderNode.size.width = 20;
    button->renderNode.size.height = 10;
    button->renderNode.backgroundColor.r = 255;
    button->renderNode.backgroundColor.g = 0;
    button->renderNode.backgroundColor.b = 0;
    button->renderNode.backgroundColor.a = 0;

    test_node(button);

    rootNode->children = &button->renderNode;

    Renderer *renderer = render_create();
    RenderBackend* sdl2 = backend_sdl2_create();
    renderer->registerBackend(renderer, sdl2);
    renderer->setRootRenderNode(renderer, rootNode);
    renderer->init(renderer);

    while (renderer->runningState != RENDERER_STATE_STOP) {
        renderer->processEvent(renderer);

        DListNode *node = &button->renderNode.node;
        int index = 0;
        while (node != NULL) {
            RenderNode *rNode = ContainerOf(node, RenderNode, node);

            DListNode *inner = &button->renderNode.node;
            while (inner != NULL) {
                collect(node, inner, index);
                inner = inner->right;
            }

            if ((rNode->pos.x + rNode->size.width >= 640) || (rNode->pos.x <= 0)) {
                offsets[index].x_off = -offsets[index].x_off;
            }
            if ((rNode->pos.y + rNode->size.height >= 480) || (rNode->pos.y <= 0)) {
                offsets[index].y_off = -offsets[index].y_off;
            }
            rNode->pos.x += offsets[index].x_off;
            rNode->pos.y += offsets[index].y_off;
            node = node->right;
            index++;
        }

        renderer->render(renderer);
    }
    renderer->destroy(renderer);

    return 0;
}