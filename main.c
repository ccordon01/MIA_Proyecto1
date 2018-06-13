#include <stdio.h>
#include <stdlib.h>
#include "analizador.h"

int main()
{
    char str[100];
    memset(str,'\0',sizeof(str));
    gets( str );
    load();
    while(strcasecmp(str,"EXIT") != 0){
    analyzer(str);
    memset(str,'\0',sizeof(str));
    gets( str );
    }
    return 0;
}
