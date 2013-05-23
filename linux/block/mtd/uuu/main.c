
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "new_type.h"
#include "main.h"
#include "mtd-self.h"
#include "version.h"
//global varibles.
PARTITION_ITEM	partion_table[PART_COUNTS];
INT32	partition_num;
UINT8 	partion_buff[512];
#define BLOCK_SIZES_FOR_WRITE 20480
static unsigned char tempBuf[BLOCK_SIZES_FOR_WRITE];
static unsigned char data[BLOCK_SIZES_FOR_WRITE];
static UINT8 md5sum1[64],md5sum2[64];
static int update(char *fileName);


//function declaration
int detect_partition();
int write_image_to_partition(UINT8 *image,UINT8 *partition);
int read_image_from_partition(UINT8 *image,UINT8 *partition);
int display_redboot_config();
int update_usr_v300(int index,UINT8 *image);
/*
 This utility is used to upgrade the update user partion,for manufacture only.
 */
 void usage(unsigned char *str){
	printf("Please input the parameter .\n");
		printf("Usage:\n");
		printf("\t %s -l :list the partion table of the unit.\n",str);
		printf("\t %s -r partion-name dest-file:Snatch the image from the unit.\n",str);
		printf("\t %s -w image-name partion-name:write the image context into the declared partition.\n",str);
		//printf("\t %s -d :display the environment of redboot.\n ",str);
		//printf("\t %s -c config-file :configure the environment for redboot.\n ",str);
		//printf("\t %s -f partion-table-file:fix the partion table for redboot.\n",str);
		printf("Vesion %s compiled by %s on %s system at %s.\n",UTILS_VERSION,UTILS_AUTHOR,UTILS_HOST_OS,UTILS_TIME);
		
		printf("Copyright veex (c) 2008-2010.\n");		
 }


//Return value :0 ,success;-1 illegal parameter .-2 the other error.
 int main(int argc,char **argv){
 	
	int ret,i;
	UINT8 partition_name[32],image_name[64];

	if(1==argc){
		usage(argv[0]);		
		return -1;
	}
	if(2==argc){
		if(strcmp("-l",argv[1])==0){
			ret=detect_partition();
			if(0!=ret){
				printf("Failed to detect partition infomation.\n");
				return -1;
			}
			printf("----------------------------------------------------------------------\n");
			printf("Part Name\t\t\tStart Addr\t\tLength\n");
			printf("----------------------------------------------------------------------\n");
			for(i=0;i<partition_num;i++){
				printf("%-16s\t\t0x%08X\t\t0x%08X\n",partion_table[i].partion_name,partion_table[i].start_addr,partion_table[i].len);
			}
			printf("----------------------------------------------------------------------\n");
			return 0;
		}
#if 0
		if(strcmp("-d",argv[1])==0){
			ret=display_redboot_config();
			return ret;
		}
#endif 

		usage(argv[0]);
		return -1;
	}
	if(4==argc){
		if(strcmp("-w",argv[1])==0){//write image into partion.
			strcpy(image_name,argv[2]);
			strcpy(partition_name,argv[3]);
			ret=write_image_to_partition(image_name,partition_name);
			return ret;
 		}
		if(strcmp("-r",argv[1])==0){			
			strcpy(partition_name,argv[2]);
			strcpy(image_name,argv[3]);
			ret=read_image_from_partition(image_name,partition_name);
			return ret;

		}
	}
	usage(argv[0]);
	return -1;
#if 0	
	//analyse the input parameter.
	if(argc==1){
		usage(argv[0]);
	}
	

 	//check the input parameter.
 	if(argc!=2){
		usage();
		return -1; // illegal input parameter.
	}
	strcpy(imageName,argv[1]);
	//update the partion 
	ret=update(imageName);
	if(ret!=0){
		printf("Error:failed to update %s\n",imageName);
		return -2;
	}
	printf("Sir ,%s is upgraded!\n",imageName);	
#endif
	return 0;
 }

/*
Function:detect  the partion information 
Return Value:
	-1:Faile.
	0:Success.
*/
int detect_partition(){
	FILE *file_pipe,*file;
	UINT8 string[32];	
	int i,j,pos,len;
	PARTITION_ITEM temp;

	
	//get the number of partition 
	file_pipe=popen("ls -l /dev/mtd? |grep \"mtd\" -c","r");
	if(NULL==file_pipe){
		printf("Failed to retrive the partition information.\n");
		return -1;
	}
	fgets(string,sizeof(string),file_pipe);
	partition_num=atoi(string);
	if((partition_num<0)||(partition_num>PART_COUNTS)){
		printf("The unit partion table is bad.\n");
		return -1;
	}
	pclose(file_pipe);
	//detect the detail partiton infomation.
	memset(string,0x00,sizeof(string));
	sprintf(string,"/dev/mtd%d",partition_num-2);
	//printf("%s\n",string);
	file=fopen(string,"r");
	if(NULL==file){
		printf("Failed to open FIS directory.\n");
		return -1;
	}
	
	for(i=0;i<partition_num;i++){
		len=fread(partion_buff,1,REDBOOT_FIS_DIRECTORY_ENTRY_SIZE,file);
		if(len!=REDBOOT_FIS_DIRECTORY_ENTRY_SIZE){
			printf("Failed to read single partion information\n");
			return -1;
		}
		pos=0;
		strcpy(partion_table[i].partion_name,partion_buff);
		pos+=16;
		partion_table[i].start_addr=0x00;
		for(j=0;j<4;j++){
			partion_table[i].start_addr=(partion_table[i].start_addr<<0x08);
			partion_table[i].start_addr+=partion_buff[pos+3-j];
		}
		pos+=8;
		partion_table[i].len=0x00;
		for(j=0;j<4;j++){
			partion_table[i].len=(partion_table[i].len<<0x08);
			partion_table[i].len+=partion_buff[pos+3-j];
		}
		//patch for the bad mtdblock3 not register in redboot.
		if(isalpha(partion_table[i].partion_name[0])==0){
			strcpy(partion_table[i].partion_name,"Usr");
			partion_table[i].len=0x39A0000;
			partion_table[i].start_addr=0xC0640000;
		}	
		//printf("Partion name:%s \t\tflash start address:%08x \t\tlength:%08x\n",partion_table[i].partion_name,partion_table[i].start_addr,partion_table[i].len);					
		//printf("Partion name:%s ",partion_table[i].partion_name);					
	}
	//sort the list by the address.
	for(i=partition_num;i>1;i--){
		for(j=1;j<i;j++){
			if(partion_table[j-1].start_addr>partion_table[j].start_addr){
				memcpy((void *)&temp,(void *)&partion_table[j-1],sizeof(PARTITION_ITEM));
				memcpy((void *)&partion_table[j-1],(void *)&partion_table[j],sizeof(PARTITION_ITEM));
				memcpy((void *)&partion_table[j],(void *)&temp,sizeof(PARTITION_ITEM));
			}
		}
	}
	fclose(file);
	return 0;	
}
/*
Function: write image context into the flash partion.
Return Value:
	0:Success.
	-1:Error.
*/
int write_image_to_partition(UINT8 *image,UINT8 * partition){
#if 1
	int i,j,index_part,ret,counts;
	INT8 option;
	UINT8 str[32],*ptr;
	FILE *dst_file,*src_file,*tst_file,*pipe_file;
	ERASE_INFO_USER eraseInfo;

	int block=BLOCK_SIZES_FOR_WRITE;
	long totalLen,backLen,len;

	ret=detect_partition();
	if(0!=ret){
		printf("Failed to detect partition infomation.\n");
		return -1;
	}
	//check the partion name.
	index_part=-1;
	for(i=0;i<partition_num;i++){
		if(strcmp(partition,partion_table[i].partion_name)==0){
			index_part=i;
			break;
		}
	}
	if(-1==index_part){
		printf("index_part:%d\n",index_part);
		printf("The partition \"%s\" is not existent.please check your input.\n",partition);
		return -1;
	}
	printf("This will overwrite \"%s\" partion .This may destroy your unit .Are you sure?[y/n]:",partition);
	scanf("%c",&option);
	if('y'!=option){
		printf("Acting only after careful consideration.:)\n");
		return -1;
	}
	//add patch for special mtdblock3 of v300
	if(strcmp(partition,"Usr")==0){
		return update_usr_v300(index_part,image);
	}
	//ok, now update the partion.
	src_file=fopen(image,"r");
	if(NULL==src_file){
		printf("Failed to open the image file \"%s\".\n",image);
		return -1;
	}
	memset(str,0x00,sizeof(str));
	sprintf(str,"/dev/mtd%d",index_part);
	dst_file=fopen(str,"r+");
	if(NULL==dst_file){
		printf("Failed to open the device file \"%s\".\n",str);
		return -1;
	}

	//erase the device partion before write.
	eraseInfo.start=0;
	eraseInfo.length=partion_table[i].len;
	ret=ioctl(fileno(dst_file),MEMERASE,&eraseInfo);
	if(0!=ret){
		printf("Failed to erase memory.\n");
		return -1;
	}

	fseek(src_file,0,SEEK_END);
	totalLen=ftell(src_file);
	fseek(src_file,0,SEEK_SET);
	backLen=totalLen;
	counts=(totalLen+block-1)/block;
	i=0;
	while(totalLen>0){
		if(totalLen>block){
			len=block;
			totalLen-=block;
		}
		else{	
			len=totalLen;
			totalLen=0;
		}		
		ret=fread(tempBuf,1,len,src_file);
		if(ret!=len){
			printf("Error: fail to read data from source file \"%s\".\n",image);
			goto EXIT;
		}
			
		ret=fwrite(tempBuf,1,len,dst_file);
		if(ret!=len){
			printf("Error:failed to write dev file \"%s\".\n",str);
			goto EXIT;
		}				
	}

	//check the integrity of partition file.
	tst_file=fopen("tmp","w");
	if(NULL==tst_file){
		printf("Failed to create temp file for test image \"%\"",image);
		return -1;
	}
	
	totalLen=backLen;
	fflush(dst_file);
	fseek(dst_file,0,SEEK_SET);
	while(totalLen>0){
		if(totalLen>block){
			len=block;
			totalLen-=block;
		}
		else{	
			len=totalLen;
			totalLen=0;
		}	
		//printf("%d ",totalLen);
		ret=fread(tempBuf,1,len,dst_file);
		if(ret!=len){
			printf("Error: fail to read data from source file \"%s\".\n",image);
			goto EXIT;
		}
			
		ret=fwrite(tempBuf,1,len,tst_file);
		if(ret!=len){
			printf("Error:failed to write to file tmp\n");
			goto EXIT;
		}						
	}
	if(NULL!=tst_file){
		fclose(tst_file);
		tst_file=NULL;
	}
	//get the number of partition 
	memset(str,0x00,sizeof(str));
	sprintf(str,"md5sum %s",image);
	pipe_file=popen(str,"r");
	if(NULL==pipe_file){
		printf("Failed to retrive the partition information.\n");
		return -1;
	}
	fgets(md5sum1,sizeof(md5sum1),pipe_file);
	pclose(pipe_file);
	memset(str,0x00,sizeof(str));
	sprintf(str,"md5sum tmp");
	pipe_file=popen(str,"r");
	if(NULL==pipe_file){
		printf("Failed to retrive the partition information.\n");
		return -1;
	}
	fgets(md5sum2,sizeof(md5sum2),pipe_file);
	pclose(pipe_file);

	ptr=index(md5sum1,' ');
	i=ptr-md5sum1;

	md5sum1[i]=0x00;
	md5sum2[i]=0x00;
	//printf("%s \n%s\n",md5sum1,md5sum2);
	if(strcmp(md5sum1,md5sum2)!=0){
		printf("The context in \"%s\" partition is not same with \"%s\".Please try again.\n",partition,image);
		return -1;
	}


SUCC:
	fclose(src_file);
	fclose(dst_file);
	if(NULL!=tst_file){
		fclose(tst_file);
		tst_file=NULL;
	}
	printf("Congratulation,the \"%s\" image has been wroten into \"%s\" partition successfully.\n",image,partition);
	return 0;	
EXIT:	
	fclose(src_file);
	fclose(dst_file);
	if(NULL!=tst_file){
		fclose(tst_file);
		tst_file=NULL;
	}
#endif

	return -1;
}
/*
Function:
	snatch the image from pation into a file .
Return value:
	0:Success.
	-1:Error.
*/
int read_image_from_partition(UINT8 *image,UINT8 *partition){
	int i,j,index_part,ret,counts;
	INT8 option;
	UINT8 str[32],md5sum1[64],md5sum2[64],*ptr;
	FILE *dst_file,*src_file,*tst_file,*pipe_file;
	ERASE_INFO_USER eraseInfo;

	int block=BLOCK_SIZES_FOR_WRITE;
	long totalLen,backLen,len;

	ret=detect_partition();
	if(0!=ret){
		printf("Failed to detect partition infomation.\n");
		return -1;
	}
	//check the partion name.
	index_part=-1;
	for(i=0;i<partition_num;i++){
		if(strcmp(partition,partion_table[i].partion_name)==0){
			index_part=i;
			break;
		}
	}
	if(-1==index_part){
		printf("index_part:%d\n",index_part);
		printf("The partition \"%s\" is not existent.please check your input.\n",partition);
		return -1;
	}
	printf("Snatch the flash image .This will overwrite \"%s\" image file . Are you sure?[y/n]:",image);
	scanf("%c",&option);
	if('y'!=option){
		printf("Acting only after careful consideration.:)\n");
		return -1;
	}
	//add patch for special mtdblock3 of v300
	if(strcmp(partition,"Usr")==0){
		return snatch_usr_v300(index_part,image);
	}

	//ok, now update the partion.
	dst_file=fopen(image,"w");
	if(NULL==dst_file){
		printf("Failed to open the image file \"%s\".\n",image);
		return -1;
	}
	memset(str,0x00,sizeof(str));
	sprintf(str,"/dev/mtd%d",index_part);
	src_file=fopen(str,"r+");
	if(NULL==src_file){
		printf("Failed to open the device file \"%s\".\n",str);
		return -1;
	}

	totalLen=partion_table[index_part].len;
	backLen=totalLen;
	counts=(totalLen+block-1)/block;
	i=0;
	while(totalLen>0){
		if(totalLen>block){
			len=block;
			totalLen-=block;
		}
		else{	
			len=totalLen;
			totalLen=0;
		}		
		ret=fread(tempBuf,1,len,src_file);
		if(ret!=len){
			printf("Error: fail to read data from source file \"%s\".\n",image);
			goto EXIT;
		}
			
		ret=fwrite(tempBuf,1,len,dst_file);
		if(ret!=len){
			printf("Error:failed to write dev file \"%s\".\n",str);
			goto EXIT;
		}				
	}

SUCC:
	fclose(src_file);
	fclose(dst_file);
	printf("Congratulation,the \"%s\" partition has been wroten into \"%s\" image successfully.\n",partition,image);
	return 0;	
EXIT:	
	fclose(src_file);
	fclose(dst_file);
	printf("Sorry, failed .Try again.\n");
	return -1;

}
/*
Function:Display the environment of redboot.

*/
int display_redboot_config(){
	int ret,index_part,i;
	FILE *file_dev;
	UINT8 str[32];
	ERASE_INFO_USER eraseInfo;
	
	ret=detect_partition();
	if(ret!=0){
		printf("Failed to retrive the redboot config .\n");
		return ret;
	}
	
	//check the partion name.
	index_part=-1;
	for(i=0;i<partition_num;i++){
		if(strcmp("RedBoot config",partion_table[i].partion_name)==0){
			index_part=i;
			break;
		}
	}
	if(-1==index_part){
		printf("Failed to retrive the redboot config .\n");
		return -1;
	}
	memset(str,0x00,sizeof(str));
	sprintf(str,"/dev/mtd%d",index_part);
	file_dev=fopen(str,"r");
	if(NULL==file_dev){
		perror(NULL);
		printf("Failed to open the device file \"%s\".\n",str);
		return -1;
	}
	memset(tempBuf,0x00,sizeof(tempBuf));

	ret=fread(tempBuf,1,MAX_CONFIG_DATA,file_dev);
	if(EOF==ret){
		printf("Failed to read \"%s\" device file .\n",str);
		return -1;
	}
	for(i=0;i<MAX_CONFIG_DATA;i++){
		printf("%02x",tempBuf[i]);
	}

	fclose(file_dev);
	return 0;

}
/*
 *Function:update the usr partion for v300. For the sake of speed ,use tar .
  Parameter:int index, the index of partion.
  		UNIT8 *image. the image name ,include the path.
  Return Value:
  	0:success. 
	-1:fail.
 */
int update_usr_v300(int index,UINT8 *image){
	int ret;
	UINT8 cmd[128];

	//chreat mount point in /tmp 
	strcpy(cmd,"mkdir -p /tmp/usr");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		return -1;
	
	}

	//mount the mtdblock3
	strcpy(cmd,"mount /dev/mtdblock3 /tmp/usr -t jffs2");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		return -1;
	}

	//"rm -fr" go wrong sometimes, so remove the following code .
	//remove the content in  the mtdblock3
	strcpy(cmd,"rm -fr /tmp/usr/* ");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		system("umount /tmp/usr");
		return -1;
	}
	//update the mtdblock3
	strcpy(cmd,"tar -xf ");
	strcat(cmd,image);
	strcat(cmd ," -C /tmp/usr");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		system("umount /tmp/usr");
		return -1;
	}

	//adjust the path for mtdblock3 
	
	strcpy(cmd,"mv /tmp/usr/usr/* /tmp/usr");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		system("umount /tmp/usr");
		return -1;
	}
	
	//remove the usr empty directory.
	strcpy(cmd,"rm -fr /tmp/usr/usr");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		system("umount /tmp/usr");
		return -1;
	}

	//sync
	strcpy(cmd,"sync");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		system("umount /tmp/usr");
		return -1;
	}

	strcpy(cmd,"umount /tmp/usr");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		return -1;
	}
	
	printf("Congratulation,the \"%s\" image has been wroten into \"%s\" partition successfully.\n",image,partion_table[index].partion_name);
	return 0;
}
/*
 *Function:snatch the mtdblock3 image for v300 .
 *Parameter:
 *	int index ,the index of partition. UINT8 *image,the name of snatched image.
 *Return Value:
 *	0:success.
 *	-1:error.
 */
int snatch_usr_v300(int index, UINT8 * image){
	int ret;
	UINT8 cmd[128],path[128];

	//creat mount point in /tmp 
	strcpy(cmd,"mkdir -p /tmp/usr");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		return -1;
	
	}

	//mount the mtdblock3
	strcpy(cmd,"mount /dev/mtdblock3 /tmp/usr -t jffs2");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		return -1;
	}

	//remove the content in  the image
	strcpy(cmd,"rm -fr  ");
	strcat(cmd,image);
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		return -1;
	}

	//record the current work directory.
	ret=getcwd(path,128);
	if(-1==ret){
		printf("get work directory failed!\n");
		return -1;
	}
	ret=chdir("/tmp");
	if(0!=ret){
		printf("failed to change directory to /tmp !\n");
		return -1;
	}
	//snatch the mtdblock3
	strcpy(cmd,"tar -czf ");
	strcat(cmd,image);
	strcat(cmd ," usr");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		return -1;
	}

	//return the origional work directory.
	ret=chdir(path);
	if(0!=ret){
		printf("failed to change directory to /tmp !\n");
		return -1;
	}
	//move  the image to the original work directory.
	strcpy(cmd,"mv /tmp/");
	strcat(cmd,image);
	strcat(cmd," ");
	strcat(cmd,image);
	ret=system(cmd);
	if(0!=ret){
		printf("failed to mv the file to the work directory!\n");
		return -1;
	}
	strcpy(cmd,"umount /tmp/usr");
	ret=system(cmd);
	if(0!=ret){
		printf("%s failed!\n",cmd);
		return -1;
	}
	
	printf("Congratulation,the \"%s\" image has been wroten into \"%s\" partition successfully.\n",partion_table[index].partion_name,image);
	return 0;
}
