SRC=tests/main.c#test-gcc.c

all: gcc clang

clang:
	clang -xc -g -fms-extensions -Wno-microsoft-anon-tag $(SRC)

gcc:
	/usr/local/Cellar/gcc/13.2.0/bin/gcc-13  -xc -g -fms-extensions $(SRC)

#	clang -xc -g -fms-extensions -Wno-microsoft-anon-tag -lgc tests/main.c -o tests/main

clean:
	rm tests/main
