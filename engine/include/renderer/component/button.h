#ifndef __UIENGINE_UI_BUTTON_H__
#define __UIENGINE_UI_BUTTON_H__

#include "../render_node.h"

typedef struct Button {
    RenderNode renderNode;
} Button;

Button *button_create();

#endif /* __UIENGINE_UI_BUTTON_H__ */