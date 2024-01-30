#import "any.h"


// TODO : rewrite this function for a deeper suport of int short, short int etc. combinaisons.
// Idea : sumply remove useless spaces and forbids multi words type (unsigned short etc..)

function_definition(char    *,normalize_type,(char *type),
    char    *o = GC_MALLOC(512);
    size_t  i = 0;
    bool    prev_is_word = false;

    while (*type)
    {
        while (isspace(*type))
            type += 1;
        if (isalpha(*type) || *type == '_')
        {
            if (prev_is_word)
            {
                print("Error: cant normalize type : ", type);
                print("Please provide one word types - u64 instead of unsigend long for example.");
                exit(1);
            }
            while (isalpha(*type) || *type == '_' || isnumber(*type))
            {
                o[i++] = *type++;
            }
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

# define make_any(T, V) Any_construct(STR(T), ALLOC(T, V))
# define any_cast(t, v) ( *(t*)(v->__check_any_cast(STR(t))->data) )
class_definition(Any,,
	((char* type, void *data),
        this->type = normalize_type(type);
        this->data = data;
	),
	(bool, is_type, (char *type),
        return !strcmp(this->type, normalize_type(type));
	),
    (Any*, __check_any_cast, (char *type),
        if (!this->is_type(type))
        {
            print("Error: wrong cast.");
            exit(1);
        }
        return this;
    )
)



