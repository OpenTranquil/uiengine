CC=gcc
CFLAGS=-I. -Iinclude -I/opt/homebrew/Cellar/sdl2/2.26.5/include -I/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include -g

LIB=-lSDL2 -lSDL2_ttf -lSDL2main `sdl2-config --cflags --libs` -lpthread

COMMON_SRCS = src/engine/web/html_document.c	\
	src/engine/mem/mem.c	\
	src/engine/renderer/renderer.c	\
	src/engine/renderer/render_node.c	\
	src/engine/renderer/component/button.c	\
	src/engine/renderer/component/circle.c	\
	src/engine/renderer/backend/sdl2/backend_sdl2.c

CACULATOR_SRCS = src/examples/caculator/caculator.c $(COMMON_SRCS)

ANIMATION_SRCS = src/examples/animation/animation.c  $(COMMON_SRCS)

EYE_TRAINER_SRCS = src/examples/eye_trainer/eye_trainer.c  $(COMMON_SRCS)

TEST_SRCS = src/tests/test.c src/tests/web/html_document.c src/tests/mem/mem.c

animation: $(ANIMATION_SRCS)
	$(CC) $(CFLAGS) -o animation $(ANIMATION_SRCS) $(LIB)

caculator: $(CACULATOR_SRCS)
	$(CC) $(CFLAGS) -o caculator $(CACULATOR_SRCS) $(LIB)

eye_trainer: $(EYE_TRAINER_SRCS)
	$(CC) $(CFLAGS) -o eye_trainer $(EYE_TRAINER_SRCS) $(LIB)

test: $(COMMON_SRCS)
	$(CC) $(CFLAGS) -o test $(TEST_SRCS)

runCacu: caculator
	./caculator

runTest: test
	./test

.PHONY: clean
clean:
	rm demo test demo_render