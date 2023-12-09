#ifndef __UIENGINE_UI_VIEW_H__
#define __UIENGINE_UI_VIEW_H__

#include <stdint.h>
#include "../dlist.h"
#include "type.h"
#include "renderer.h"
#include "../web/html_document.h"

typedef void (*RenderNodeRender)(struct RenderNode *, struct Renderer *);

typedef void (*RenderNodeOnMouseDown)(struct RenderNode *, Event e);
typedef void (*RenderNodeOnCustomMouseDown)(struct RenderNode *, struct HtmlElement *elem);
typedef void (*RenderNodeOnMouseUp)(struct RenderNode *, Event e);
typedef void (*RenderNodeOnCustomMouseUp)(struct RenderNode *, struct HtmlElement *elem);
typedef void (*RenderNodeOnClick)(struct RenderNode *, Event e);
typedef void (*RenderNodeOnCustomClick)(struct HtmlElement *elem);
typedef void (*RenderNodeOnMouseEnter)(struct RenderNode *, Event e);
typedef void (*RenderNodeOnCustomMouseEnter)(struct RenderNode *, struct HtmlElement *elem);
typedef void (*RenderNodeOnMouseLeave)(struct RenderNode *, Event e);
typedef void (*RenderNodeOnCustomMouseLeave)(struct RenderNode *, struct HtmlElement *elem);
typedef void (*RenderNodeOnMouseHover)(struct RenderNode *, Event e);
typedef void (*RenderNodeOnCustomMouseHover)(struct RenderNode *, struct HtmlElement *elem);

typedef struct RenderNode {
    Position pos;
    Size size;
    Padding padding;
    Margin margin;
    Color foregroundColor;
    Color backgroundColor;
    Color borderColor;

    struct RenderNode *children;
    DListNode node;

    RenderNodeOnMouseDown onMouseDown;
    RenderNodeOnCustomMouseDown onCustomMouseDown;
    RenderNodeOnMouseUp onMouseUp;
    RenderNodeOnCustomMouseUp onCustomMouseUp;
    RenderNodeOnClick onClick;
    RenderNodeOnCustomClick onCustomClick;

    RenderNodeOnMouseEnter onMouseEnter;
    RenderNodeOnCustomMouseEnter onCustomMouseEnter;
    RenderNodeOnMouseHover onMouseHover;
    RenderNodeOnCustomMouseHover onCustomMouseHover;
    RenderNodeOnMouseLeave onMouseLeave;
    RenderNodeOnCustomMouseLeave onCustomMouseLeave;

    RenderNodeRender render;

    struct HtmlElement *dom;

    bool mouseIn;
} RenderNode;

void render_child(RenderNode *renderNode, struct Renderer *renderer);

#endif /* __UIENGINE_UI_VIEW_H__ */