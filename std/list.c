

class_definition(List,,
	(							(),
		this->next = NULL;
		this->prev = NULL;
    ),

	(List *, 	first, 			(),
        List *it = this;
     
        while (it)
        {
            if (!it->prev)
                return it;
            it = it->prev;
        }
        return NULL;
    ),
        
	(List *, 	last, 			(),
		List *it = this;
		return NULL;
    ),
    
	(List *, 	add_back, 		(Any a),
		return NULL;
	),
	
    (List *, 	add_front, 		(Any a),
		return NULL;
	),
    
    (void, 		link_back, 		(List *lst),
		return;
	),
    
	(void, 		link_front, 	(List *lst),
		return;
	),

	(List *, 	get_by_index, 	(u64 idx),
        List    *it = this;
        u64     i = 0;
     
        while (it && i < idx)
        {
            i += 1;
            if (!it->next)
                return NULL;
            it = it->prev;
        }
        return NULL;
    ),
    
	(void,		del_by_ptr, 	(List *ptr),
        List    *it = this;

        while (it)
        {
            if (it == ptr)
            {
                
            }
        }
        return;
    ),
    
	(List *,	clone, 			(),
        return NULL;
    ),
    
	(void,		free, 			(),
        return ;
    )
);

