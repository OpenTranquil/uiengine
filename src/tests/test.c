#include <stdio.h>
#include "../include/assert.h"
#include "../include/web/html_document.h"

DEFINE_TEST_CASE(element_id_should_equal) {
    struct HtmlDocument* document = document_load("<body id=\"test\">sss</body>");
    struct HtmlElement* element = document->get_element_by_id(document, "test");
    ASSERT_NOT_NULL(element);
    ASSERT_STR_EQUAL(element->dom.id, "test");
    return 0;
}

DEFINE_TEST_CASE(document_should_consume_whitespace) {
    char *doc = "          <body></body>";
    doc = document_consume_whitespace(doc);
    ASSERT_STR_EQUAL(doc, "<body></body>");
    return 0;
}

DEFINE_TEST_CASE(document_should_match_and_consume) {
    char *doc = "<body></body>";
    doc = document_match_and_consume(doc, "<");
    ASSERT_STR_EQUAL(doc, "body></body>");
    doc = document_match_and_consume(doc, "body");
    ASSERT_STR_EQUAL(doc, "></body>");
    doc = document_match_and_consume(doc, ">");
    ASSERT_STR_EQUAL(doc, "</body>");
    doc = document_match_and_consume(doc, "</");
    ASSERT_STR_EQUAL(doc, "body>");
    doc = document_match_and_consume(doc, "body");
    ASSERT_STR_EQUAL(doc, ">");
    doc = document_match_and_consume(doc, ">");
    ASSERT_STR_EQUAL(doc, "\0");
    return 0;
}

DEFINE_TEST_CASE(document_should_parse_tag) {
    char *doc = "body></body>";
    char *tag = document_parse_tag(doc);
    ASSERT_STR_EQUAL(tag, "body");
    return 0;
}

DEFINE_TEST_CASE(document_should_parse_tag_2) {
    char *doc = "<body>sss</body>";
    HtmlDocument *document = document_load(doc);
    ASSERT_NOT_NULL(document);
    ASSERT_NOT_NULL(document->body);
    ASSERT_NOT_NULL(document->body->dom.tag);
    ASSERT_STR_EQUAL(document->body->dom.tag, "body");
    return 0;
}

DEFINE_TEST_CASE(document_should_parse_attr) {
    char *doc = "<body id=\"test_id\">sss</body>";
    HtmlDocument *document = document_load(doc);
    ASSERT_NOT_NULL(document);
    ASSERT_NOT_NULL(document->body);
    ASSERT_NOT_NULL(document->body->dom.attributes);
    ASSERT_NOT_NULL(document->body->dom.attributes->key);
    ASSERT_STR_EQUAL(document->body->dom.attributes->key, "id");
    ASSERT_NOT_NULL(document->body->dom.attributes->val);
    ASSERT_STR_EQUAL(document->body->dom.attributes->val, "test_id");
    return 0;
}

DEFINE_TEST_CASE(document_should_parse_attr2) {
    char *doc = "<body id=\"test_id\" class=\"test_class\">sss</body>";
    HtmlDocument *document = document_load(doc);
    ASSERT_NOT_NULL(document);
    ASSERT_NOT_NULL(document->body);

    HtmlDomElement dom = document->body->dom;
    HtmlAttribute *attr = dom.attributes;
    ASSERT_NOT_NULL(attr);
    ASSERT_NOT_NULL(attr->key);
    ASSERT_STR_EQUAL(attr->key, "id");
    ASSERT_NOT_NULL(attr->val);
    ASSERT_STR_EQUAL(attr->val, "test_id");

    HtmlAttribute *attr2 = ContainerOf(attr->node.right, HtmlAttribute, node);
    ASSERT_NOT_NULL(attr2);
    ASSERT_NOT_NULL(attr2->key);
    ASSERT_STR_EQUAL(attr2->key, "class");
    ASSERT_NOT_NULL(attr2->val);
    ASSERT_STR_EQUAL(attr2->val, "test_class");

    return 0;
}

DEFINE_TEST_CASE(document_should_parse_attr3) {
    char *doc = "<body id=\"test_id\" class=\"test_class\" onclick=\"hello\">sss</body>";
    HtmlDocument *document = document_load(doc);
    ASSERT_NOT_NULL(document);
    ASSERT_NOT_NULL(document->body);

    HtmlDomElement dom = document->body->dom;
    HtmlAttribute *attr = dom.attributes;
    ASSERT_NOT_NULL(attr);
    ASSERT_NOT_NULL(attr->key);
    ASSERT_STR_EQUAL(attr->key, "id");
    ASSERT_NOT_NULL(attr->val);
    ASSERT_STR_EQUAL(attr->val, "test_id");

    HtmlAttribute *attr2 = ContainerOf(attr->node.right, HtmlAttribute, node);
    ASSERT_NOT_NULL(attr2);
    ASSERT_NOT_NULL(attr2->key);
    ASSERT_STR_EQUAL(attr2->key, "onclick");
    ASSERT_NOT_NULL(attr2->val);
    ASSERT_STR_EQUAL(attr2->val, "hello");

    HtmlAttribute *attr3 = ContainerOf(attr2->node.right, HtmlAttribute, node);
    ASSERT_NOT_NULL(attr3);
    ASSERT_NOT_NULL(attr3->key);
    ASSERT_STR_EQUAL(attr3->key, "class");
    ASSERT_NOT_NULL(attr3->val);
    ASSERT_STR_EQUAL(attr3->val, "test_class");

    return 0;
}

DEFINE_TEST_CASE(document_should_parse_content_children) {
    char *doc = "<body id=\"test_id\">sss</body>";
    HtmlDocument *document = document_load(doc);
    ASSERT_NOT_NULL(document);
    ASSERT_NOT_NULL(document->body);
    ASSERT_NOT_NULL(document->body->dom.attributes);
    ASSERT_NOT_NULL(document->body->dom.attributes->key);
    ASSERT_STR_EQUAL(document->body->dom.attributes->key, "id");
    ASSERT_NOT_NULL(document->body->dom.attributes->val);
    ASSERT_STR_EQUAL(document->body->dom.attributes->val, "test_id");
    ASSERT_NOT_NULL(document->body->dom.childrens);
    ASSERT_STR_EQUAL(document->body->dom.childrens->content.content, "sss");
    return 0;
}

DEFINE_TEST_CASE(document_should_parse_dom_children) {
    char *doc = "<body id=\"test_id\"><p>text</p></body>";
    HtmlDocument *document = document_load(doc);
    ASSERT_NOT_NULL(document);
    ASSERT_NOT_NULL(document->body);
    ASSERT_NOT_NULL(document->body->dom.attributes);
    ASSERT_NOT_NULL(document->body->dom.attributes->key);
    ASSERT_STR_EQUAL(document->body->dom.attributes->key, "id");
    ASSERT_NOT_NULL(document->body->dom.attributes->val);
    ASSERT_STR_EQUAL(document->body->dom.attributes->val, "test_id");
    ASSERT_NOT_NULL(document->body->dom.childrens);
    ASSERT_STR_EQUAL(document->body->dom.childrens->dom.tag, "p");
    ASSERT_NOT_NULL(document->body->dom.childrens->dom.childrens);
    ASSERT_STR_EQUAL(document->body->dom.childrens->dom.childrens->content.content, "text");
    return 0;
}

DEFINE_TEST_CASE(document_should_parse_dom_children2) {
    char *doc = "<body id=\"test_id\"><p>text</p><button>click</button></body>";
    HtmlDocument *document = document_load(doc);
    ASSERT_NOT_NULL(document);

    HtmlElement *element = document->body;
    ASSERT_NOT_NULL(element);
    ASSERT_NOT_NULL(element->dom.attributes);
    ASSERT_NOT_NULL(element->dom.attributes->key);
    ASSERT_STR_EQUAL(element->dom.attributes->key, "id");
    ASSERT_NOT_NULL(element->dom.attributes->val);
    ASSERT_STR_EQUAL(element->dom.attributes->val, "test_id");

    HtmlElement* children = element->dom.childrens;
    ASSERT_NOT_NULL(children);
    ASSERT_STR_EQUAL(children->dom.tag, "p");

    HtmlElement* children_children = children->dom.childrens;
    ASSERT_NOT_NULL(children_children);
    ASSERT_STR_EQUAL(children_children->content.content, "text");
    ASSERT_NULL(children_children->node.right);

    HtmlElement *children2 = ContainerOf(children->node.right, HtmlElement, node);
    ASSERT_NOT_NULL(children2);
    ASSERT_NOT_NULL(children2->dom.tag);
    ASSERT_STR_EQUAL(children2->dom.tag, "button");
    ASSERT_NULL(children2->node.right);

    HtmlElement *children2_children = children2->dom.childrens;
    ASSERT_NOT_NULL(children2_children);
    ASSERT_STR_EQUAL(children2_children->content.content, "click");
    ASSERT_NULL(children2_children->node.right);
    return 0;
}

DEFINE_TEST_CASE(document_should_parse_dom_children_with_attr) {
    char *doc = "<body id=\"test_id\"><p id=\"name\">text</p></body>";
    HtmlDocument *document = document_load(doc);
    ASSERT_NOT_NULL(document);
    ASSERT_NOT_NULL(document->body);
    ASSERT_NOT_NULL(document->body->dom.attributes);
    ASSERT_NOT_NULL(document->body->dom.attributes->key);
    ASSERT_STR_EQUAL(document->body->dom.attributes->key, "id");
    ASSERT_NOT_NULL(document->body->dom.attributes->val);
    ASSERT_STR_EQUAL(document->body->dom.attributes->val, "test_id");
    ASSERT_NOT_NULL(document->body->dom.childrens);
    ASSERT_STR_EQUAL(document->body->dom.childrens->dom.tag, "p");
    ASSERT_NOT_NULL(document->body->dom.childrens->dom.childrens);
    ASSERT_STR_EQUAL(document->body->dom.childrens->dom.childrens->content.content, "text");
    ASSERT_NOT_NULL(document->body->dom.childrens->dom.attributes);
    ASSERT_NOT_NULL(document->body->dom.childrens->dom.attributes->key);
    ASSERT_STR_EQUAL(document->body->dom.childrens->dom.attributes->key, "id");
    ASSERT_NOT_NULL(document->body->dom.childrens->dom.attributes->val);
    ASSERT_STR_EQUAL(document->body->dom.childrens->dom.attributes->val, "name");
    return 0;
}

int main(int argc, char* argv[]) {
    // RUN_ALL_TESTS();
    TEST_document_should_consume_whitespace();
    TEST_document_should_match_and_consume();
    TEST_document_should_parse_tag();
    TEST_document_should_parse_tag_2();
    TEST_document_should_parse_attr();
    TEST_document_should_parse_attr2();
    TEST_document_should_parse_attr3();
    TEST_document_should_parse_content_children();
    TEST_document_should_parse_dom_children();
    TEST_document_should_parse_dom_children2();
    TEST_document_should_parse_dom_children_with_attr();

    // TEST_element_id_should_equal();
    return 0;
}