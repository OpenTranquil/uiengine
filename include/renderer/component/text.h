#ifndef __UIENGINE_UI_TEXT_H__
#define __UIENGINE_UI_TEXT_H__

#include "../render_node.h"

typedef struct Text {
    RenderNode renderNode;
    char *str;
} Text;

Text *text_create(char *str);

#endif /* __UIENGINE_UI_TEXT_H__ */