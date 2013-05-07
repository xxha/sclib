/*
Define some common macro and struture.
*/
#ifndef _MAIN_H
#define _MAIN_H
#define PART_COUNTS	10	
#define V100_PART_COUNTS	10
#define V300_PART_COUNTS	6
#define REDBOOT_FIS_DIRECTORY_ENTRY_SIZE	256
#define MAX_CONFIG_DATA 4096
typedef struct{
	UINT8 	partion_name[32];
	UINT32	start_addr;
	UINT32	end_addr;
	UINT32	len;	
}PARTITION_ITEM;
// Internal structure used to hold config data
struct _config {
    unsigned long len;
    unsigned long key1;
    unsigned char config_data[MAX_CONFIG_DATA-(4*4)];
    unsigned long key2;
    unsigned long cksum;
};
#endif

