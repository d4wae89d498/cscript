#!./scripts/crunbang -g -fms-extensions -Wno-microsoft-anon-tag
#! 	-lgc -include gc.h -include cedilla.h -include assert.h -include main.c
#! 	-DON_STARTUP=GC_INIT();

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

/*
#import "cedc/parser.m"



auto ast = Ast_construct();
ast->on(lambda((),

	cursor =
	this->match_node(lambda((ast_node *node),
		return node->type->is_raw("OPENING_PARENTHESE");
	))
	this->match_nodes_until(lambda((),

	))


	cursor = match_ast(
		match_node(lambda ( ... return node->type == OPENING_PARENTHESES ));
		match_nodes_until(lambda( ( ... return tree->last->type == CLOSING_PARENTHESIS)));
	);
	if (!cursor.success)
		return NEXT_LISTENER;
	// Here we loop back until the opening one
	// Then we alter the tree to add a new node..
	return RESTART_PREPROCESSING_AT_CURSORR;
))

auto parser = Parser_construct("test.cedilla", ast);
parser->on(lambda((char *str),
	if (*str == '\'')
		return ast_list .... type=quote
));



parser->run();*/
