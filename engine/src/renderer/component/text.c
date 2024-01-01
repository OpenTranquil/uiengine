#include "renderer/component/text.h"
#include "mem/mem.h"
#include <stdio.h>
#include <string.h>

void text_default_render(RenderNode *renderNode, Renderer *renderer) {
    Text *text = ContainerOf(renderNode, Text, renderNode);
    renderer->drawText(renderer, text->renderNode.pos, text->renderNode.size, text->renderNode.backgroundColor, text->str);
    render_child(&text->renderNode, renderer);
}

void text_default_onclick(RenderNode *renderNode, Event e) {
    if (renderNode->onCustomClick != NULL) {
        renderNode->onCustomClick(renderNode->dom);
    }
}

void text_default_onmouse_down(RenderNode *renderNode, Event e) {}

void text_default_onmouse_up(RenderNode *renderNode, Event e) {}

void text_default_onmouse_enter(RenderNode *renderNode, Event e) {}

void text_default_onmouse_leave(RenderNode *renderNode, Event e) {}

void text_default_onmouse_hover(RenderNode *renderNode, Event e) {}

Text *text_create(char *str) {
    Text *text = (Text *)mem_alloc(sizeof(Text));
    text->renderNode.render = text_default_render;
    text->renderNode.onMouseDown = text_default_onmouse_down;
    text->renderNode.onCustomMouseDown = NULL;
    text->renderNode.onMouseUp = text_default_onmouse_up;
    text->renderNode.onCustomMouseUp = NULL;
    text->renderNode.onClick = text_default_onclick;
    text->renderNode.onCustomClick = NULL;
    text->renderNode.onMouseEnter = text_default_onmouse_enter;
    text->renderNode.onCustomMouseEnter = NULL;
    text->renderNode.onMouseLeave = text_default_onmouse_leave;
    text->renderNode.onCustomMouseLeave = NULL;
    text->renderNode.onMouseHover = text_default_onmouse_hover;
    text->renderNode.onCustomMouseHover = NULL;
    text->str = str;
    dlist_init(&text->renderNode.node);
    return text;
}