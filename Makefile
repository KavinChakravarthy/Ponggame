build:
	gcc -Wall -std=c17 -I src/include -L src/lib -o main main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
run:
	./main
clean:
	rm main