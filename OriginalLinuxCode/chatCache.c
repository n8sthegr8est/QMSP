#include "chatCache.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//add a new chat to the cache
void cacheNew(char* hostloc,unsigned long cid,char* nickname){
	FILE*                   file;
	char*                   name;
	char               buf[1024];

	name = "./conDir/conmem.log";
	file = fopen(name,"a");

	snprintf(buf,1024,"%s: %s, %lu",nickname,hostloc,cid);
	fputs(buf,file);
	fclose(file);
}

//retrieve a chat from the cache
struct cachedChat getDetails(char* nickname){
	FILE*                   file;
	char*                   name;
	char               buf[1024];
	unsigned long long      nlen;
	unsigned long long     comma;
	unsigned long long     hclen;
	struct cachedChat      cchat;
	char*                  ngrab;
	unsigned long           chat;

	name = "./conDir/conmem.log";
	file = fopen(name,"r");
	if(file == NULL){
		cchat.hostloc = 0x00;
		cchat.cid     = 0;
		return cchat;
	}

	do{
		if(fgets(buf,1024,file)==NULL){
			cchat.hostloc = 0x00;
			cchat.cid     = 0;
			return cchat;
		}
		nlen = strcspn(buf,":");
	}while(strncmp(nickname,buf,nlen));

	comma = strcspn(buf,",");
	hclen = comma - nlen - 1;
	cchat.hostloc = malloc(hclen);
	strncpy(cchat.hostloc,buf + nlen + 2, hclen - 1);
	ngrab = buf + comma + 2;

	chat = 0;
	while(*ngrab != 0xA && *ngrab != 0x0){
		chat *= 10;
		chat += *ngrab - '0';
		ngrab++;
	}
	cchat.cid = chat;
	return cchat;
}
