//
//  main.c
//  cedilla
//
//  Created by Marc on 22/01/2024.
//

#include "../cedilla.h"

int main()
{


    auto std = ({
        #include "../std/_"
    });

    GC_INIT();


	print("parent of Tree is : ", std.Tree_cmap.parent->name);


    print("______");

    function(void, String_consumer,(typeof(std.String_construct(0,0)) *str),

    )


    auto montruc = std.Salut_construct(5);

	std.Salut_meta(montruc, lambda(void, (const char*type, const char*name, void *d),
		//printf("okok");
	print("--name: ", name);
	print("--type: ", type);
	));

	printf("this was %p\n", montruc);
    montruc->make2x();
    printf("%i\n", montruc->toto);
    printf("montruc was.. %p\n", montruc);
    montruc->make2x();
    printf("%i\n", montruc->toto);
    montruc->make2x();
    printf("%i\n", montruc->toto);
    montruc->make2x();
    printf("%i\n", montruc->toto);
    montruc->make2x();
    printf("%i\n", montruc->toto);
    montruc->make2x();
    printf("%i\n", montruc->toto);
    montruc->make2x();
    printf("%i\n", montruc->toto);
    montruc->make2x();
    printf("%i\n", montruc->toto);

//	exit(0);

   auto t = std.Tree_construct();

    montruc->make2x();
    printf("%i\n", montruc->toto);
//   print("succes ? ", montruc);
 //  printf("--%p\n", montruc); 
//    montruc->meta(lambda(void, (const char *name, const char *type, void *data),
//	print("--name: ", name);
//	print("--type: ", type);
 //  ));

/*
    print(montruc->toto, " une string ", 3.14);

    montruc->make2x();

    print(montruc->toto, " une string ", 3.14);



    print("Testing Any...");
    auto n1 = 4;



    auto any_int = std.make_any(int, n1);



    auto n2 = any_cast(int, any_int);

//    any_cast(time_t, any_int);
    assert(n2 == n1);
*/
}
