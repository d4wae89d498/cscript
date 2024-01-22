all:
	clang -xc -g -fms-extensions -Wno-microsoft-anon-tag -lgc -include gc.h -I. -include cedilla.h -include assert.h  .root.c
