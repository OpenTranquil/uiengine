#ifndef __UIENGINE_RENDERER_RENDERER_H__
#define __UIENGINE_RENDERER_RENDERER_H__

#include <stdint.h>
#include <stddef.h>
#include "type.h"
#include "render_node.h"

typedef enum RendererState {
    RENDERER_STATE_RUNNING,
    RENDERER_STATE_STOP,
} RendererState;

typedef void (*RenderBackendDrawRect)(struct RenderBackend *, Position, Size, Color);
typedef void (*RenderBackendDrawText)(struct RenderBackend *, Position, Size, Color, char *text);
typedef void (*RenderBackendFillCircle)(struct RenderBackend *backend, Position pos, Radius radius, Color color);
typedef void (*RenderBackendInit)(struct RenderBackend *, const char *winTitle, uint32_t fullscreen, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
typedef void (*RenderBackendSubmit)(struct RenderBackend *);
typedef void (*RenderBackendClear)(struct RenderBackend *);
typedef Event (*RenderBackendPolling)(struct RenderBackend *);
typedef uint32_t (*RenderBackendGetScreenHeight)(struct RenderBackend *);
typedef uint32_t (*RenderBackendGetScreenWidth)(struct RenderBackend *);
typedef struct RenderBackend {
    RenderBackendInit init;
    RenderBackendSubmit submit;
    RenderBackendClear clear;
    RenderBackendPolling polling;
    RenderBackendDrawRect drawRect;
    RenderBackendDrawRect fillRect;
    RenderBackendFillCircle fillCircle;

    RenderBackendDrawText drawText;

    RenderBackendGetScreenHeight getScreenHeight;
    RenderBackendGetScreenWidth getScreenWidth
} RenderBackend;

typedef void (*RendererRegisterBackend)(struct Renderer *, RenderBackend *);
typedef void (*RendererSetRootRenderNode)(struct Renderer *, struct RenderNode *);
typedef void (*RendererDrawRect)(struct Renderer *, Position, Size, Color);
typedef void (*RendererFillCircle)(struct Renderer *, Position pos, Radius radius, Color color);
typedef void (*RendererDrawText)(struct Renderer *, Position, Size, Color, char*);
typedef void (*RendererRender)(struct Renderer *);
typedef void (*RendererProcessEvent)(struct Renderer *);
typedef void (*RendererInit)(struct Renderer *, const char *winTitle, uint32_t fullscreen, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
typedef void (*RendererDestroy)(struct Renderer *);
typedef void (*RendererDump)(struct Renderer *);
typedef uint32_t (*RendererGetWindowWidth)(struct Renderer *);
typedef uint32_t (*RendererGetWindowHeight)(struct Renderer *);
typedef void (*RenderOnMouseMotion)(struct Renderer *, Event e);
typedef struct Renderer {
    RendererState runningState;

    // TODO: move to window object
    uint32_t windowFullscreen;
    uint32_t windowWidth;
    uint32_t windowHeight;

    RendererGetWindowWidth getWindowWidth;
    RendererGetWindowHeight getWindowHeight;

    RenderBackend *renderBackend;
    struct RenderNode *rootNode;

    RenderOnMouseMotion onMouseMotion;

    RendererRegisterBackend registerBackend;
    RendererSetRootRenderNode setRootRenderNode;

    RendererRender render;
    RendererInit init;
    RendererProcessEvent processEvent;
    RendererDestroy destroy;

    RendererDump dump;

    RendererDrawRect drawRect;
    RendererDrawRect fillRect;
    RendererFillCircle fillCircle;
    RendererDrawText drawText;
} Renderer;

Renderer *render_create();

#endif /* __UIENGINE_RENDERER_RENDERER_H__ */