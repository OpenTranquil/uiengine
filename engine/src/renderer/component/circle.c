#include "renderer/component/circle.h"
#include "mem/mem.h"
#include <stdio.h>
#include <string.h>

void circle_default_render(RenderNode *renderNode, Renderer *renderer) {
    Circle *circle = ContainerOf(renderNode, Circle, renderNode);
    renderer->fillCircle(renderer, circle->renderNode.pos, circle->renderNode.radius, circle->renderNode.backgroundColor);
    render_child(&circle->renderNode, renderer);
}

void circle_default_onclick(RenderNode *renderNode, Event e) {
    if (renderNode->onCustomClick != NULL) {
        renderNode->onCustomClick(renderNode->dom);
    }
}

void circle_default_onmouse_down(RenderNode *renderNode, Event e) {}

void circle_default_onmouse_up(RenderNode *renderNode, Event e) {}

void circle_default_onmouse_enter(RenderNode *renderNode, Event e) {}

void circle_default_onmouse_leave(RenderNode *renderNode, Event e) {}

void circle_default_onmouse_hover(RenderNode *renderNode, Event e) {
    if (renderNode->onCustomMouseHover != NULL) {
        renderNode->onCustomMouseHover(renderNode, renderNode->dom);
    }
}

Circle *circle_create() {
    Circle *circle = (Circle *)mem_alloc(sizeof(Circle));
    circle->renderNode.render = circle_default_render;
    circle->renderNode.onMouseDown = circle_default_onmouse_down;
    circle->renderNode.onCustomMouseDown = NULL;
    circle->renderNode.onMouseUp = circle_default_onmouse_up;
    circle->renderNode.onCustomMouseUp = NULL;
    circle->renderNode.onClick = circle_default_onclick;
    circle->renderNode.onCustomClick = NULL;
    circle->renderNode.onMouseEnter = circle_default_onmouse_enter;
    circle->renderNode.onCustomMouseEnter = NULL;
    circle->renderNode.onMouseLeave = circle_default_onmouse_leave;
    circle->renderNode.onCustomMouseLeave = NULL;
    circle->renderNode.onMouseHover = circle_default_onmouse_hover;
    circle->renderNode.onCustomMouseHover = NULL;
    circle->renderNode.children = NULL;
    dlist_init(&circle->renderNode.node);
    return circle;
}