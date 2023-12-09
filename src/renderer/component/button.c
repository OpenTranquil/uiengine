#include "../../../include/renderer/component/button.h"
#include "../../../include/mem/mem.h"
#include <stdio.h>
#include <string.h>

void button_default_render(RenderNode *renderNode, Renderer *renderer) {
    Button *button = ContainerOf(renderNode, Button, renderNode);
    renderer->fillRect(renderer, button->renderNode.pos, button->renderNode.size, button->renderNode.backgroundColor);
    renderer->drawRect(renderer, button->renderNode.pos, button->renderNode.size, button->renderNode.borderColor);
    if ((renderNode->dom->type == HTML_ELEMENT_TYPE_DOM) && 
        (strcmp(renderNode->dom->dom.tag, "button") == 0)){
        HtmlElement *e = renderNode->dom->dom.childrens;
        if ((e != NULL) && (e->type == HTML_ELEMENT_TYPE_CONTENT)) {
            char *text = e->content.content;
            uint32_t fontSize = 50;
            uint32_t fontLen = strlen(text) * (fontSize / 2.5);

            uint32_t posX = renderNode->pos.x + (renderNode->size.width - fontLen) / 2;
            uint32_t posY = renderNode->pos.y + (renderNode->size.height - fontSize) / 2;

            Position pos = { posX, posY };
            Size size = { fontLen, fontSize};
            Color color = {255,255,255,0};
            renderer->drawText(renderer, pos, size, color, text);
        }
    }
    render_child(&button->renderNode, renderer);
}

void button_default_onclick(RenderNode *renderNode, Event e) {
    if (renderNode->onCustomClick != NULL) {
        renderNode->onCustomClick(renderNode->dom);
    }
}

void button_default_onmouse_down(RenderNode *renderNode, Event e) {
    renderNode->backgroundColor.r -= 20;
    renderNode->backgroundColor.g -= 20;
    renderNode->backgroundColor.b -= 20;
    if (renderNode->onCustomMouseDown != NULL) {
        renderNode->onCustomMouseDown(renderNode, renderNode->dom);
    }
}

void button_default_onmouse_up(RenderNode *renderNode, Event e) {
    renderNode->backgroundColor.r += 20;
    renderNode->backgroundColor.g += 20;
    renderNode->backgroundColor.b += 20;
    if (renderNode->onCustomMouseUp != NULL) {
        renderNode->onCustomMouseUp(renderNode, renderNode->dom);
    }
}

void button_default_onmouse_enter(RenderNode *renderNode, Event e) {
    if (renderNode->onCustomMouseEnter != NULL) {
        renderNode->onCustomMouseEnter(renderNode, renderNode->dom);
    }
    if (renderNode->onCustomMouseHover != NULL) {
        renderNode->onCustomMouseHover(renderNode, renderNode->dom);
    }
}

void button_default_onmouse_leave(RenderNode *renderNode, Event e) {
    if (renderNode->onCustomMouseLeave != NULL) {
        renderNode->onCustomMouseLeave(renderNode, renderNode->dom);
    }
}

void button_default_onmouse_hover(RenderNode *renderNode, Event e) {
    if (renderNode->onCustomMouseHover != NULL) {
        renderNode->onCustomMouseHover(renderNode, renderNode->dom);
    }
}

Button *button_create() {
    Button *button = (Button *)mem_alloc(sizeof(Button));
    button->renderNode.render = button_default_render;
    button->renderNode.onMouseDown = button_default_onmouse_down;
    button->renderNode.onCustomMouseDown = NULL;
    button->renderNode.onMouseUp = button_default_onmouse_up;
    button->renderNode.onCustomMouseUp = NULL;
    button->renderNode.onClick = button_default_onclick;
    button->renderNode.onCustomClick = NULL;
    button->renderNode.onMouseEnter = button_default_onmouse_enter;
    button->renderNode.onCustomMouseEnter = NULL;
    button->renderNode.onMouseLeave = button_default_onmouse_leave;
    button->renderNode.onCustomMouseLeave = NULL;
    button->renderNode.onMouseHover = button_default_onmouse_hover;
    button->renderNode.onCustomMouseHover = NULL;
    return button;
}