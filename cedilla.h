#ifndef CEDILLA_H
# define CEDILLA_H
# include <gc.h>
# include <Block.h>

struct Block_layout {
    void *isa;
    int flags;
    int reserved;
    void (*invoke)();//TODO: check if void* as first arg is a better option
    struct Block_descriptor *descriptor;
};

/*=======================================
 * M A C R O	P R I M I T I V E S
 *=======================================*/

# define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
# define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
# define CONCATENATE2(arg1, arg2) arg1##arg2

# define COUNT_ARGS_HELPER(_0, _1, _2, _3, _4, _5, N, ...) N
# define COUNT_ARGS(...) COUNT_ARGS_HELPER(dummy, ##__VA_ARGS__, 5, 4, 3, 2, 1, 0)
# define CALL(NAME, ...) NAME(__VA_ARGS__)
# define EXPAND(X) X
# define UNPACK(...) __VA_ARGS__
# define IF_ONE_ARG(ARGS, ...)
# define FOR_EACH_0(what, x)
# define FOR_EACH_1(what, x) what(x)
# define FOR_EACH_2(what, x, ...) what(x) EXPAND(FOR_EACH_1(what, __VA_ARGS__))
# define FOR_EACH_3(what, x, ...) EXPAND(what(x)) EXPAND(FOR_EACH_2(what, __VA_ARGS__))
# define FOR_EACH_4(what, x, ...) what(x) EXPAND(FOR_EACH_3(what, __VA_ARGS__))
# define FOR_EACH_5(what, x, ...) what(x) EXPAND(FOR_EACH_4(what, __VA_ARGS__))
# define FOR_EACH_(N, what, ...) EXPAND(CONCATENATE(FOR_EACH_, N)(what, __VA_ARGS__))
# define FOR_EACH(what, ...) FOR_EACH_(COUNT_ARGS(__VA_ARGS__), what, __VA_ARGS__)

# define FOR_EACH2_1(what, extra_arg, x) what(extra_arg, x)
# define FOR_EACH2_2(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_1(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_3(what, extra_arg, x, ...) EXPAND(what(extra_arg, x)) EXPAND(FOR_EACH2_2(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_4(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_3(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_5(what, extra_arg, x, ...) what(extra_arg, x) EXPAND(FOR_EACH2_4(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2_(N, what, extra_arg, ...) EXPAND(CONCATENATE(FOR_EACH2_, N)(what, extra_arg, __VA_ARGS__))
# define FOR_EACH2(what, extra_arg, ...) FOR_EACH2_(COUNT_ARGS(__VA_ARGS__), what, extra_arg, __VA_ARGS__)

# define IF_ARGS_1(...) __VA_ARGS__
# define IF_ARGS_0(...)
# define IF_ARGS(ARGS, ...) CALL(CALL(CONCATENATE, IF_ARGS_, COUNT_ARGS ARGS), __VA_ARGS__)

# define FIRST_ARG(X, ...) X
# define REST_ARGS(X, ...) __VA_ARGS__

# define STR(X) #X

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

# define class_meta_prototype(class_name, parent_class_name, properties, ...) \
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


# define class_definition(class_name, parent_class_name, constructor, ...) \
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
	class_meta_prototype(class_name, parent_class_name, properties, __VA_ARGS__)\
	class_definition(class_name, parent_class_name, constructor, __VA_ARGS__)

# define METHOD_PROTO(method_def) \
    EXPAND(METHOD_PROTO_ method_def)
# define METHOD_PROTO_(ret_type, name, args, ...) \
    typeof(^ ret_type args { (void)4;  }) name;
# define METHOD_SET(method_def) \
	EXPAND(CALL(METHOD_SET_, UNPACK(EXPAND(UNPACK method_def))))
# define METHOD_SET_(ret_type, name, args, ...) \
    this->name = Block_copy(^ ret_type args {\
		__VA_ARGS__\
	});

/*=======================================
 * A D D I T I O N A L	  K E Y W O R D S
 *=======================================*/

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

#endif
