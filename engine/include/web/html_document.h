#ifndef __UI_WEB_HTML_PARSER_H__
#define __UI_WEB_HTML_PARSER_H__

#include <stdint.h>
#include "../dlist.h"
#include "../../include/renderer/render_node.h"

typedef unsigned char bool;
#define true 1
#define false 0 

#define INIT_DNODE(node) {  \
    do {    \
        node.left = NULL;  \
        node.right = NULL; \
    } while(0); \
}

typedef struct HtmlAttribute {
    char *key;
    char *val;

    DListNode node;
} HtmlAttribute;

typedef enum HtmlElementType {
    HTML_ELEMENT_TYPE_DOM = 0,
    HTML_ELEMENT_TYPE_CONTENT,
    HTML_ELEMENT_TYPE_MAX
} HtmlElementType;

typedef struct HtmlDomElement {
    char *tag;
    char *id;
    char *name;

    HtmlAttribute *attributes;

    struct HtmlElement *childrens;
} HtmlDomElement;

typedef struct HtmlContentElement {
    char *content;
} HtmlContentElement;

typedef struct HtmlElement {
    HtmlElementType type;
    struct HtmlElement *parent;
    struct RenderNode *renderNode;
    bool needRebuild;
    DListNode node;
    union {
        HtmlDomElement dom;
        HtmlContentElement content;
    };
} HtmlElement;

typedef struct HtmlDomFunc {
    char *name;
    void (*func)(struct HtmlElement *);
    DListNode node;
} HtmlDomFunc;

typedef struct HtmlElement* (*HtmlDocumentGetElementById)(struct HtmlDocument *, char *);
typedef struct HtmlElement* (*HtmlDocumentGetElementByName)(struct HtmlDocument *, char *);
typedef void (*HtmlDocumentDump)(struct HtmlDocument *);
typedef struct RenderNode *(*HtmlDocumentBuildRenderTree)(struct HtmlDocument *);
typedef struct RenderNode *(*HtmlDocumentUpdateRender)(struct HtmlDocument *);
typedef void (*HtmlDocumentRegisterDomFunc)(struct HtmlDocument *, char *, void(*)(struct HtmlDocument*));
typedef struct HtmlDocument {
    HtmlElement *body;

    HtmlElement *head;

    HtmlDomFunc *domFuncs;

    HtmlDocumentGetElementById get_element_by_id;
    HtmlDocumentGetElementByName get_element_by_name;

    HtmlDocumentBuildRenderTree buildRenderTree;
    HtmlDocumentUpdateRender updateRender;

    HtmlDocumentRegisterDomFunc registerDomFunc;
    HtmlDocumentDump dump;
} HtmlDocument;

char* document_consume_whitespace(char *doc);
char *document_match_and_consume(char *doc, char *tok);
char *document_parse_str(char *doc, char *bound);
char *document_parse_tag(char *doc);
char *document_parse_childrens(HtmlElement *children, char *doc);
char *document_parse_attributes(HtmlElement *element, HtmlAttribute* attr, char *doc);
char *document_parse_element(HtmlElement *element, char *doc);
struct HtmlDocument *document_load(const char *doc);

#endif /* __UI_WEB_HTML_PARSER_H__ */