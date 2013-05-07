#include <stdio.h>
#include <stdlib.h>


char name[] = "test";
char buffer[64];
int main()
{
        snprintf(buffer,sizeof(buffer),"rm -rf %s", name);
        system(buffer);
        return 0;
}
