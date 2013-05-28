/* convert hex str "0xFF" to decimal 255 */

#include <stdio.h>
#include <stdlib.h>
int main()
{
	int a;
	char pNum[]="0xFF";

	a=strtoul(pNum,0,16);
	printf("%d\n",a);

	return 0;
}
