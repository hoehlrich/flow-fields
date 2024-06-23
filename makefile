CFLAGS = -Wall -Wextra -I/usr/include/SDL2
LDFLAGS = -Wl,-rpath=/usr/local/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2_image -lm

flowfields : main.o draw.o event.o perlin.o flowfields.h
	gcc *.o *.h -o flowfields $(LDFLAGS) $(CFLAGS)

%.o : %.c
	gcc -c $< -o $@ $(CFLAGS)

clean :
	rm *.o flowfields
