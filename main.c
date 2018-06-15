#include <stdio.h>
#include <stdlib.h>
#include "analizador.h"

int main()
{//fdisk %Allocation->E
    char str[250];
    memset(&str,'\0',sizeof(str));
    gets( str );
    load();
    while(strcasecmp(str,"EXIT") != 0){
    analyzer(str);
    memset(&str,'\0',sizeof(str));
    gets( str );
    }
    exit(0);
    return 0;
}
