class_meta_prototype(Tree,, (
		(int, i)
	),
	(void, foo, ()),
	(void, lol, ())
);

class_definition(Tree,,
	((),
		this->i = 42;
	),
	(void, foo, ()

	),
	(void, lol, ()

	)
);


auto t = Tree_construct();



Tree_print(t, print_proprety);
