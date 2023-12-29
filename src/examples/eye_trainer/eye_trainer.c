#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "mem/mem.h"
#include "web/html_document.h"
#include "renderer/renderer.h"
#include "dlist.h"
#include "renderer/component/circle.h"
#include "renderer/component/text.h"
#include "../../../src/engine/renderer/backend/sdl2/backend_sdl2.h"

const char *winTitle = "EyeTrainer 1.0.1";
#define BALL_MAX_RADIUS 50

typedef enum BallStatus {
    BALL_ALIVE,
    BALL_DEADING,
    BALL_REROENING,
} BallStatus;

typedef struct Ball {
    Circle *circle;
    BallStatus status;
    Position targetPos;
} Ball;

Ball *ball = NULL;
Ball *player = NULL;

void onMouseMotion(struct Renderer *renderer, Event e) {
    player->circle->renderNode.pos.x = e.mouseMove.x;
    player->circle->renderNode.pos.y = e.mouseMove.y;

    if (ball->status == BALL_ALIVE) {
        uint32_t disx = abs(ball->circle->renderNode.pos.x - player->circle->renderNode.pos.x);
        uint32_t disy = abs(ball->circle->renderNode.pos.y - player->circle->renderNode.pos.y);
        uint32_t distance = sqrt(disx * disx + disy * disy);
        if (distance <= (ball->circle->renderNode.radius.topleft + player->circle->renderNode.radius.topleft)) {
            ball->status = BALL_DEADING;
            ball->targetPos.x = rand() % renderer->getWindowWidth(renderer);
            ball->targetPos.y = rand() % renderer->getWindowWidth(renderer);
        }
    }
}

void ball_init(RenderNode *rootNode) {
    Circle *ballCircle = circle_create();
    ballCircle->renderNode.pos.x = 200;
    ballCircle->renderNode.pos.y = 200;
    ballCircle->renderNode.radius.topleft = 1;
    ballCircle->renderNode.backgroundColor.r = 0;
    ballCircle->renderNode.backgroundColor.g = 255;
    ballCircle->renderNode.backgroundColor.b = 0;
    ballCircle->renderNode.backgroundColor.a = 0;
    rootNode->children = &ballCircle->renderNode;

    Circle *playerCircle = circle_create();
    playerCircle->renderNode.pos.x = 600;
    playerCircle->renderNode.pos.y = 600;
    playerCircle->renderNode.radius.topleft = 50;
    playerCircle->renderNode.backgroundColor.r = 0;
    playerCircle->renderNode.backgroundColor.g = 0;
    playerCircle->renderNode.backgroundColor.b = 255;
    playerCircle->renderNode.backgroundColor.a = 0;

    dlist_append_tail(&ballCircle->renderNode.node, &playerCircle->renderNode.node);

    Text *text = text_create(winTitle);
    text->renderNode.pos.x = 50;
    text->renderNode.pos.y = 0;
    text->renderNode.size.width = 300;
    text->renderNode.size.height = 60;
    text->renderNode.backgroundColor.r = 255;
    text->renderNode.backgroundColor.g = 255;
    text->renderNode.backgroundColor.b = 255;
    text->renderNode.backgroundColor.a = 45;

    dlist_append_tail(&ballCircle->renderNode.node, &text->renderNode.node);

    ball = (Ball *)malloc(sizeof(Ball));
    ball->status = BALL_REROENING;
    ball->circle = ballCircle;

    player = (Ball *)malloc(sizeof(Ball));
    player->status = BALL_ALIVE;
    player->circle = playerCircle;

    srand((unsigned int)time(NULL));
}

void update(struct Renderer *renderer) {
    usleep(5000); // set fps ≈ 90
    if (ball->status == BALL_REROENING) {
        if (ball->circle->renderNode.radius.topleft < BALL_MAX_RADIUS) {
            ball->circle->renderNode.radius.topleft++;
        } else {
            ball->status = BALL_ALIVE;
        }
    }
    if (ball->status == BALL_DEADING) {
        if (ball->circle->renderNode.radius.topleft > 1) {
            ball->circle->renderNode.radius.topleft--;
        } else {
            ball->status = BALL_REROENING;
            ball->circle->renderNode.pos.x = rand() % renderer->getWindowWidth(renderer);
            ball->circle->renderNode.pos.y = rand() % renderer->getWindowHeight(renderer);
        }
    }
}

int main(int argc, char* argv[]) {
    RenderNode *rootNode = (RenderNode*)mem_alloc(sizeof(RenderNode));
    ball_init(rootNode);

    Renderer *renderer = render_create();
    RenderBackend* sdl2 = backend_sdl2_create();
    renderer->registerBackend(renderer, sdl2);
    renderer->setRootRenderNode(renderer, rootNode);

    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t width = 1280;
    uint32_t height = 960;
    renderer->init(renderer, winTitle, 1, x, y, width, height);
    renderer->onMouseMotion = &onMouseMotion;

    while (renderer->runningState != RENDERER_STATE_STOP) {
        renderer->processEvent(renderer);
        update(renderer);
        renderer->render(renderer);
    }
    renderer->destroy(renderer);

    return 0;
}