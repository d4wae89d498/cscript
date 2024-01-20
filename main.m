#!./scripts/crunbang -g -fms-extensions -Wno-microsoft-anon-tag
#! -lgc -Istd -include gc.h -include cedilla.h
#! -DON_STARTUP=GC_INIT();

#import "cedilla.h"

#import "std.m"


function(void, print_proprety, (char *type, void *data),
	// TODO: use Any and class tree yo actually print the data pointer
	print("type: ", type)
	print(" data: ", data, "\n")
)


class (Optional , , (
		(int, 		x),
		(float, 	y),
		(char *, 	str)
	),
	((int x),
		printf("Base constructed with %i %p!\n", x, this);
		this->x = x;
	),
	(void, dumpX, (),
		printf("this->x %i\n", this->x);
	)
)

auto x = Optional_construct(2);

x->dumpX();

x = Optional_construct(21);
x = Optional_construct(99);

print("Class x contains:\n");
Optional_print(x, print_proprety);
