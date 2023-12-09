#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "include/mem/mem.h"
#include "include/web/html_document.h"
#include "include/renderer/renderer.h"
#include "include/renderer/component/button.h"
#include "src/renderer/backend/sdl2/backend_sdl2.h"

HtmlDocument* document = NULL;

char result[0x40] = {0};

void update_result_view(HtmlElement *elem, char *str) {
    if (elem != NULL && elem->type != HTML_ELEMENT_TYPE_DOM) {
        return;
    }
    HtmlElement *dom = document->get_element_by_id(document, "result");
    if (dom == NULL) {
        return;
    }

    if (dom->dom.childrens == NULL) {
        return;
    }
    dom->dom.childrens->content.content = str;
}

void button_0(HtmlElement *elem) { 
    update_result_view(elem, "0");
}

void button_1(HtmlElement *elem) { 
    update_result_view(elem, "1");
}

void button_2(HtmlElement *elem) { 
    update_result_view(elem, "2");
}

void button_3(HtmlElement *elem) { 
    update_result_view(elem, "3");
}

void button_4(HtmlElement *elem) { 
    update_result_view(elem, "4");
}

void button_5(HtmlElement *elem) { 
    update_result_view(elem, "5");
}

void button_6(HtmlElement *elem) { 
    update_result_view(elem, "6");
}

void button_7(HtmlElement *elem) { 
    update_result_view(elem, "7");
}

void button_8(HtmlElement *elem) { 
    update_result_view(elem, "8");
}

void button_9(HtmlElement *elem) { 
    update_result_view(elem, "9");
}

void button_add(HtmlElement *elem) { 
    update_result_view(elem, "add");
}

void button_sub(HtmlElement *elem) { 
    update_result_view(elem, "sub");
}

void button_mul(HtmlElement *elem) { 
    update_result_view(elem, "mul");
}

void button_div(HtmlElement *elem) { 
    update_result_view(elem, "div");
}

void button_ac(HtmlElement *elem) { 
    update_result_view(elem, "ac");
}

void button_mod(HtmlElement *elem) { 
    update_result_view(elem, "mod");
}

void button_add_sub(HtmlElement *elem) { 
    update_result_view(elem, "sub");
}

void button_dot(HtmlElement *elem) { 
    update_result_view(elem, "dot");
}

void button_equal(HtmlElement *elem) { 
    update_result_view(elem, "equal");
}

int main(int argc, char* argv[]) {
    char *doc =
    "<body id=\"test_id\">"
        "<button id=\"result\" onclick=\"button1_click\" width=\"320\" height=\"80\" x=\"0\" y=\"0\" border-color=\"rgb(47,41,38)\" background=\"rgb(50,45,41)\">0</button>"

        "<button onclick=\"button_ac\" width=\"80\" height=\"60\" x=\"0\" y=\"80\" border-color=\"rgb(47,41,38)\" background=\"rgb(67,62,59)\">AC</button>"
        "<button onclick=\"button_add_sub\" width=\"80\" height=\"60\" x=\"80\" y=\"80\" border-color=\"rgb(47,41,38)\" background=\"rgb(67,62,59)\">+/-</button>"
        "<button onclick=\"button_mod\" width=\"80\" height=\"60\" x=\"160\" y=\"80\" border-color=\"rgb(47,41,38)\" background=\"rgb(67,62,59)\">%</button>"
        "<button onclick=\"button_div\" width=\"80\" height=\"60\" x=\"240\" y=\"80\" border-color=\"rgb(47,41,38)\" background=\"rgb(242,163,60)\">/</button>"

        "<button onclick=\"button_7\" width=\"80\" height=\"60\" x=\"0\" y=\"140\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">7</button>"
        "<button onclick=\"button_8\" width=\"80\" height=\"60\" x=\"80\" y=\"140\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">8</button>"
        "<button onclick=\"button_9\" width=\"80\" height=\"60\" x=\"160\" y=\"140\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">9</button>"
        "<button onclick=\"button_mul\" width=\"80\" height=\"60\" x=\"240\" y=\"140\" border-color=\"rgb(47,41,38)\" background=\"rgb(242,163,60)\">x</button>"

        "<button onclick=\"button_4\" width=\"80\" height=\"60\" x=\"0\" y=\"200\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">4</button>"
        "<button onclick=\"button_5\" width=\"80\" height=\"60\" x=\"80\" y=\"200\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">5</button>"
        "<button onclick=\"button_6\" width=\"80\" height=\"60\" x=\"160\" y=\"200\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">6</button>"
        "<button onclick=\"button_sub\" width=\"80\" height=\"60\" x=\"240\" y=\"200\" border-color=\"rgb(47,41,38)\" background=\"rgb(242,163,60)\">-</button>"

        "<button onclick=\"button_1\" width=\"80\" height=\"60\" x=\"0\" y=\"260\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">1</button>"
        "<button onclick=\"button_2\" width=\"80\" height=\"60\" x=\"80\" y=\"260\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">2</button>"
        "<button onclick=\"button_3\" width=\"80\" height=\"60\" x=\"160\" y=\"260\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">3</button>"
        "<button onclick=\"button_add\" width=\"80\" height=\"60\" x=\"240\" y=\"260\" border-color=\"rgb(47,41,38)\" background=\"rgb(242,163,60)\">+</button>"

        "<button onclick=\"button_0\" width=\"160\" height=\"60\" x=\"0\" y=\"320\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">0</button>"
        "<button onclick=\"button_dot\" width=\"80\" height=\"60\" x=\"160\" y=\"320\" border-color=\"rgb(47,41,38)\" background=\"rgb(105,101,98)\">.</button>"
        "<button onclick=\"button_equal\" width=\"80\" height=\"60\" x=\"240\" y=\"320\" border-color=\"rgb(47,41,38)\" background=\"rgb(242,163,60)\">=</button>"
    "</body>";
    document = document_load(doc);

    // Binding
    document->registerDomFunc(document, "button_0", button_0);
    document->registerDomFunc(document, "button_1", button_1);
    document->registerDomFunc(document, "button_2", button_2);
    document->registerDomFunc(document, "button_3", button_3);
    document->registerDomFunc(document, "button_4", button_4);
    document->registerDomFunc(document, "button_5", button_5);
    document->registerDomFunc(document, "button_6", button_6);
    document->registerDomFunc(document, "button_7", button_7);
    document->registerDomFunc(document, "button_8", button_8);
    document->registerDomFunc(document, "button_9", button_9);
    document->registerDomFunc(document, "button_add", button_add);
    document->registerDomFunc(document, "button_sub", button_sub);
    document->registerDomFunc(document, "button_mul", button_mul);
    document->registerDomFunc(document, "button_div", button_div);
    document->registerDomFunc(document, "button_ac", button_ac);
    document->registerDomFunc(document, "button_mod", button_mod);
    document->registerDomFunc(document, "button_add_sub", button_add_sub);
    document->registerDomFunc(document, "button_dot", button_dot);
    document->registerDomFunc(document, "button_equal", button_equal);

    document->dump(document);
    printf("\nMem Usage: %d bytes\n", mem_get_usage());

    Renderer *renderer = render_create();
    RenderBackend* sdl2 = backend_sdl2_create();
    renderer->registerBackend(renderer, sdl2);

    RenderNode *rootNode = document->buildRenderTree(document);
    renderer->setRootRenderNode(renderer, rootNode);

    renderer->dump(renderer);

    renderer->init(renderer);
    while (renderer->runningState != RENDERER_STATE_STOP) {
        renderer->processEvent(renderer);
        document->updateRender(document);
        renderer->render(renderer);
    }
    renderer->destroy(renderer);

    return 0;
}