#include <ctype.h>

#include "echar/echar_delta.h"

struct echar_delta 
make_echar_delta_ascii( unsigned char ascii ){
    char enc;
    if( 'a' <= ascii && ascii <= 'z') enc = ascii - 'a';
    else if( 'A' <= ascii && ascii <= 'Z') enc = ascii - 'A';
    else enc = 26;
    return (struct echar_delta){ .delta = enc };
}

char echar_delta_to_ALPHA( struct echar_delta d ){
    return 'A' + d.delta;
}
