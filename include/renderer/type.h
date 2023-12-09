#ifndef __UIENGINE_RENDERER_TYPE_H__
#define __UIENGINE_RENDERER_TYPE_H__

#include <stdint.h>

typedef unsigned char bool;
#define true 1
#define false 0 

typedef struct Position {
    uint32_t x;
    uint32_t y;
} Position;

typedef struct Size {
    uint32_t width;
    uint32_t height;
} Size;

typedef struct Padding {
    uint32_t left;
    uint32_t right;
    uint32_t top;
    uint32_t bottom;
} Padding;

typedef struct Margin {
    uint32_t left;
    uint32_t right;
    uint32_t top;
    uint32_t bottom;
} Margin;

typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;

typedef enum EventType {
    EVENT_EXIT,
    EVENT_MOUSE_MOTION,
    EVENT_MOUSE_LEFT_UP,
    EVENT_MOUSE_LEFT_DOWN,
    EVENT_MOUSE_MIDDLE_UP,
    EVENT_MOUSE_MIDDLE_DOWN,
    EVENT_MOUSE_RIGHT_UP,
    EVENT_MOUSE_RIGHT_DOWN,
    EVENT_KEY_UP,
    EVENT_KEY_DOWN,
    EVENT_UNKNOWN,
} EventType;

typedef struct Event {
    EventType type;
    union {
        struct {
            uint32_t x;
            uint32_t y;
        } mouseMove;
        struct {
            uint32_t x;
            uint32_t y;
        } leftMouseDown;
        struct {
            uint32_t x;
            uint32_t y;
        } leftMouseUp;
         struct {
            uint32_t x;
            uint32_t y;
        } rightMouseDown;
        struct {
            uint32_t x;
            uint32_t y;
        } rightMouseUp;
        struct {
            uint32_t key;
        } keyboardUp;
         struct {
            uint32_t key;
        } keyboardDown;
    };
} Event;

#endif /* __UIENGINE_RENDERER_TYPE_H__ */