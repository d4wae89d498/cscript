all:
	clang -xc -g -fms-extensions -Wno-microsoft-anon-tag -lgc tests/main.c -o tests/main 

clean: 
	rm tests/main
