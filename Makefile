build:
	gcc main.c -lm -o pseudocod

install: build
	mv pseudocod /bin/pseudocod

clean:
	rm pseudocod

