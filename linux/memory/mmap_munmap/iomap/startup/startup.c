#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "iomap.h"
#include "lld.h"
#include "../cx180/include/igmconfig.h"

//#define IGMCONFIG_FILE_PATH     "/usr/app/igmcfg.ini"
//#define IPCONFIG_SH_PATH        "/usr/app/scripts/ipconfig.sh"


int main( int argc, char* argv[] )
{
    char buf[32];
    char cmd1[64];
    char cmd2[64];

    st_IGMConfig IGMConfig;

    // Read flash to get the device information!
    char* pModuleCtrl = (char* )GetVirtualMemory( (void* )FLASH_START_ADDR, NULL );
    st_IGMFlashOption* pFlashOpt = (st_IGMFlashOption* )pModuleCtrl;
    memcpy( &IGMConfig, pFlashOpt->ucIGMConfig, sizeof(st_IGMConfig) );

    strcpy( cmd1, "ifconfig eth1 " );
    sprintf( buf, "%d.%d.%d.%d", IGMConfig.ucArrLocalIP[0], IGMConfig.ucArrLocalIP[1], 
        IGMConfig.ucArrLocalIP[2], IGMConfig.ucArrLocalIP[3] );
    strcat( cmd1, buf );
    strcat( cmd1, " netmask " );
    sprintf( buf, "%d.%d.%d.%d\n", IGMConfig.ucArrNetmask[0], IGMConfig.ucArrNetmask[1], 
        IGMConfig.ucArrNetmask[2], IGMConfig.ucArrNetmask[3] );
    strcat( cmd1, buf );

    strcpy( cmd2, "route add default gw " );
    sprintf( buf, "%d.%d.%d.%d", IGMConfig.ucArrGateway[0], IGMConfig.ucArrGateway[1], 
        IGMConfig.ucArrGateway[2], IGMConfig.ucArrGateway[3] );
    strcat( cmd2, buf );
    strcat( cmd2, " eth1\n" );

    system( cmd1 );
    usleep( 500000 );
    system( cmd2 );

    usleep( 500000 );
    system( "cp /usr/app/version /root/softver.txt\n" );

    usleep( 500000 );
    remove( "/usr/app/upgradeover.bmp" );
    remove( "/usr/app/upgrading.bmp" );

    return 0;
}



