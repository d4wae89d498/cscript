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
        #import "../std/_"
    });

    GC_INIT();
    
    auto test_raw1 = "hello1";
    auto str = std.String_construct(test_raw1);
    assert(str->len == strlen(test_raw1));
    assert(str->is_raw(test_raw1));
    
    print("tests passed for ", test_raw1);
   
    print("______");
    print("Dumping properties of a String: ")
    // _print => meta ?
    std.String_meta(str, lambda(void, (char *type, char *name, void *data),
			print ("type: ", type, "\t name: ", name, "\t data: ", data);
	));
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
