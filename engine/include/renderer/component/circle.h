#ifndef __UIENGINE_UI_CIRCLE_H__
#define __UIENGINE_UI_CIRCLE_H__

#include "../render_node.h"

typedef struct Circle {
    RenderNode renderNode;
} Circle;

Circle *circle_create();

#endif /* __UIENGINE_UI_CIRCLE_H__ */