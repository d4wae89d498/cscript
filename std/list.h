

class_meta_prototype(List,,
	(
		(Any, 				data),
		(struct List *, 	prev),
		(struct List *, 	next)
	),
	(List *, 	first, 			()),
	(List *, 	last, 			()),
	(List *, 	add_back, 		(Any)),
	(List *, 	add_front, 		(Any)),
	(void, 		link_back, 		(List *)),
	(void, 		link_front, 	(List *)),
	(List *, 	get_by_index, 	(u64)),
	(void,		del_by_ptr, 	(List *)),
	(List *,	clone, 			()),
	(void,		free, 			())
);
