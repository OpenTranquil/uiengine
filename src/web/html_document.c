#include "../../include/web/html_document.h"
#include "../../include/mem/mem.h"
#include "../../include/assert.h"
#include "../../include/renderer/component/button.h"

const char *ElementTypeName[HTML_ELEMENT_TYPE_MAX] = {"dom", "text"};

static inline iswhitespace(char c) {
    return (c == ' ') || (c == '\t');
}

static inline isalpha(char c) {
    return ((c >= 'a') && (c <= 'z')) || 
           ((c >= 'A') && (c <= 'Z'));
}

static inline isnumeric(char c) {
    return ((c >= '0') && (c <= '9'));
}

char* document_consume_whitespace(char *doc) {
    while (iswhitespace(*doc)) {
        doc++;
    }
    return doc;
}

char* document_match_bound(char *doc) {
    if (*doc == '\"') {
        return "\"";
    } else if (*doc == '\'') {
        return "\'";
    } else {
        perror("syntax error: shoud have a bound\n");
        exit(0);
    }
}

int document_match_str(char *doc, char *str) {
    char *tok_tmp = str;
    while ((*doc != '\0') && (*tok_tmp != '\0')) {
        if (*doc == *tok_tmp) {
            doc++;
            tok_tmp++;
        } else {
            return 0;
        }
    }
    return 1;
}

char *document_match_and_consume(char *doc, char *tok) {
    char *tok_tmp = tok;
    while ((*doc != '\0') && (*tok_tmp != '\0')) {
        if (*doc == *tok_tmp) {
            doc++;
            tok_tmp++;
        } else {
            break;
        }
    }
    return doc;
}

char *document_parse_str_until(char *doc, char *bound) {
    char *str = mem_alloc(0x20);
    memset(str, 0, 0x20);
    uint32_t idx = 0;
    while ((*doc != '\0') && (*doc != *bound)) {
           str[idx++] = *doc;
           doc++;
    }
    return str;
}

char *document_parse_tag(char *doc) {
    char *tag = mem_alloc(0x20);
    memset(tag, 0, 0x20);
    uint32_t idx = 0;
    while ((*doc != '\0')) {
        if (isalpha(*doc) || isnumeric(*doc) || *doc == '-') {
            tag[idx++] = *doc;
            doc++;
        } else {
            break;
        }
    }
    return tag;
}

HtmlElement* document_default_get_element_by_name(struct HtmlDocument *doc, char *name) {
    return NULL;
}


HtmlDocument *document_get_element_by_id(struct HtmlElement *element, char *id) {
    if (element == NULL) {
        return NULL;
    }
    if (element->type == HTML_ELEMENT_TYPE_DOM) {
        if (element->dom.id == NULL) {
            return NULL;
        }
        if (strcmp(element->dom.id, id) == 0) {
            return element;
        }
        HtmlElement *childrens = element->dom.childrens;
        if (childrens != NULL) {
            DListNode *node = &childrens->node;
            while (node != NULL) {
                HtmlElement *e = document_get_element_by_id(ContainerOf(node, HtmlElement, node), id);
                if (e != NULL) {
                    return e;
                }
                node = node->right;
            }
        }
    }
    return NULL;
}

// TODO: refactor by more efficency struct like hashmap
HtmlElement* document_default_get_element_by_id(struct HtmlDocument *doc, char *id) {
    HtmlElement *element = doc->body;
    return document_get_element_by_id(element, id);
}

void document_dump_element_attr(HtmlAttribute *attr) {
    if (attr == NULL) {
        printf("null");
        return;
    }
    DListNode *node = &attr->node;
    while (node != NULL) {
        HtmlAttribute *attribute = ContainerOf(node, HtmlAttribute, node);
        printf("%s='%s'", attribute->key, attribute->val);
        if (node->right != NULL) {
            printf(", ");
        }
        node = node->right;
    }
}

//[body](id='test_id')
//   ├──[p](background='rgb(10,20,30)')
//   │   └── "text"
//   ├──[div](null)
//   │   ├──[jjj](null)
//   │   └──"124"
//   └──[button](width='20px', width='20px')
//       └──"click"
static void document_dump_element(int depth, char *prefix, HtmlElement *element) {
    if (element == NULL){
        return;
    }

    for (int i = 0; i < depth; i++){ 
        printf("│   ");
    } 
    if (element->type == HTML_ELEMENT_TYPE_CONTENT) {
        printf("%s \"%s\"\n", prefix, element->content.content);
        return;
    }

    printf("%s[%s](", prefix, element->dom.tag);
    document_dump_element_attr(element->dom.attributes);
    printf(")\n");

    HtmlElement *children = element->dom.childrens;
    if (children == NULL) {
        return;
    }
    DListNode *node = &children->node;
    while (node != NULL) {
        HtmlElement *elem = ContainerOf(node, HtmlElement, node);
        depth++;
        if (node->right != NULL) {
            document_dump_element(depth, "├──", elem);
        } else {
            document_dump_element(depth, "└──", elem);
        }
        depth--;
        node = node->right;
    }
}

void document_default_dump(struct HtmlDocument *doc) {
    document_dump_element(-1, "", doc->body);
}

static RenderNode *document_create_render_component(HtmlElement *element) {
    RenderNode *renderNode = NULL;
    if (element->type == HTML_ELEMENT_TYPE_DOM) {
        if (strcmp(element->dom.tag, "button") == 0) {
            Button *button = button_create();
            renderNode = &button->renderNode;
        } else if (strcmp(element->dom.tag, "body") == 0) {
            Button *button = button_create();
            renderNode = &button->renderNode;
        } else {
            // FIXME: there is no component for tag
            printf("No component mapping found for tag:'%s'\n", element->dom.tag);
            return NULL;
        }
    } else {
        // FIXME: here should creat text component
        Button *button = button_create();
        renderNode = &button->renderNode;
    }
    INIT_DNODE(renderNode->node);
    renderNode->dom = NULL;
    return renderNode;
}

HtmlDomFunc *document_find_dom_func_by_name(HtmlDocument *document, char *name) {
    if (name == NULL) {
        return NULL;
    }
    if (document->domFuncs == NULL) {
        return NULL;
    }
    DListNode *node = &document->domFuncs->node;
    while (node != NULL) {
        HtmlDomFunc *func = ContainerOf(node, HtmlDomFunc, node);
        if (strcmp(func->name, name) == 0) {
            return func;
        }
        node = node->right;
    }
    return NULL;
}

static Color document_attribute_parse_rgb_color(char *colorStr) {
    colorStr = document_match_and_consume(colorStr, "rgb(");

    char *r = document_parse_str_until(colorStr, ",");
    colorStr = document_match_and_consume(colorStr, r);
    colorStr = document_match_and_consume(colorStr, ",");
    colorStr = document_consume_whitespace(colorStr);

    char *g = document_parse_str_until(colorStr, ",");
    colorStr = document_match_and_consume(colorStr, g);
    colorStr = document_match_and_consume(colorStr, ",");
    colorStr = document_consume_whitespace(colorStr);

    char *b = document_parse_str_until(colorStr, ")");
    colorStr = document_match_and_consume(colorStr, b);
    colorStr = document_match_and_consume(colorStr, ")");
    colorStr = document_consume_whitespace(colorStr);

    Color color;
    color.r = atoi(r);
    color.g = atoi(g);
    color.b = atoi(b);
    return color;
}

// TODO: need rafactor by mapping
//      void (*func)(HtmlAttribute*) = get_attribute_set_func(attribute->key);
//      func(attribute);
//
void documen_set_render_node_attribute(struct HtmlDocument *document, RenderNode *renderNode, HtmlAttribute *attributes) {
    HtmlAttribute *attr = attributes;
    if (attr != NULL) {
        DListNode *attrNode = &attr->node;
        while (attrNode != NULL) {
            HtmlAttribute *attribute = ContainerOf(attrNode, HtmlAttribute, node);
            if (strcmp(attribute->key, "width") == 0) {
                renderNode->size.width = atoi(attribute->val);
            }
            if (strcmp(attribute->key, "height") == 0) {
                renderNode->size.height = atoi(attribute->val);
            }
            if (strcmp(attribute->key, "x") == 0) {
                renderNode->pos.x = atoi(attribute->val);
            }
            if (strcmp(attribute->key, "y") == 0) {
                renderNode->pos.y = atoi(attribute->val);
            }
            if (strcmp(attribute->key, "background") == 0) {
                char *colorStr = attribute->val;
                Color color = document_attribute_parse_rgb_color(colorStr);
                renderNode->backgroundColor.r = color.r;
                renderNode->backgroundColor.g = color.g;
                renderNode->backgroundColor.b = color.b;
            }
            if (strcmp(attribute->key, "border-color") == 0) {
                char *colorStr = attribute->val;
                Color color = document_attribute_parse_rgb_color(colorStr);
                renderNode->borderColor.r = color.r;
                renderNode->borderColor.g = color.g;
                renderNode->borderColor.b = color.b;
            }
            if (strcmp(attribute->key, "onclick") == 0) {
                HtmlDomFunc *func = document_find_dom_func_by_name(document, attribute->val);
                if (func == NULL) {
                    printf("No func registered for '%s'\n", attribute->val);
                    renderNode->onCustomClick = NULL;
                } else {
                    renderNode->onCustomClick = func->func;
                }
            }
            attrNode = attrNode->right;
        }
    }
}

RenderNode *document_build_render_element(struct HtmlDocument *document, HtmlElement *element) {
    if (element == NULL) {
        return NULL;
    }
    RenderNode *renderNode = document_create_render_component(element);
    if (renderNode == NULL) {
        return NULL;
    }
    RenderNode *head = renderNode;

    bool first = true;
    DListNode *node = &element->node;
    while (node != NULL) {
        HtmlElement *elem = ContainerOf(node, HtmlElement, node);
        if (!first) {
            renderNode = document_create_render_component(element);
        }

        renderNode->size.width = 0;
        renderNode->size.height = 0;
        renderNode->pos.x = 0;
        renderNode->pos.y = 0;

        renderNode->backgroundColor.r = 0;
        renderNode->backgroundColor.g = 0;
        renderNode->backgroundColor.b = 0;
        renderNode->backgroundColor.a = 0;

        if (elem->type == HTML_ELEMENT_TYPE_DOM) {
            documen_set_render_node_attribute(document, renderNode, elem->dom.attributes);
        }

        renderNode->dom = elem;
        elem->renderNode = renderNode;
        elem->needRebuild = false;
        renderNode->children = document_build_render_element(document, elem->dom.childrens);

        if (!first) {
            dlist_insert(&head->node, &renderNode->node);
        }

        first = false;
        node = node->right;
    }
    return head;
}

RenderNode *document_default_build_render_tree(struct HtmlDocument *document) {
    RenderNode *rootNode = (RenderNode *)mem_alloc(sizeof(RenderNode));
    INIT_DNODE(rootNode->node);
    rootNode->dom = document->body;
    rootNode->children = document_build_render_element(document, document->body);
    return rootNode;
}

RenderNode *document_update_render_element(HtmlElement *element) {
    if (element == NULL) {
        return NULL;
    }

    DListNode *node = &element->node;
    while (node != NULL) {
        HtmlElement *elem = ContainerOf(node, HtmlElement, node);
        if (elem->needRebuild) {
            // FIXME: update render node by attributes
            elem->renderNode->backgroundColor.r = 0;
            elem->renderNode->backgroundColor.g = 255;
            elem->needRebuild = false;
        }
        if (elem->type == HTML_ELEMENT_TYPE_DOM) {
            document_update_render_element(elem->dom.childrens);
        }
        node = node->right;
    }
    return element->renderNode;
}

RenderNode *document_default_update_render(struct HtmlDocument *document) {
    return document_update_render_element(document->body);
}

void document_default_register_dom_func(struct HtmlDocument *document, char *name, void(*func)(HtmlDocument*)) {
    //FIXME: rb tree is better
    HtmlDomFunc *domFunc = (HtmlDomFunc *)mem_alloc(sizeof(HtmlDomFunc));
    domFunc->name = name;
    domFunc->func = func;
    INIT_DNODE(domFunc->node);

    if (document->domFuncs == NULL) {
        document->domFuncs = domFunc;
        return;
    }

    dlist_insert(&document->domFuncs->node, &domFunc->node);
}

void document_init(HtmlDocument *document) {
    document->body = NULL;
    document->domFuncs = NULL;
    document->get_element_by_id = document_default_get_element_by_id;
    document->get_element_by_name = document_default_get_element_by_name;
    document->dump = document_default_dump;
    document->buildRenderTree = document_default_build_render_tree;
    document->updateRender = document_default_update_render;
    document->registerDomFunc = document_default_register_dom_func;
}

char *document_parse_content(HtmlElement *content, char *doc) {
    char* text = document_parse_str_until(doc, "<");
    INIT_DNODE(content->node);
    content->dom.attributes = NULL;
    content->dom.childrens = NULL;
    content->type = HTML_ELEMENT_TYPE_CONTENT;
    content->content.content = text;
    doc = document_match_and_consume(doc, text);
    return doc;
}

/**
 * @brief ({WHITESPACE}({TEXT})[0:1]{WHITESPACE}({ELEMENT})*)* </
 * 
 * @param children
 * @param doc
 * @return char*
 */
char *document_parse_childrens(HtmlElement *children, char *doc) {
    HtmlElement *head = children;
    bool first = true;
    while ((*doc != '\0') &&
           (*(doc + 1) != '\0')) {
            doc = document_consume_whitespace(doc);
            if (!first) {
                children = (HtmlElement*)mem_alloc(sizeof(HtmlElement));
                INIT_DNODE(children->node);
                children->dom.attributes = NULL;
                children->dom.childrens = NULL;
                children->parent = NULL;
            }
            if ((*doc == '<')) {
                if (*(doc + 1) == '/') {
                    break;
                }
                children->type = HTML_ELEMENT_TYPE_DOM;
                doc = document_parse_element(children, doc);
            } else {
                doc = document_parse_content(children, doc);
            }
            if (!first) {
                dlist_insert(&head->node, &children->node);
            }
            first = false;
    }
    children = head;
    return doc;
}

/**
 * @brief parse attribute
 * {KEY}{WHITESPACE}={WHITESPACE}{"/'}{VALUE}{"/'}{WHITESPACE}
 * @param doc 
 * @return HtmlAttribute* 
 */
char *document_parse_attributes(HtmlElement *element, HtmlAttribute* attr, char *doc) {
    HtmlAttribute *head = attr;
    bool first = true;
    while(*doc != '>') {
        if (!first) {
            attr = (HtmlAttribute*)mem_alloc(sizeof(HtmlAttribute));
            INIT_DNODE(attr->node);
        }
        attr->key = document_parse_tag(doc);
        doc = document_match_and_consume(doc, attr->key);
        doc = document_consume_whitespace(doc);
        doc = document_match_and_consume(doc, "=");
        doc = document_consume_whitespace(doc);
        char *bound = document_match_bound(doc);
        doc = document_match_and_consume(doc, bound);
        attr->val = document_parse_str_until(doc, bound);
        doc = document_match_and_consume(doc, attr->val);
        if (strcmp(attr->key, "style") == 0) {
            // TODO: parse css
        }

        if (strcmp(attr->key, "id") == 0) {
            element->dom.id = attr->val;
        }
        doc = document_match_and_consume(doc, bound);
        doc = document_consume_whitespace(doc);

        if (!first) {
            dlist_insert(&head->node, &attr->node);
        }
        first = false;
    }
    attr = head;
    return doc;
}

/**
 * @brief parse element
 * <{TAG}{WHITESPACE}{ATTRIBUTES}{WHITESPACE}>{CHILDRENS}</{TAG}>
 * @param doc 
 * @return HtmlElement* 
 */
char *document_parse_element(HtmlElement *element, char *doc) {
    doc = document_consume_whitespace(doc);
    doc = document_match_and_consume(doc, "<");
    element->dom.tag = document_parse_tag(doc);
    doc = document_match_and_consume(doc, element->dom.tag);
    doc = document_consume_whitespace(doc);
    if (*doc != '>') {
        HtmlAttribute *attr = (HtmlAttribute*)mem_alloc(sizeof(HtmlAttribute));
        INIT_DNODE(attr->node);
        doc = document_parse_attributes(element, attr, doc);
        element->dom.attributes = attr;
    }
    doc = document_match_and_consume(doc, ">");
    if (document_match_str(doc, "</")) {
        goto close_tag;
    } else {
        HtmlElement *children = (HtmlElement*)mem_alloc(sizeof(HtmlElement));
        INIT_DNODE(children->node);
        children->dom.attributes = NULL;
        children->dom.childrens = NULL;
        children->parent = NULL;
        children->renderNode = NULL;
        children->needRebuild = true;
        doc = document_parse_childrens(children, doc);
        element->dom.childrens = children;
    }

close_tag:
    doc = document_match_and_consume(doc, "</");
    doc = document_match_and_consume(doc, element->dom.tag);
    doc = document_match_and_consume(doc ,">");

    return doc;
}

void document_parse(HtmlDocument *document, char *doc) {
    ASSERT_NOT_NULL(document);
    ASSERT_NOT_NULL(doc);
    HtmlElement *element = (HtmlElement *)mem_alloc(sizeof(HtmlElement));
    INIT_DNODE(element->node);
    element->dom.attributes = NULL;
    element->dom.childrens = NULL;
    element->parent = NULL;
    element->renderNode = NULL;
    element->needRebuild = true;
    document_parse_element(element, doc);
    document->body = element;
}

struct HtmlDocument *document_load(const char *doc) {
    HtmlDocument *document = (HtmlDocument*)mem_alloc(sizeof(HtmlDocument));
    document_init(document);
    document_parse(document, doc);
    return document;
}