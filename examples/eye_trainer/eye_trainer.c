#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "mem/mem.h"
#include "web/html_document.h"
#include "renderer/renderer.h"
#include "dlist.h"
#include "renderer/component/button.h"
#include "renderer/component/circle.h"
#include "renderer/component/text.h"
#include "../../engine/src/renderer/backend/sdl2/backend_sdl2.h"

const char *winTitle = "EyeTrainer 1.0.1";
#define BALL_MAX_RADIUS 50
#define TAIL_NUMS 12

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

typedef struct Player {
    Ball *head;
    Ball *tails[TAIL_NUMS];
} Player;

Ball *ball = NULL;
Player *player = NULL;

static uint32_t rebornLeft = 0;

uint32_t updateIndex = 1;
void onMouseMotion(struct Renderer *renderer, Event e) {
    player->head->circle->renderNode.pos.x = e.mouseMove.x;
    player->head->circle->renderNode.pos.y = e.mouseMove.y;

    player->head->targetPos.x = e.mouseMove.x;
    player->head->targetPos.y = e.mouseMove.y;

    if (ball->status == BALL_ALIVE) {
        uint32_t disx = abs(ball->circle->renderNode.pos.x - player->head->circle->renderNode.pos.x);
        uint32_t disy = abs(ball->circle->renderNode.pos.y - player->head->circle->renderNode.pos.y);
        uint32_t distance = sqrt(disx * disx + disy * disy);
        if (distance <= (ball->circle->renderNode.radius.topleft + player->head->circle->renderNode.radius.topleft)) {
            ball->status = BALL_DEADING;
            uint32_t boardWidth = renderer->getWindowWidth(renderer) / 2 - 200;
            uint32_t boardHeight = renderer->getWindowHeight(renderer) - 400;
            if (rebornLeft) {
                ball->targetPos.x = 100 + (rand() % boardWidth);
                ball->targetPos.y = 200 + (rand() % boardHeight);
                rebornLeft = 0;
            } else {
                ball->targetPos.x = (boardWidth + 300) + (rand() % boardWidth);
                ball->targetPos.y = 200 + (rand() % boardHeight);
                rebornLeft = 1;
            }
        }
    }
}

void ball_init(struct Renderer *renderer, RenderNode *rootNode) {
    Button *leftBoard = button_create();
    leftBoard->renderNode.pos.x = 100;
    leftBoard->renderNode.pos.y = 100;
    leftBoard->renderNode.size.width = renderer->getWindowWidth(renderer) / 2 - 200;
    leftBoard->renderNode.size.height = renderer->getWindowHeight(renderer) - 200;
    leftBoard->renderNode.backgroundColor.r = 0xf2;
    leftBoard->renderNode.backgroundColor.g = 0xf3;
    leftBoard->renderNode.backgroundColor.b = 0xf4;
    leftBoard->renderNode.backgroundColor.a = 0;
    leftBoard->renderNode.borderColor.r = 255;
    leftBoard->renderNode.borderColor.g = 255;
    leftBoard->renderNode.borderColor.b = 255;
    leftBoard->renderNode.borderColor.a = 0;

    rootNode->children = &leftBoard->renderNode;

    Button *rightBoard = button_create();
    rightBoard->renderNode.pos.x = renderer->getWindowWidth(renderer) / 2 + 100;
    rightBoard->renderNode.pos.y = 100;
    rightBoard->renderNode.size.width = renderer->getWindowWidth(renderer) / 2 - 200;
    rightBoard->renderNode.size.height = renderer->getWindowHeight(renderer) - 200;
    rightBoard->renderNode.backgroundColor.r = 0xf2;
    rightBoard->renderNode.backgroundColor.g = 0xf3;
    rightBoard->renderNode.backgroundColor.b = 0xf4;
    rightBoard->renderNode.backgroundColor.a = 0;
    rightBoard->renderNode.borderColor.r = 255;
    rightBoard->renderNode.borderColor.g = 255;
    rightBoard->renderNode.borderColor.b = 255;
    rightBoard->renderNode.borderColor.a = 0;
    dlist_append_tail(&leftBoard->renderNode.node, &rightBoard->renderNode.node);

    Text *text = text_create(winTitle);
    text->renderNode.pos.x = 10;
    text->renderNode.pos.y = 0;
    text->renderNode.size.width = 300;
    text->renderNode.size.height = 60;
    text->renderNode.backgroundColor.r = 0xc0;
    text->renderNode.backgroundColor.g = 0xc0;
    text->renderNode.backgroundColor.b = 0xc0;
    text->renderNode.backgroundColor.a = 0;
    dlist_append_tail(&rightBoard->renderNode.node, &text->renderNode.node);

    Circle *ballCircle = circle_create();
    ballCircle->renderNode.pos.x = 200;
    ballCircle->renderNode.pos.y = 200;
    ballCircle->renderNode.radius.topleft = 1;
    ballCircle->renderNode.backgroundColor.r = 0;
    ballCircle->renderNode.backgroundColor.g = 255;
    ballCircle->renderNode.backgroundColor.b = 0;
    ballCircle->renderNode.backgroundColor.a = 0;
    dlist_append_tail(&text->renderNode.node, &ballCircle->renderNode.node);

    ball = (Ball *)malloc(sizeof(Ball));
    ball->status = BALL_REROENING;
    ball->circle = ballCircle;

    player = (Player*)malloc(sizeof(Player));

    Circle *playerCircle = circle_create();
    playerCircle->renderNode.pos.x = 600;
    playerCircle->renderNode.pos.y = 600;
    playerCircle->renderNode.radius.topleft = BALL_MAX_RADIUS;
    playerCircle->renderNode.backgroundColor.r = 0;
    playerCircle->renderNode.backgroundColor.g = 0;
    playerCircle->renderNode.backgroundColor.b = 255;
    playerCircle->renderNode.backgroundColor.a = 0;

    player->head = (Ball *)malloc(sizeof(Ball));
    player->head->status = BALL_ALIVE;
    player->head->circle = playerCircle;

    player->tails[0] = player->head;

    DListNode *node = &ballCircle->renderNode.node;
    for (size_t i = 1; i < TAIL_NUMS; i++) {
        Circle *tmp = circle_create();
        tmp->renderNode.pos.x = player->tails[i - 1]->circle->renderNode.pos.x - 10;
        tmp->renderNode.pos.y = 600;
        int32_t radius = BALL_MAX_RADIUS - i * 3;
        tmp->renderNode.radius.topleft = radius > 1 ? radius : 1;
        tmp->renderNode.backgroundColor.r = 0 + i * 20;
        tmp->renderNode.backgroundColor.g = 0;
        tmp->renderNode.backgroundColor.b = 255;
        tmp->renderNode.backgroundColor.a = 0;
        dlist_append_tail(node, &tmp->renderNode.node);
        node = &tmp->renderNode.node;

        Ball *tmpB = (Ball *)malloc(sizeof(Ball));
        tmpB->status = BALL_ALIVE;
        tmpB->circle = tmp;
        player->tails[i] = tmpB;
    }
    dlist_append_tail(node, &playerCircle->renderNode.node);

    srand((unsigned int)time(NULL));
}

void update(struct Renderer *renderer) {
    usleep(1000);
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
            ball->circle->renderNode.pos.x = ball->targetPos.x;
            ball->circle->renderNode.pos.y = ball->targetPos.y;
        }
    }

    for (size_t i = 1; i < TAIL_NUMS; i++) {
        uint32_t disx = abs(player->tails[i]->circle->renderNode.pos.x - player->tails[i]->targetPos.x);
        uint32_t disy = abs(player->tails[i]->circle->renderNode.pos.y - player->tails[i]->targetPos.y);
        uint32_t distance = sqrt(disx * disx + disy * disy);
        uint32_t speed = distance / 8;
        if (speed == 0) {
            speed = 1;
        }
        if (player->tails[i]->circle->renderNode.pos.x != player->tails[i]->targetPos.x) {
            player->tails[i]->circle->renderNode.pos.x += (player->tails[i]->circle->renderNode.pos.x > player->tails[i]->targetPos.x ? -speed:speed);
        }
        if (player->tails[i]->circle->renderNode.pos.y != player->tails[i]->targetPos.y) {
            player->tails[i]->circle->renderNode.pos.y += (player->tails[i]->circle->renderNode.pos.y > player->tails[i]->targetPos.y ? -speed:speed);
        }
    }

    player->tails[updateIndex]->targetPos.x = player->tails[updateIndex-1]->circle->renderNode.pos.x;
    player->tails[updateIndex]->targetPos.y = player->tails[updateIndex-1]->circle->renderNode.pos.y;
    updateIndex++;
    if (updateIndex == TAIL_NUMS) {
        updateIndex = 1;
    }
}

int main(int argc, char* argv[]) {
    Renderer *renderer = render_create();
    RenderBackend* sdl2 = backend_sdl2_create();
    renderer->registerBackend(renderer, sdl2);

    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t width = 1280;
    uint32_t height = 960;
    renderer->init(renderer, winTitle, 1, x, y, width, height);
    renderer->onMouseMotion = &onMouseMotion;

    RenderNode *rootNode = (RenderNode*)mem_alloc(sizeof(RenderNode));
    ball_init(renderer, rootNode);
    renderer->setRootRenderNode(renderer, rootNode);

    while (renderer->runningState != RENDERER_STATE_STOP) {
        renderer->processEvent(renderer);
        update(renderer);
        renderer->render(renderer);
    }
    renderer->destroy(renderer);

    return 0;
}