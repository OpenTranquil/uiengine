#include "renderer/renderer.h"
#include "mem/mem.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   380

SDL_Window* gWindow = NULL;
SDL_Renderer *sdlRenderer = NULL;
TTF_Font *gFont = NULL;

typedef struct TextTexture {
    char *text;
    Color color;
    SDL_Texture *texture;
    DListNode node;
} TextTexture;

// TODO: Tempory use dlist, wiall change to hashmap latter
TextTexture *textTextureCache = NULL;

SDL_Texture *sdl_font_texutre_cache_get(Color color, char *text) {
    if (textTextureCache == NULL) {
        return NULL;
    }
    DListNode *list = &textTextureCache->node;
    while (list != NULL) {
        TextTexture *texture = ContainerOf(list, TextTexture, node);
        if ((strcmp(texture->text, text) == 0) &&
            ((texture->color.r == color.r) && 
            (texture->color.r == color.r) && 
            (texture->color.r == color.r) && 
            (texture->color.r == color.r))) {
                return texture->texture;
            }
        list = list->right;
    }
    return NULL;
}

void sdl_font_texutre_cache_set(Color color, char *text, SDL_Texture *fontTexture) {
    TextTexture *textTexture = (TextTexture *)mem_alloc(sizeof(TextTexture));
    INIT_DNODE(textTexture->node);

    char *str = malloc(strlen(text) + 1);
    strcpy(str, text);
    textTexture->color = color;
    textTexture->text = str;
    textTexture->texture = fontTexture;
    if (textTextureCache == NULL) {
        textTextureCache = textTexture;
    } else {
        dlist_insert(&textTextureCache->node, &textTexture->node);
    }
}

void sdl2_backend_default_draw_text(RenderBackend *sdl2, Position pos, Size size, Color color, char *text) {
    // TODO: margin for text
    SDL_Texture *fontTexture = sdl_font_texutre_cache_get(color, text);
    if (fontTexture == NULL) {
        SDL_Color col;
        col.r = color.r;
        col.g = color.g;
        col.b = color.b;
        col.a = color.a;
        SDL_Surface *surface = TTF_RenderText_Blended(gFont, text, col);
        fontTexture = SDL_CreateTextureFromSurface(sdlRenderer, surface);
        if (fontTexture == NULL) {
            printf("TTF: create texture for text '%s' failed!\n", text);
        }
        SDL_FreeSurface(surface);
        sdl_font_texutre_cache_set(color, text, fontTexture);
    }
    SDL_Rect textQuad = {
        pos.x, pos.y, size.width, size.height
    };
    SDL_RenderCopy(sdlRenderer, fontTexture, NULL, &textQuad);
}

void sdl2_backend_default_draw_rect(RenderBackend *sdl2, Position pos, Size size, Color borderColor) {
    SDL_SetRenderDrawColor(sdlRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_Rect rect = {
        pos.x, pos.y, size.width, size.height
    };
    SDL_RenderDrawRect(sdlRenderer, &rect);
    Color c = {255,255,255,0};
}

void sdl2_backend_default_fill_rect(RenderBackend *sdl2, Position pos, Size size, Color backgroundColor) {
    SDL_SetRenderDrawColor(sdlRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_Rect rect = {
        pos.x, pos.y, size.width, size.height
    };
    SDL_RenderFillRect(sdlRenderer, &rect);
}

void sdl2_backend_default_fill_circle(RenderBackend *sdl2, Position pos, Radius radius, Color color) {
    SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
    int offsetx = 0;
    int offsety = radius.topleft;
    int d = radius.topleft - 1;
    int status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawLine(sdlRenderer, pos.x - offsety, pos.y + offsetx,
                                     pos.x + offsety, pos.y + offsetx);
        status += SDL_RenderDrawLine(sdlRenderer, pos.x - offsetx, pos.y + offsety,
                                     pos.x + offsetx, pos.y + offsety);
        status += SDL_RenderDrawLine(sdlRenderer, pos.x - offsetx, pos.y - offsety,
                                     pos.x + offsetx, pos.y - offsety);
        status += SDL_RenderDrawLine(sdlRenderer, pos.x - offsety, pos.y - offsetx,
                                     pos.x + offsety, pos.y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx +=1;
        } else if (d < 2 * (radius.topleft - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        } else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void sdl2_backend_default_init(struct RenderBackend *sdl2, const char *winTitle, uint32_t fullscreen, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    gWindow = SDL_CreateWindow(winTitle, x, y, width, height, SDL_WINDOW_SHOWN );
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    sdlRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if (sdlRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetVSync(sdlRenderer, -1);
    if (fullscreen) {
        SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN);
    }
    if (TTF_Init() < 0) {
        printf("TTF int failed! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    gFont = TTF_OpenFont("./puhui.ttf", 64);
    if (gFont == NULL) {
        printf("TTF open failed! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    return;
}

void sdl2_backend_default_submit(RenderBackend *sdl2) {
    SDL_RenderPresent(sdlRenderer);
}

void sdl2_backend_default_clear(RenderBackend *sdl2) {
    SDL_SetRenderDrawColor(sdlRenderer, 0xfe, 0xfe, 0xfe, 0);
    SDL_RenderClear(sdlRenderer);
}

Event sdl2_backend_default_polling(RenderBackend *sdl2) {
    SDL_Event e;
    Event retE;
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) {
        retE.type = EVENT_EXIT;
    } else if (e.type == SDL_MOUSEMOTION) {
        retE.type = EVENT_MOUSE_MOTION;
        retE.mouseMove.x = e.motion.x;
        retE.mouseMove.y = e.motion.y;
    } else if (e.type == SDL_MOUSEBUTTONUP) {
        if (e.button.button == 1) {
            retE.type = EVENT_MOUSE_LEFT_UP;
        }
        retE.leftMouseUp.x = e.button.x;
        retE.leftMouseUp.y = e.button.y;
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == 1) {
            retE.type = EVENT_MOUSE_LEFT_DOWN;
        }
        retE.leftMouseUp.x = e.button.x;
        retE.leftMouseUp.y = e.button.y;
    } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            retE.type = EVENT_EXIT;
        } else {
            retE.type = EVENT_KEY_DOWN;
            retE.keyboardDown.key = e.key.keysym.sym;
        }
    } else if (e.type == SDL_KEYUP) {
        retE.type = EVENT_KEY_UP;
        retE.keyboardUp.key = e.key.keysym.sym;
    } else {
        retE.type = EVENT_UNKNOWN;
    }
    return retE;
}

uint32_t sdl2_backend_default_get_screen_height(RenderBackend *sdl2) {
    uint32_t width = 0;
    uint32_t height = 0;
    SDL_GetRendererOutputSize(sdlRenderer, &width, &height);
    return height;
}

uint32_t sdl2_backend_default_get_screen_width(RenderBackend *sdl2) {
    uint32_t width = 0;
    uint32_t height = 0;
    SDL_GetRendererOutputSize(sdlRenderer, &width, &height);
    return width;
}

RenderBackend *backend_sdl2_create() {
    RenderBackend *sdl2  = (RenderBackend *)mem_alloc(sizeof(RenderBackend));
    sdl2->drawRect = sdl2_backend_default_draw_rect;
    sdl2->fillRect = sdl2_backend_default_fill_rect;
    sdl2->fillCircle = sdl2_backend_default_fill_circle;
    sdl2->drawText = sdl2_backend_default_draw_text;
    sdl2->submit = sdl2_backend_default_submit;
    sdl2->clear = sdl2_backend_default_clear;
    sdl2->polling = sdl2_backend_default_polling;
    sdl2->init = sdl2_backend_default_init;

    sdl2->getScreenHeight = sdl2_backend_default_get_screen_height;
    sdl2->getScreenWidth = sdl2_backend_default_get_screen_width;
    return sdl2;
}