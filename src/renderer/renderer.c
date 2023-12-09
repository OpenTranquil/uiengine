#include "../../include/renderer/renderer.h"
#include "../../include/renderer/render_node.h"
#include "../../include/mem/mem.h"
#include <stdio.h>

void renderer_default_draw_rect(Renderer *renderer, Position pos, Size size, Color backgroundColor) {
    renderer->renderBackend->drawRect(renderer->renderBackend, pos, size, backgroundColor);
}

void renderer_default_draw_text(Renderer *renderer, Position pos, Size size, Color fontColor, char *text) {
    renderer->renderBackend->drawText(renderer->renderBackend, pos, size, fontColor, text);
}

void renderer_default_fill_rect(Renderer *renderer, Position pos, Size size, Color backgroundColor) {
    renderer->renderBackend->fillRect(renderer->renderBackend, pos, size, backgroundColor);
}

void renderer_default_register_backend(Renderer *renderer, RenderBackend *renderBackend) {
    renderer->renderBackend = renderBackend;
}

void renderer_default_set_root_render_node(Renderer *renderer, RenderNode *rootNode) {
    renderer->rootNode = rootNode;
}

void renderer_default_render(struct Renderer *renderer) {
    if (renderer->renderBackend == NULL) {
        printf("render should register a backend first!\n");
        return;
    }
    renderer->renderBackend->clear(renderer->renderBackend);
    render_child(renderer->rootNode, renderer);
    renderer->renderBackend->submit(renderer->renderBackend);
}

void renderer_default_init(struct Renderer *renderer) {
    if (renderer->renderBackend == NULL) {
        printf("render should register a backend first!\n");
        return;
    }
    renderer->renderBackend->init(renderer->renderBackend);
    renderer->runningState = RENDERER_STATE_RUNNING;
}

void renderer_teigger_mouse_event(RenderNode *renderNode, Event e) {
    if (renderNode == NULL) {
        return;
    }

    RenderNode *children = renderNode->children;
    if (children == NULL) {
        return;
    }
    DListNode *node = &children->node;
    while (node != NULL) {
        RenderNode *elem = ContainerOf(node, RenderNode, node);
        renderer_teigger_mouse_event(elem, e);
        node = node->right;
    }
    if (renderNode->dom->type != HTML_ELEMENT_TYPE_DOM) {
        return;
    }
    if (renderNode->onMouseDown != NULL && e.type == EVENT_MOUSE_LEFT_DOWN) {
        if ((e.leftMouseUp.x >= renderNode->pos.x && e.leftMouseUp.x <= (renderNode->pos.x + renderNode->size.width)) &&
            (e.leftMouseUp.y >= renderNode->pos.y && e.leftMouseUp.y <= (renderNode->pos.y + renderNode->size.height))) {
            renderNode->onMouseDown(renderNode, e);
        }
    }
    if (renderNode->onMouseUp != NULL && e.type == EVENT_MOUSE_LEFT_UP) {
        if ((e.leftMouseUp.x >= renderNode->pos.x && e.leftMouseUp.x <= (renderNode->pos.x + renderNode->size.width)) &&
            (e.leftMouseUp.y >= renderNode->pos.y && e.leftMouseUp.y <= (renderNode->pos.y + renderNode->size.height))) {
            renderNode->onMouseUp(renderNode, e);
        }
    }
    if (renderNode->onClick != NULL && e.type == EVENT_MOUSE_LEFT_UP) {
        if ((e.leftMouseUp.x >= renderNode->pos.x && e.leftMouseUp.x <= (renderNode->pos.x + renderNode->size.width)) &&
            (e.leftMouseUp.y >= renderNode->pos.y && e.leftMouseUp.y <= (renderNode->pos.y + renderNode->size.height))) {
            renderNode->onClick(renderNode, e);
        }
    }

    if ((renderNode->onMouseEnter != NULL || renderNode->onMouseLeave != NULL) && e.type == EVENT_MOUSE_MOTION) {
        if ((e.mouseMove.x >= renderNode->pos.x && e.mouseMove.x <= (renderNode->pos.x + renderNode->size.width)) &&
            (e.mouseMove.y >= renderNode->pos.y && e.mouseMove.y <= (renderNode->pos.y + renderNode->size.height))) {
            if (!renderNode->mouseIn) {
                if(renderNode->onMouseEnter) {
                    renderNode->onMouseEnter(renderNode, e);
                }
                renderNode->mouseIn = true;
            }
        } else {
            if (renderNode->mouseIn) {
                if(renderNode->onMouseLeave) {
                    renderNode->onMouseLeave(renderNode, e);
                }
                renderNode->mouseIn = false;
            }
        }
    }
}

void renderer_default_process_event(Renderer *renderer) {
    if (renderer->renderBackend == NULL) {
        printf("render should register a backend first!\n");
        return;
    }
    Event e = renderer->renderBackend->polling(renderer->renderBackend);
    if (e.type == EVENT_EXIT) {
        renderer->runningState = RENDERER_STATE_STOP;
    } else if (e.type == EVENT_MOUSE_LEFT_UP ||
               e.type == EVENT_MOUSE_LEFT_DOWN ||
               e.type == EVENT_MOUSE_MOTION) {
        // Find Trigger RenderNode by ZIndex
        // FIXME: ZIndex is better
        renderer_teigger_mouse_event(renderer->rootNode, e);
    }
}

void renderer_default_destroy(Renderer *renderer) {
    if (renderer->renderBackend == NULL) {
        printf("render should register a backend first!\n");
        return;
    }
}

static void renderer_dump_node(int depth, char *prefix, RenderNode *renderNode) {
    if (renderNode == NULL){
        return;
    }

    for (int i = 0; i < depth; i++){ 
        printf("│   ");
    }

    printf("<Node>(x:%d,y:%d)(w:%d, h:%d)(r:%d,g:%d,b:%d)\n",
            renderNode->pos.x, renderNode->pos.y,
            renderNode->size.width, renderNode->size.height,
            renderNode->backgroundColor.r, renderNode->backgroundColor.g, renderNode->backgroundColor.b);

    RenderNode *children = renderNode->children;
    if (children == NULL) {
        return;
    }
    DListNode *node = &children->node;
    while (node != NULL) {
        RenderNode *elem = ContainerOf(node, RenderNode, node);
        depth++;
        if (node->right != NULL) {
            renderer_dump_node(depth, "├──", elem);
        } else {
            renderer_dump_node(depth, "└──", elem);
        }
        depth--;
        node = node->right;
    }
}

void renderer_default_dump(Renderer *renderer) {
    renderer_dump_node(-1, "", renderer->rootNode);
}

Renderer *render_create() {
    Renderer *renderer = (Renderer *)mem_alloc(sizeof(Renderer));
    renderer->registerBackend = renderer_default_register_backend;
    renderer->render = renderer_default_render;
    renderer->init = renderer_default_init;
    renderer->processEvent = renderer_default_process_event;
    renderer->setRootRenderNode = renderer_default_set_root_render_node;
    renderer->destroy = renderer_default_destroy;

    renderer->dump = renderer_default_dump;

    renderer->drawRect = renderer_default_draw_rect;
    renderer->fillRect = renderer_default_fill_rect;
    renderer->drawText = renderer_default_draw_text;
    return renderer;
}