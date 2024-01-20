#import "std/string.h"
#import "std/any.h"
#import "std/list.h"
#import "std/tree.h"

#import "std/string.m"
#import "std/any.m"
#import "std/list.m"
#import "std/tree.m"

print("_____\n");
print("STD TESTS\n");


auto raw = "hello";

auto str = String_construct(raw);

assert(str->len == strlen(raw));
assert(!strcmp(str->ptr, raw));
assert(str->is_raw(raw));
print("String class tests passed.\n");

print("STD TESTS END\n");
print("_____\n");

