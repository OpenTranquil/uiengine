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
typedef void (*RenderBackendInit)(struct RenderBackend *);
typedef void (*RenderBackendSubmit)(struct RenderBackend *);
typedef void (*RenderBackendClear)(struct RenderBackend *);
typedef Event (*RenderBackendPolling)(struct RenderBackend *);
typedef struct RenderBackend {
    RenderBackendInit init;
    RenderBackendSubmit submit;
    RenderBackendClear clear;
    RenderBackendPolling polling;
    RenderBackendDrawRect drawRect;
    RenderBackendDrawRect fillRect;
    RenderBackendDrawText drawText;
} RenderBackend;

typedef void (*RendererRegisterBackend)(struct Renderer *, RenderBackend *);
typedef void (*RendererSetRootRenderNode)(struct Renderer *, struct RenderNode *);
typedef void (*RendererDrawRect)(struct Renderer *, Position, Size, Color);
typedef void (*RendererDrawText)(struct Renderer *, Position, Size, Color, char*);
typedef void (*RendererRender)(struct Renderer *);
typedef void (*RendererProcessEvent)(struct Renderer *);
typedef void (*RendererInit)(struct Renderer *);
typedef void (*RendererDestroy)(struct Renderer *);
typedef void (*RendererDump)(struct Renderer *);
typedef struct Renderer {
    RendererState runningState;

    RenderBackend *renderBackend;
    struct RenderNode *rootNode;

    RendererRegisterBackend registerBackend;
    RendererSetRootRenderNode setRootRenderNode;
    RendererRender render;
    RendererInit init;
    RendererProcessEvent processEvent;
    RendererDestroy destroy;

    RendererDump dump;

    RendererDrawRect drawRect;
    RendererDrawRect fillRect;
    RendererDrawText drawText;
} Renderer;

Renderer *render_create();

#endif /* __UIENGINE_RENDERER_RENDERER_H__ */