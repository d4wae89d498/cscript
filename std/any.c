#import "any.h"

class_definition(Any,,
	((char* type, void *data),
		this->type = type;
		this->data = data;
	),
	(bool, is_type, (char *type),
		return !strcmp(this->type, type);
	)
)

# define any_cast(t, v) (*(t*)__check_any_cast(v, normalize_type(STR(t))).data)
function(Any*, __check_any_cast, (Any *v, char *type),
	if (!v->is_type(type))
	{
		print("Error: wrong cast.");
		exit(1);
	}
	return v;
)

// TODO : rewrite this function for a deeper suport of int short, short int etc. combinaisons.
// Idea : sumply remove useless spaces and forbids multi words type (unsigned short etc..)

function_definition(char	*,normalize_type,(char *type),

	char	*o = GC_MALLOC(512);
	size_t	i = 0;
	bool 	prev_is_word = false;

	while (*type)
	{
		while (isspace(*type))
			type += 1;
		if (isalpha(*type) || *type == '_')
		{
			if (prev_is_word)
					o[i++] = ' ';
			while (isalnum(*type)  || *type == '_')
				o[i++] = *type++;
			prev_is_word = true;
		}
		while (isspace(*type))
			type += 1;
		if (! (isalpha(*type) || *type == '_'))
		{
			while ((!isspace(*type)) && *type )
				o[i++] = *type++;
			prev_is_word = false;
		}
	}
	o[i] = 0;
	return o;
)

