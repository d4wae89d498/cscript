### CScript

This repository contains the initial release of a c superset that is built using C Macros only. It is just an experiments about abusing C PreProcessor.

Current implementation relies on :
- Clang
- BDW GC (ARC ref couting is planned


Usage :

```bash
	brew install bdw clang
	git clone ...
	./main.m

```

.m extension is here for a better IDE compatibility as we are using clang blocks. We instead experimented with GCC nested functions, but found that the flexibility of variable lifetimes was significantly reduced.


## Class syntax

```C
class([ name ] extends [ base ],
		[ type property1 ];
		[ type property2 ]
	(([ constructor args ]),
		[constructor body]
		super([parent args])
	)
	([method1 type], [method1 name], ([method1 args]),
		[method1 body]
	)
	...
)
```

## Function syntax

```C
function([function type], [function name], ([function args]),
		[function body]
)
```


Cast to C compatible function pointer : `raw_function(void,[ name ],(void*, void*))`

## Lambda syntax

```C
lambda ([lambda type], ([lambda args]),
		[lambda body]
)
```

## Module system

```C
	// for a lib that export(func1, class1 ...)
	auto std = ({
		#include "path/to/lib"
	});

	std.print();
```

```C
	// for a lib that export_as(std, func1, class1 ...)

	#include "path/to/lib"
	std.print();

	// using namespace

	#define std name
	#include "path/to/lib"

	name.print();
```


## Macro system

// see todo file


## GC custom free hooks

```C
	GC_REGISTER_FINALIZER([ptr], raw_function(void,[destructor name],(void*, void*)),NULL, NULL, NULL);\
```

Standard library documentation :
