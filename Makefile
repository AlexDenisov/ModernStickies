BINARY_PATH=/tmp/Stickies\ Hack.app/Contents/MacOS/Stickies

patch: build
	./modern_stickies $(BINARY_PATH)

build:
	clang main.c -o modern_stickies

clean:
	rm -f modern_stickies

