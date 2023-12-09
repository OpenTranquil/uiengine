CC=gcc
CFLAGS=-I. -I/opt/homebrew/Cellar/sdl2/2.26.5/include -I/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include -g

LIB=-lSDL2 -lSDL2_ttf -lSDL2main `sdl2-config --cflags --libs` -lpthread

COMMON_SRCS = src/web/html_document.c	\
	src/mem/mem.c	\
	src/renderer/renderer.c	\
	src/renderer/render_node.c	\
	src/renderer/component/button.c	\
	src/renderer/backend/sdl2/backend_sdl2.c

DEMO_SRCS = src/demo.c $(COMMON_SRCS)

DEMO_RENDER_SRCS = src/demo_render.c  $(COMMON_SRCS)

TEST_SRCS = src/test.c src/web/html_document.c src/mem/mem.c

demo_render: $(DEMO_RENDER_SRCS)
	$(CC) $(CFLAGS) -o demo_render $(DEMO_RENDER_SRCS) $(LIB)

demo: $(DEMO_SRCS)
	$(CC) $(CFLAGS) -o demo $(DEMO_SRCS) $(LIB)

test: $(COMMON_SRCS)
	$(CC) $(CFLAGS) -o test $(TEST_SRCS)

runDemo: demo
	./demo

runTest: test
	./test

.PHONY: clean
clean:
	rm demo test