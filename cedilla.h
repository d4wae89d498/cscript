#ifndef CEDILLA_H
# define CEDILLA_H
# include <libc.h>
# include <assert.h>
# include <stdint.h>
# include <string.h>

/*=======================================
 * C L A N G	B A S E D	C L O S U R E S
 *=======================================*/
# include <Block.h>

struct Block_layout {
    void *isa;
    int flags;
    int reserved;
    void (*invoke)();//TODO: check if void* as first arg is a better option
    struct Block_descriptor *descriptor;
};

/*=======================================
 * C L A S S	S Y S T E M
 *=======================================*/
// todo : registers classes in an a tree

// TODO : Do memcopy during super call. Handle cases like useless alloc for parents, and other class construct in constructors
# define class(...) _class(__VA_ARGS__)
# define super(...)\
	(parent = __parent_constructor(__VA_ARGS__))
# define class_prototype(class_name, parent_class_name, properties, ...) \
    typedef struct class_name class_name; \
	struct  class_name {\
			IF_ARGS((parent_class_name), parent_class_name;)\
			struct {\
				properties\
			}; \
			FOR_EACH(METHOD_PROTO, __VA_ARGS__) \
	};

# define extract_meta_propriety(prop)\
	EXPAND(extract_meta_propriety_ prop)

# define extract_meta_propriety_(type, var)\
	type var;

# define print_meta_propriety(self, prop)\
	EXPAND(CALL(print_meta_propriety_, self, UNPACK(EXPAND(UNPACK prop))))

# define print_meta_propriety_(self, type, var)\
	cb(STR(type), &(self -> var));

# define class_meta_prototype(...) _class_meta_prototype(__VA_ARGS__)
# define _class_meta_prototype(class_name, parent_class_name, properties, ...) \
    typedef struct class_name class_name; \
	struct  class_name {\
			IF_ARGS((parent_class_name), parent_class_name;)\
			struct {\
				FOR_EACH(extract_meta_propriety, UNPACK properties) \
			}; \
			FOR_EACH(METHOD_PROTO, __VA_ARGS__) \
	};\
	auto class_name ## _print = lambda(void, (class_name *this, lambda_ptr(void,  cb, (char *type, void *data))),\
		FOR_EACH2(print_meta_propriety, this, UNPACK properties) \
	);

# define class_definition(...) \
	_class_definition(__VA_ARGS__)
# define _class_definition(class_name, parent_class_name, constructor, ...) \
    __auto_type class_name##_construct = ^ class_name * CALL(FIRST_ARG, UNPACK constructor) {\
		__block class_name * this = GC_MALLOC(sizeof(class_name));\
		IF_ARGS((parent_class_name), \
			typeof(parent_class_name ## _construct)  __parent_constructor = parent_class_name ## _construct;\
			parent_class_name *parent;\
		);\
		Tree t;\
		FOR_EACH(METHOD_SET, __VA_ARGS__) \
	   	CALL(REST_ARGS, UNPACK constructor)\
	   	IF_ARGS((parent_class_name), \
	   		printf("parent copied from %s (%p) to %s (%p)\n", STR(parent_class_name), this, STR(class_name), parent);\
			memcpy(this, parent, sizeof(parent_class_name));\
		);\
		printf("[this is %p]\n", this);\
		function(void, defaultDestructor, (void *obj, class_name *o2, void *data),\
			/*print("Destrouying", o2, "\n");*/\
			/*TODO: find why this dont leaks to fix this destructor with Block_release(obj) for class methods;*/\
		);\
		GC_REGISTER_FINALIZER(this, raw_function(void,defaultDestructor,(void*, void*)),NULL, NULL, NULL);\
	   	return this;\
    };

# define _class(class_name, parent_class_name, properties, constructor, ...) \
	/*class_prototype(class_name, parent_class_name, properties, __VA_ARGS__)*/\
	_class_meta_prototype(class_name, parent_class_name, properties, __VA_ARGS__)\
	_class_definition(class_name, parent_class_name, constructor, __VA_ARGS__)

# define METHOD_PROTO(method_def) \
    EXPAND(METHOD_PROTO_ method_def)
# define METHOD_PROTO_(ret_type, name, args, ...) \
    ret_type (^name) args;
# define METHOD_SET(method_def) \
	EXPAND(CALL(METHOD_SET_, UNPACK(EXPAND(UNPACK method_def))))
# define METHOD_SET_(ret_type, name, args, ...) \
    this->name = Block_copy(^ ret_type args {\
		__VA_ARGS__\
	});

/*=======================================
 * A D D I T I O N A L	  K E Y W O R D S
 *=======================================*/

# define u64 uint64_t
# define u32 uint32_t
# define u16 uint16_t
# define u8 	uint8_t

# define s64 int64_t
# define s32 int32_t
# define s16 int16_t
# define s8 	int8_t



# define print(...) FOR_EACH(_print_generic, __VA_ARGS__)
# define _print_generic(x) printf(_Generic((x), \
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

# define raw_function(ret, name, args)\
	(ret(*)(UNPACK args))(((struct Block_layout *)( void *)name)->invoke)
# define auto __auto_type
# define extends ,
# define inst(X)		X,
# define redecl(X)	typeof(X) X;
# define export(...)\
	(struct {\
		FOR_EACH(redecl, __VA_ARGS__)\
	}) {\
		FOR_EACH(inst, __VA_ARGS__)\
	};
# define export_as(name, ...)\
	struct {\
		FOR_EACH(redecl, __VA_ARGS__)\
	} name = {\
		FOR_EACH(inst, __VA_ARGS__)\
	};\
	(typeof(name)) name;
# define lambda_ptr(ret, name, args)\
	ret (^name) args
# define lambda(type, args, body)\
	^ type args { body }
# define function_prototype(type, name, args)\
type (^name) args;
# define function_definition(type, name, args, body)\
name = ^type args { body };
# define function(type, name, args, body)\
	auto name = ^type args { body };
# define local(name, value)\
	auto name = value;
# define global(name, value)\
	typeof(value) *const name = GC_MALLOC(sizeof(value));\
	if (name == NULL)\
		exit(!!printf("memory full %p.", name));\
	*name = value;
# define constant(name, value)\
	const auto name = value;
# define extends ,

/*=======================================
 *  B I N A R Y
 *=======================================*/

# define SET_BIT(VAR, POS) ((VAR) |= (1ULL << (POS)))
# define CLEAR_BIT(VAR, POS) ((VAR) &= ~(1ULL << (POS)))
# define TOGGLE_BIT(VAR, POS) ((VAR) ^= (1ULL << (POS)))
# define GET_BIT(VAR, POS) (!!((VAR) & (1ULL << (POS))))

/*=======================================
 * 	L O G G I N G
 *=======================================*/


# define PRINT(...) 							\
	((dprintf(STDERR_FILENO, "[%s:%i]\t", __FILE__, __LINE__)\
+ 												\
	dprintf(STDERR_FILENO, __VA_ARGS__)			\
+												\
	dprintf(STDERR_FILENO, "\n")))

# define MPRINT(file, line, ...) 				\
	((int)(dprintf(STDERR_FILENO, "[%s:%i]\t", file, line)\
+ 												\
	dprintf(STDERR_FILENO, __VA_ARGS__)			\
+												\
	dprintf(STDERR_FILENO, "\n")))


void dprint_binary(int fd, void *data, size_t bytes)
{
    unsigned char *byteArray = (unsigned char *)data;
	size_t	i = 0;
	while (i < bytes)
	{
		if (i)
			write(fd, " ", 1);
		u8 y = 0;
		while(y < 8)
		{
            char bit = (GET_BIT(byteArray[i], y)) ? '1' : '0';
			write(fd, &bit, 1);
			y += 1;
        }
		i += 1;
	}
}

void print_binary(void *data, size_t bytes)
{
	dprint_binary(STDERR_FILENO, data, bytes);
}

/*=======================================
 *  M E M O R Y
 *=======================================*/

// TODO: create a class for file with fd beeing a private field. On destroy, add a gc hook to close the file.

# define _malloc GC_MALLOC
# define _realloc GC_REALLOC
# define _open open
# define _strdup _strdup

char	*_strdup(char *str)
{
	char *o = _malloc(strlen(str));
	strcpy(o, str);
	return o;
}


void	*_check_malloc(const char *file, int line, void *ptr, size_t bytes)
{
	if (!ptr)
		exit(!!MPRINT(file, line, "error, unable to allocate %zu bytes.", bytes));
	return ptr;
}

void	*_check_realloc(const char *file, int line, void *old_ptr, void *new_ptr, size_t bytes)
{
	if (!new_ptr)
		exit(!!MPRINT(file, line, "error, unable to reallocate %p to %zu bytes.", old_ptr, bytes));
	return new_ptr;
}

int		_check_open(const char *file, int line, int fd, const char *path, int flag)
{
	if (fd < 0)
	    exit(!!MPRINT(file, line, "error, unable to open file %s with flags %x.", path, flag));
	return fd;
}

void	*_assign(void *ptr, void *data, size_t size_of_data)
{
	memcpy(ptr, data, size_of_data);
	return ptr;
}


# define MALLOC(SIZE) (							\
	_check_malloc(								\
		__FILE__,								\
		__LINE__,								\
		_malloc(SIZE),							\
		SIZE									\
	)											\
)
# define ALLOC(TYPE, ...) (						\
	(TYPE *) _assign(							\
		_check_malloc(							\
			__FILE__,							\
			__LINE__,							\
			_malloc(sizeof(TYPE)),				\
			sizeof(TYPE)						\
		),										\
		&((TYPE){__VA_ARGS__}),					\
		sizeof(TYPE)							\
	)											\
)
# define REALLOC(DATA, SIZE) (					\
	_check_realloc(								\
		__FILE__,								\
		__LINE__,								\
		DATA,									\
		_realloc(DATA, SIZE),					\
		SIZE									\
	)											\
)
# define STRDUP(STR) (							\
	(char*) _check_malloc(						\
		__FILE__,								\
		__LINE__,								\
		_strdup(STR),							\
		strlen(STR)								\
	)											\
)
# define OPEN(PATH, FLAG) (						\
	_check_open(								\
		__FILE__,								\
		__LINE__,								\
		_open(PATH, FLAG),						\
		FLAG									\
	)											\
)

/*=======================================
 * M A C R O	P R I M I T I V E S
 *=======================================*/

# define FIRST_ARG(X, ...) X
# define REST_ARGS(X, ...) __VA_ARGS__
# define STR(X) #X
# define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
# define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
# define CONCATENATE2(arg1, arg2) arg1##arg2
# define COUNT_ARGS_HELPER(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,\
	_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
	_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,\
	_63,_64,N,...) N
# define COUNT_ARGS(...) COUNT_ARGS_HELPER(dummy, ##__VA_ARGS__, 64,63,62,61,60,59,58,57,56,\
	55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,\
	26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
# define CALL(NAME, ...) NAME(__VA_ARGS__)
# define EXPAND(X) X
# define UNPACK(...) __VA_ARGS__
# define IF_ONE_ARG(ARGS, ...)

# define FOR_EACH(what, ...) FOR_EACH_(COUNT_ARGS(__VA_ARGS__), what, __VA_ARGS__)
# define FOR_EACH_(N, what, ...) EXPAND(CONCATENATE(FOR_EACH_, N)(what, __VA_ARGS__))
# define FOR_EACH_0(what, x)
# define FOR_EACH_1(what, x) what(x)
# define FOR_EACH_2(what, x, ...) what(x) EXPAND(FOR_EACH_1(what, __VA_ARGS__))
# define FOR_EACH_3(what, x, ...) what(x) EXPAND(FOR_EACH_2(what, __VA_ARGS__))
# define FOR_EACH_4(what, x, ...) what(x) EXPAND(FOR_EACH_3(what, __VA_ARGS__))
# define FOR_EACH_5(what, x, ...) what(x) EXPAND(FOR_EACH_4(what, __VA_ARGS__))
# define FOR_EACH_6(what, x, ...) what(x) EXPAND(FOR_EACH_5(what, __VA_ARGS__))
# define FOR_EACH_7(what, x, ...) what(x) EXPAND(FOR_EACH_6(what, __VA_ARGS__))
# define FOR_EACH_8(what, x, ...) what(x) EXPAND(FOR_EACH_7(what, __VA_ARGS__))
# define FOR_EACH_9(what, x, ...) what(x) EXPAND(FOR_EACH_8(what, __VA_ARGS__))
# define FOR_EACH_10(what, x, ...) what(x) EXPAND(FOR_EACH_9(what, __VA_ARGS__))
# define FOR_EACH_11(what, x, ...) what(x) EXPAND(FOR_EACH_10(what, __VA_ARGS__))
# define FOR_EACH_12(what, x, ...) what(x) EXPAND(FOR_EACH_11(what, __VA_ARGS__))
# define FOR_EACH_13(what, x, ...) what(x) EXPAND(FOR_EACH_12(what, __VA_ARGS__))
# define FOR_EACH_14(what, x, ...) what(x) EXPAND(FOR_EACH_13(what, __VA_ARGS__))
# define FOR_EACH_15(what, x, ...) what(x) EXPAND(FOR_EACH_14(what, __VA_ARGS__))
# define FOR_EACH_16(what, x, ...) what(x) EXPAND(FOR_EACH_15(what, __VA_ARGS__))
# define FOR_EACH_17(what, x, ...) what(x) EXPAND(FOR_EACH_16(what, __VA_ARGS__))
# define FOR_EACH_18(what, x, ...) what(x) EXPAND(FOR_EACH_17(what, __VA_ARGS__))
# define FOR_EACH_19(what, x, ...) what(x) EXPAND(FOR_EACH_18(what, __VA_ARGS__))
# define FOR_EACH_20(what, x, ...) what(x) EXPAND(FOR_EACH_19(what, __VA_ARGS__))
# define FOR_EACH_21(what, x, ...) what(x) EXPAND(FOR_EACH_20(what, __VA_ARGS__))
# define FOR_EACH_22(what, x, ...) what(x) EXPAND(FOR_EACH_21(what, __VA_ARGS__))
# define FOR_EACH_23(what, x, ...) what(x) EXPAND(FOR_EACH_22(what, __VA_ARGS__))
# define FOR_EACH_24(what, x, ...) what(x) EXPAND(FOR_EACH_23(what, __VA_ARGS__))
# define FOR_EACH_25(what, x, ...) what(x) EXPAND(FOR_EACH_24(what, __VA_ARGS__))
# define FOR_EACH_26(what, x, ...) what(x) EXPAND(FOR_EACH_25(what, __VA_ARGS__))
# define FOR_EACH_27(what, x, ...) what(x) EXPAND(FOR_EACH_26(what, __VA_ARGS__))
# define FOR_EACH_28(what, x, ...) what(x) EXPAND(FOR_EACH_27(what, __VA_ARGS__))
# define FOR_EACH_29(what, x, ...) what(x) EXPAND(FOR_EACH_28(what, __VA_ARGS__))
# define FOR_EACH_30(what, x, ...) what(x) EXPAND(FOR_EACH_29(what, __VA_ARGS__))
# define FOR_EACH_31(what, x, ...) what(x) EXPAND(FOR_EACH_30(what, __VA_ARGS__))
# define FOR_EACH_32(what, x, ...) what(x) EXPAND(FOR_EACH_31(what, __VA_ARGS__))
# define FOR_EACH_33(what, x, ...) what(x) EXPAND(FOR_EACH_32(what, __VA_ARGS__))
# define FOR_EACH_34(what, x, ...) what(x) EXPAND(FOR_EACH_33(what, __VA_ARGS__))
# define FOR_EACH_35(what, x, ...) what(x) EXPAND(FOR_EACH_34(what, __VA_ARGS__))
# define FOR_EACH_36(what, x, ...) what(x) EXPAND(FOR_EACH_35(what, __VA_ARGS__))
# define FOR_EACH_37(what, x, ...) what(x) EXPAND(FOR_EACH_36(what, __VA_ARGS__))
# define FOR_EACH_38(what, x, ...) what(x) EXPAND(FOR_EACH_37(what, __VA_ARGS__))
# define FOR_EACH_39(what, x, ...) what(x) EXPAND(FOR_EACH_38(what, __VA_ARGS__))
# define FOR_EACH_40(what, x, ...) what(x) EXPAND(FOR_EACH_39(what, __VA_ARGS__))
# define FOR_EACH_41(what, x, ...) what(x) EXPAND(FOR_EACH_40(what, __VA_ARGS__))
# define FOR_EACH_42(what, x, ...) what(x) EXPAND(FOR_EACH_41(what, __VA_ARGS__))
# define FOR_EACH_43(what, x, ...) what(x) EXPAND(FOR_EACH_42(what, __VA_ARGS__))
# define FOR_EACH_44(what, x, ...) what(x) EXPAND(FOR_EACH_43(what, __VA_ARGS__))
# define FOR_EACH_45(what, x, ...) what(x) EXPAND(FOR_EACH_44(what, __VA_ARGS__))
# define FOR_EACH_46(what, x, ...) what(x) EXPAND(FOR_EACH_45(what, __VA_ARGS__))
# define FOR_EACH_47(what, x, ...) what(x) EXPAND(FOR_EACH_46(what, __VA_ARGS__))
# define FOR_EACH_48(what, x, ...) what(x) EXPAND(FOR_EACH_47(what, __VA_ARGS__))
# define FOR_EACH_49(what, x, ...) what(x) EXPAND(FOR_EACH_48(what, __VA_ARGS__))
# define FOR_EACH_50(what, x, ...) what(x) EXPAND(FOR_EACH_49(what, __VA_ARGS__))
# define FOR_EACH_51(what, x, ...) what(x) EXPAND(FOR_EACH_50(what, __VA_ARGS__))
# define FOR_EACH_52(what, x, ...) what(x) EXPAND(FOR_EACH_51(what, __VA_ARGS__))
# define FOR_EACH_53(what, x, ...) what(x) EXPAND(FOR_EACH_52(what, __VA_ARGS__))
# define FOR_EACH_54(what, x, ...) what(x) EXPAND(FOR_EACH_53(what, __VA_ARGS__))
# define FOR_EACH_55(what, x, ...) what(x) EXPAND(FOR_EACH_54(what, __VA_ARGS__))
# define FOR_EACH_56(what, x, ...) what(x) EXPAND(FOR_EACH_55(what, __VA_ARGS__))
# define FOR_EACH_57(what, x, ...) what(x) EXPAND(FOR_EACH_56(what, __VA_ARGS__))
# define FOR_EACH_58(what, x, ...) what(x) EXPAND(FOR_EACH_57(what, __VA_ARGS__))
# define FOR_EACH_59(what, x, ...) what(x) EXPAND(FOR_EACH_58(what, __VA_ARGS__))
# define FOR_EACH_60(what, x, ...) what(x) EXPAND(FOR_EACH_59(what, __VA_ARGS__))
# define FOR_EACH_61(what, x, ...) what(x) EXPAND(FOR_EACH_60(what, __VA_ARGS__))
# define FOR_EACH_62(what, x, ...) what(x) EXPAND(FOR_EACH_61(what, __VA_ARGS__))
# define FOR_EACH_63(what, x, ...) what(x) EXPAND(FOR_EACH_62(what, __VA_ARGS__))
# define FOR_EACH_64(what, x, ...) what(x) EXPAND(FOR_EACH_63(what, __VA_ARGS__))



# define FOR_EACH2(what, extra_arg, ...) FOR_EACH2_(COUNT_ARGS(__VA_ARGS__), what, extra_arg, __VA_ARGS__)
# define FOR_EACH2_(N, what, extra_arg, ...) EXPAND(CONCATENATE(FOR_EACH2_, N)(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_0(what, extra_arg, x)
# define FOR_EACH2_1(what, extra_arg, x) what(extra_arg, x)
# define FOR_EACH2_2(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_1(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_3(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_2(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_4(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_3(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_5(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_4(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_6(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_5(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_7(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_6(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_8(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_7(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_9(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_8(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_10(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_9(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_11(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_10(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_12(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_11(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_13(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_12(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_14(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_13(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_15(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_14(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_16(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_15(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_17(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_16(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_18(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_17(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_19(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_18(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_20(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_19(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_21(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_20(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_22(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_21(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_23(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_22(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_24(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_23(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_25(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_24(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_26(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_25(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_27(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_26(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_28(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_27(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_29(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_28(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_30(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_29(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_31(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_30(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_32(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_31(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_33(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_32(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_34(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_33(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_35(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_34(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_36(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_35(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_37(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_36(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_38(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_37(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_39(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_38(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_40(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_39(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_41(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_40(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_42(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_41(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_43(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_42(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_44(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_43(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_45(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_44(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_46(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_45(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_47(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_46(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_48(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_47(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_49(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_48(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_50(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_49(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_51(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_50(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_52(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_51(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_53(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_52(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_54(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_53(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_55(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_54(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_56(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_55(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_57(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_56(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_58(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_57(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_59(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_58(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_60(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_59(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_61(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_60(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_62(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_61(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_63(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_62(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_64(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_63(what, extra_arg, __VA_ARGS__))



# define IF_ARGS(ARGS, ...) CALL(CALL(CONCATENATE, IF_ARGS_, COUNT_ARGS ARGS), __VA_ARGS__)
# define IF_ARGS_0(...)
# define IF_ARGS_1(...) __VA_ARGS__
# define IF_ARGS_2(...) __VA_ARGS__
# define IF_ARGS_3(...) __VA_ARGS__
# define IF_ARGS_4(...) __VA_ARGS__
# define IF_ARGS_5(...) __VA_ARGS__
# define IF_ARGS_6(...) __VA_ARGS__
# define IF_ARGS_7(...) __VA_ARGS__
# define IF_ARGS_8(...) __VA_ARGS__
# define IF_ARGS_9(...) __VA_ARGS__
# define IF_ARGS_10(...) __VA_ARGS__
# define IF_ARGS_11(...) __VA_ARGS__
# define IF_ARGS_12(...) __VA_ARGS__
# define IF_ARGS_13(...) __VA_ARGS__
# define IF_ARGS_14(...) __VA_ARGS__
# define IF_ARGS_15(...) __VA_ARGS__
# define IF_ARGS_16(...) __VA_ARGS__
# define IF_ARGS_17(...) __VA_ARGS__
# define IF_ARGS_18(...) __VA_ARGS__
# define IF_ARGS_19(...) __VA_ARGS__
# define IF_ARGS_20(...) __VA_ARGS__
# define IF_ARGS_21(...) __VA_ARGS__
# define IF_ARGS_22(...) __VA_ARGS__
# define IF_ARGS_23(...) __VA_ARGS__
# define IF_ARGS_24(...) __VA_ARGS__
# define IF_ARGS_25(...) __VA_ARGS__
# define IF_ARGS_26(...) __VA_ARGS__
# define IF_ARGS_27(...) __VA_ARGS__
# define IF_ARGS_28(...) __VA_ARGS__
# define IF_ARGS_29(...) __VA_ARGS__
# define IF_ARGS_30(...) __VA_ARGS__
# define IF_ARGS_31(...) __VA_ARGS__
# define IF_ARGS_32(...) __VA_ARGS__
# define IF_ARGS_33(...) __VA_ARGS__
# define IF_ARGS_34(...) __VA_ARGS__
# define IF_ARGS_35(...) __VA_ARGS__
# define IF_ARGS_36(...) __VA_ARGS__
# define IF_ARGS_37(...) __VA_ARGS__
# define IF_ARGS_38(...) __VA_ARGS__
# define IF_ARGS_39(...) __VA_ARGS__
# define IF_ARGS_40(...) __VA_ARGS__
# define IF_ARGS_41(...) __VA_ARGS__
# define IF_ARGS_42(...) __VA_ARGS__
# define IF_ARGS_43(...) __VA_ARGS__
# define IF_ARGS_44(...) __VA_ARGS__
# define IF_ARGS_45(...) __VA_ARGS__
# define IF_ARGS_46(...) __VA_ARGS__
# define IF_ARGS_47(...) __VA_ARGS__
# define IF_ARGS_48(...) __VA_ARGS__
# define IF_ARGS_49(...) __VA_ARGS__
# define IF_ARGS_50(...) __VA_ARGS__
# define IF_ARGS_51(...) __VA_ARGS__
# define IF_ARGS_52(...) __VA_ARGS__
# define IF_ARGS_53(...) __VA_ARGS__
# define IF_ARGS_54(...) __VA_ARGS__
# define IF_ARGS_55(...) __VA_ARGS__
# define IF_ARGS_56(...) __VA_ARGS__
# define IF_ARGS_57(...) __VA_ARGS__
# define IF_ARGS_58(...) __VA_ARGS__
# define IF_ARGS_59(...) __VA_ARGS__
# define IF_ARGS_60(...) __VA_ARGS__
# define IF_ARGS_61(...) __VA_ARGS__
# define IF_ARGS_62(...) __VA_ARGS__
# define IF_ARGS_63(...) __VA_ARGS__
# define IF_ARGS_64(...) __VA_ARGS__


# define IF_NARGS(ARGS, ...) CALL(CALL(CONCATENATE, IF_NARGS_, COUNT_ARGS ARGS), __VA_ARGS__)
# define IF_NARGS_0(...) __VA_ARGS__
# define IF_NARGS_1(...)
# define IF_NARGS_2(...)
# define IF_NARGS_3(...)
# define IF_NARGS_4(...)
# define IF_NARGS_5(...)
# define IF_NARGS_6(...)
# define IF_NARGS_7(...)
# define IF_NARGS_8(...)
# define IF_NARGS_9(...)
# define IF_NARGS_10(...)
# define IF_NARGS_11(...)
# define IF_NARGS_12(...)
# define IF_NARGS_13(...)
# define IF_NARGS_14(...)
# define IF_NARGS_15(...)
# define IF_NARGS_16(...)
# define IF_NARGS_17(...)
# define IF_NARGS_18(...)
# define IF_NARGS_19(...)
# define IF_NARGS_20(...)
# define IF_NARGS_21(...)
# define IF_NARGS_22(...)
# define IF_NARGS_23(...)
# define IF_NARGS_24(...)
# define IF_NARGS_25(...)
# define IF_NARGS_26(...)
# define IF_NARGS_27(...)
# define IF_NARGS_28(...)
# define IF_NARGS_29(...)
# define IF_NARGS_30(...)
# define IF_NARGS_31(...)
# define IF_NARGS_32(...)
# define IF_NARGS_33(...)
# define IF_NARGS_34(...)
# define IF_NARGS_35(...)
# define IF_NARGS_36(...)
# define IF_NARGS_37(...)
# define IF_NARGS_38(...)
# define IF_NARGS_39(...)
# define IF_NARGS_40(...)
# define IF_NARGS_41(...)
# define IF_NARGS_42(...)
# define IF_NARGS_43(...)
# define IF_NARGS_44(...)
# define IF_NARGS_45(...)
# define IF_NARGS_46(...)
# define IF_NARGS_47(...)
# define IF_NARGS_48(...)
# define IF_NARGS_49(...)
# define IF_NARGS_50(...)
# define IF_NARGS_51(...)
# define IF_NARGS_52(...)
# define IF_NARGS_53(...)
# define IF_NARGS_54(...)
# define IF_NARGS_55(...)
# define IF_NARGS_56(...)
# define IF_NARGS_57(...)
# define IF_NARGS_58(...)
# define IF_NARGS_59(...)
# define IF_NARGS_60(...)
# define IF_NARGS_61(...)
# define IF_NARGS_62(...)
# define IF_NARGS_63(...)
# define IF_NARGS_64(...)

#endif
