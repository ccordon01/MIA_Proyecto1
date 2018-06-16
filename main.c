#include <stdio.h>
#include <stdlib.h>
#include "analizador.h"

int main()
{
    load();
    char str[250];
    memset(&str,'\0',sizeof(str));
    gets( str );
    while(strcasecmp(str,"EXIT") != 0){
    analyzer(str);
    memset(&str,'\0',sizeof(str));
    gets( str );
    }
    exit(0);
    return 0;
}
