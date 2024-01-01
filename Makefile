CC=gcc
CFLAGS=-I./engine/include -I/opt/homebrew/Cellar/sdl2/2.28.5/include -I/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include -g

LIB=-lSDL2 -lSDL2_ttf -lSDL2main `sdl2-config --cflags --libs` -lpthread

COMMON_SRCS = engine/src/web/html_document.c	\
	engine/src/mem/mem.c	\
	engine/src/renderer/renderer.c	\
	engine/src/renderer/render_node.c	\
	engine/src/renderer/component/button.c	\
	engine/src/renderer/component/circle.c	\
	engine/src/renderer/component/text.c	\
	engine/src/renderer/backend/sdl2/backend_sdl2.c

CACULATOR_SRCS = examples/caculator/caculator.c $(COMMON_SRCS)

ANIMATION_SRCS = examples/animation/animation.c  $(COMMON_SRCS)

EYE_TRAINER_SRCS = examples/eye_trainer/eye_trainer.c  $(COMMON_SRCS)

TEST_SRCS = tests/test.c tests/web/html_document.c tests/mem/mem.c

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
	rm caculator test animation eye_trainer
