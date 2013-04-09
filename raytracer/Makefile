CC = gcc
CFLAGS = -O3 -Wall -lcsfml-graphics -lcsfml-window -lcsfml-system  -lpthread -std=gnu99

mac: 
	$(CC) $(CFLAGS) *.c -o raytracer

linux:
	$(CC) $(CFLAGS) -lm *.c -o raytracer

clean:
	rm -f *.o