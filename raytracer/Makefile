CC = gcc
CFLAGS = -O3 -Wall -std=gnu99

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  CFLAGS += -lm
endif

.PHONY: clean

with_libpng:
	$(CC)  *.c -o raytracer $(CFLAGS) -lpng -DPNG
with_csfml:: 
	$(CC)  *.c -o raytracer $(CFLAGS) -lcsfml-graphics -lcsfml-window -lcsfml-system
clean:
	rm -f *.o raytracer
