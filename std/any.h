class_meta_prototype(Any,, (
		(char *, type),
		(void *, data)
	),
	(bool, is_type, (char *type)),
    (Any*, __check_any_cast, (char *type))

);
function_prototype(char	*,normalize_type,(char *type));
