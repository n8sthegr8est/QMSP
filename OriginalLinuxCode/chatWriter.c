#include "chatReader.h"
#include "chatWriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

//find the chat with a given id and store the name in a pointer to send back to the caller.
FILE* findChat_storeName(unsigned long cid, char** fileName){
	DIR*  chatsDir;
	struct dirent* entry;
	char  findLine[19];
	char  filepath[1024];
	FILE* file;
	char  idLine[19];

	sprintf(findLine, "ChatID: %lu", cid);

	chatsDir = opendir("./localChats");
	if(chatsDir == NULL){
		return NULL;
	}

	while((entry = readdir(chatsDir)) != NULL){
		if (entry->d_type == DT_REG){
			snprintf(filepath, sizeof(filepath),"./localChats/%s", entry->d_name);
			file = fopen(filepath,"r+");
			if(file != NULL){
				if(fgets(idLine,19,file) != NULL){
					if(strncmp(idLine,findLine,strlen(findLine)) == 0){
						*fileName = realloc(*fileName,strlen(filepath));
						strcpy(*fileName,filepath);
						closedir(chatsDir);
						return file;
					}
				}
			fclose(file);
			}
		}
	}
	closedir(chatsDir);
	return NULL;
}

//find the file path to a chat with a given id.
char* findChatName(unsigned long cid){
	DIR*  chatsDir;
	struct dirent* entry;
	char  findLine[19];
	char  filepath[1024];
	FILE* file;
	char  idLine[19];
	char* fileName;

	sprintf(findLine, "ChatID: %lu", cid);

	chatsDir = opendir("./localChats");
	if(chatsDir == NULL){
		return NULL;
	}

	while((entry = readdir(chatsDir)) != NULL){
		if (entry->d_type == DT_REG){
			snprintf(filepath, sizeof(filepath),"./localChats/%s", entry->d_name);
			file = fopen(filepath,"r+");
			if(file != NULL){
				if(fgets(idLine,19,file) != NULL){
					if(strncmp(idLine,findLine,strlen(findLine)) == 0){
						fileName = malloc(strlen(filepath));
						strcpy(fileName,filepath);
						closedir(chatsDir);
						fclose(file);
						return fileName;
					}
				}
			fclose(file);
			}
		}
	}
	closedir(chatsDir);
	return NULL;
}

//get the number of messages in this chat
unsigned long long getNumberOfMessages(unsigned long cid){
	FILE* 		        file;
	unsigned long long     count;
	char               buf[1024];

	file = findChat(cid);
	count = 0;
	fgets(buf,1024,file);
	while(strncmp("Messages:\n",buf,10)!=0){
		fgets(buf,1024,file);
	}
	while(fgets(buf,1024,file)!=NULL){
		count++;
	}
	fclose(file);
	return count;
}

//add a new user to this chat
void addNewUser(unsigned long cid, unsigned long long uid, char permissions, char* nickname){
	FILE*       file;
	FILE*      tempy;
	char*   origName;
	char   buf[1024];
	char  newUsr[30];
	char  cidLin[18];

	origName = NULL;
	file = findChat_storeName(cid,&origName);
	tempy = fopen("./localChats/copyChat.tmp", "w");

	snprintf(newUsr,30,"\t%llu, 11100111, Guest\n",uid);
	snprintf(cidLin,18,"ChatID: %lu\n",cid);

	fputs(cidLin,tempy);

	while(fgets(buf, 1024, file) != NULL){
		if(strncmp(buf,"Messages:\n",10) == 0){
			fputs(newUsr,tempy);
		}
		fputs(buf,tempy);
	}

	fclose(file);
	fclose(tempy);

	remove(origName);
	rename("./localChats/copyChat.tmp",origName);
	free(origName);
}

//add a new post to this chat.
void addNewPost(unsigned long cid, unsigned long long uid, unsigned long long time, char* text){
	FILE*                   file;
	char*                   name;
	unsigned long long       mid;
	char               buf[1024];
	unsigned long long      tlen;

	name = findChatName(cid);
	mid  = getNumberOfMessages(cid);
	
	file = fopen(name,"a");
	free(name);

	snprintf(buf,1024,"\t%llu, %llu, %llu, 0, , %c",mid,uid,time,2);
	fputs(buf,file);
	tlen = 0;
	while(tlen < strlen(text)){
		snprintf(buf,1024,"%s",text+tlen);
		tlen += 1024;
		fputs(buf,file);
	}
	fputs("\x03",file);
	fclose(file);
}
