#include <ctype.h>

#include "echar/echar_delta.h"
#include "charmap.h"

struct echar_delta 
make_echar_delta_ascii( unsigned char ascii ){
    return (struct echar_delta){ .delta = code[ascii] };
}

char echar_delta_to_ALPHA( struct echar_delta d ){
    return 'A' + d.delta;
}
