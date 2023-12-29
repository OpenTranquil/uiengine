#ifndef __UIENGINE_DLIST_H__
#define __UIENGINE_DLIST_H__

#include <stddef.h>

#define OffsetOf(type, member) (char *) (&(((type *) 0)->member))
#define ContainerOf(ptr, type, member) ((type *) ((char *) (ptr) - (char *) (&(((type *) 0)->member))))

typedef struct DListNode {
    struct DListNode *left;
    struct DListNode *right;
} DListNode;

static void inline dlist_insert(DListNode *head, DListNode *node) {
    if ((head != NULL) && (node != NULL)) {
        if (head->right == NULL) {
            head->right = node;
            node->left = head;
        } else {
            // HEAD <--> NODE <--> TIAL
            head->right->left = node;
            node->right = head->right;
            node->left = head;
            head->right = node;
        }
    } else if ((head == NULL) && (node != NULL)) {
        head = node;
    }
}

static inline void dlist_append_tail(struct DListNode *dist, struct DListNode *item) {
	struct DListNode *node = dist;
	if (node == NULL) {
		return;
	}
	while (node->right != NULL) {
		node = node->right;
	}
	node->right = item;
	item->left = node;
}

#endif /* __UIENGINE_DLIST_H__ */