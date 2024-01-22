class_definition(String,,
	((char *ptr, u64 len),
		this->ptr = GC_MALLOC(len);
		this->len = len;
		strcpy(this->ptr, ptr);
	),
	(bool, is_raw, (char *str),
		return !strcmp(this->ptr, str);
	)
)

function(String*, String_construct_raw, (char *ptr),
         return String_construct(ptr, strlen(ptr));
)

/*
// Optional argument for len. If not set, set it to strlen(ptr).
#define String_construct(X, ...)\
	IF_ARGS((__VA_ARGS__),\
		String_construct(X, __VA_ARGS__)\
	)\
	IF_NARGS((__VA_ARGS__),\
		String_construct(X, strlen(X))\
	)
*/
