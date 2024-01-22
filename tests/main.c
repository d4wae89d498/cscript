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
    
    auto test_raw1 = "hello1";
    auto str = std.String_construct_raw(test_raw1);
    assert(str->len == strlen(test_raw1));
    assert(str->is_raw(test_raw1));
    
    print("tests passed for ", test_raw1);
   
    print("______");
    print("Dumping properties of a String: ")
    // _print => meta ?
    std.String_print(str, lambda(void, (char *type, char *name, void *data),
                                    print ("type: ", type, "\t name: ", name, "\t data: ", data);
                                 ));
    print("______");
    
    function(void, String_consumer,(typeof(std.String_construct(0,0)) *str),
             
    )
    
    auto list = std.List_construct();
}
