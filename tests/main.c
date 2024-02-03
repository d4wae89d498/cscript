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





    print("______");

    function(void, String_consumer,(typeof(std.String_construct(0,0)) *str),

    )


    auto montruc = std.Salut_construct(5);



    print(montruc->toto, " une string ", 3.14);

    montruc->make2x();

    print(montruc->toto, " une string ", 3.14);



    print("Testing Any...");
    auto n1 = 4;



    auto any_int = std.make_any(int, n1);



    auto n2 = any_cast(int, any_int);

//    any_cast(time_t, any_int);
    assert(n2 == n1);

}
