#include "../../include/renderer/render_node.h"
#include <stdio.h>

void render_child(RenderNode *renderNode, Renderer *renderer) {
    if (renderNode == NULL) {
        return;
    }
    if (renderNode->children != NULL) {
        DListNode *node = &renderNode->children->node;
        while (node != NULL) {
            RenderNode *child = ContainerOf(node, RenderNode, node);
            if (child->render) {
                child->render(child, renderer);
            }
            node = node->right;
        }
    }
}