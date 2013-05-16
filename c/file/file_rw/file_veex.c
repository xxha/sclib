#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define IGMCONFIG_FILE_PATH	 "/usr/app/igmcfg.ini"
#define IGM_MAX_PORT_COUNT		  (10)

typedef struct st_IGMInfo
{
	/* IGM name */
	char Name[100];

	/* Mac address */
	unsigned char MAC[6];

	/* IGM location */
	char Location[255];

	int  nIGMNumber;

} st_IGMInfo;

void* veex_memset( void* s, int c, size_t n )
{
  int i = 0;
  unsigned char* temp = (unsigned char* )s;

  for( i = 0; i < n; i++ )
	*(temp++) = (c & 0xff);

  return s;
}


int main()
{

	st_IGMInfo igminfo1 = {"hello", "who", "are you?", 8080};
	st_IGMInfo igminfo2;
	int hTargetFile = -1;
	char *name = "hello";
	char *mac = "who";
	char *location = "are you?";

	strcpy(igminfo2.Name, name);
	strcpy(igminfo2.MAC, mac);
	strcpy(igminfo2.Location, location);
	igminfo2.nIGMNumber = 8080;

	printf("igminfo2.Name = %s\n", igminfo2.Name);
	printf("igminfo2.MAC = %s\n", igminfo2.MAC);
	printf("igminfo2.Location = %s\n", igminfo2.Location);
	printf("igminfo2.nIGMNumber = %d\n", igminfo2.nIGMNumber);


	veex_memset( &igminfo2, 0, sizeof(st_IGMInfo) );

	/* write */
	if( (hTargetFile = open(IGMCONFIG_FILE_PATH, O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO)) < 0 )
	{
		printf( "Open config file 2 %s failed.\n", IGMCONFIG_FILE_PATH);
	}
	else
	{
		lseek( hTargetFile, 0, SEEK_SET );
		write( hTargetFile, &igminfo1, sizeof(st_IGMInfo) );
	}

	if( hTargetFile >= 0 )
	{
		close( hTargetFile );
		hTargetFile = -1;
	}

	/* read */
	if( (hTargetFile = open( IGMCONFIG_FILE_PATH, O_RDWR )) < 0 )
	{
		printf( "Open config file 1 %s failed.\n", IGMCONFIG_FILE_PATH);
	}
	else
	{
		read( hTargetFile, &igminfo2, sizeof(st_IGMInfo) );
		close( hTargetFile );
		printf("igminfo2.Name = %s\n", igminfo2.Name);
		printf("igminfo2.MAC = %s\n", igminfo2.MAC);
		printf("igminfo2.Location = %s\n", igminfo2.Location);
		printf("igminfo2.nIGMNumber = %d\n", igminfo2.nIGMNumber);
	}

}
