#define print(...) FOR_EACH(_print_generic, __VA_ARGS__)
#define _print_generic(x) printf(_Generic((x), \
    int: "%d", \
    long: "%ld", \
    long long: "%lld", \
    unsigned int: "%u", \
    unsigned long: "%lu", \
    unsigned long long: "%llu", \
    float: "%f", \
    double: "%f", \
    char: "%c", \
    char*: "%s", \
    const char*: "%s", \
    void*: "%p", \
    default: "<Unknown type: %p>" \
), x);




