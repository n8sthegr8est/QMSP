#include "chatReader.h"
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

//Find the chat with this id number
FILE* findChat(unsigned long cid){
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

//Confirm a chat exists
int confirmChat(unsigned long long cid){
	FILE* myChat;

	myChat = findChat(cid);

	if(myChat == NULL){
		return 0;
	}
	fclose(myChat);
	return 1;
}

//get all messages from a chat
struct Post** getMessages(unsigned long cid){
}

//get messages from the chat starting from the end
struct Post* getLastMessage(unsigned long cid, unsigned long long prev){
	return NULL;
	//FILE*          myChat;
	//struct Post*   oldPst;
	//char             line[1024];
	//unsigned long   fsize;
	//unsigned long  offset;
	//int            nlFlag;
	//unsigned long  lstart;
	//char              seg[1025];
	//unsigned long curSize;

	//myChat = findChat(cid);
	//if(myChat == NULL){
	//	return NULL;
	//}
	
	//if(fseek(myChat,0,SEEK_END) != 0){
	//	fclose(myChat);
	//	return NULL;
	//}

	//fsize = ftell(myChat);
	//if(fsize == 0){
	//	fclose(myChat);
	//	return NULL;
	//}

	//while(offset > 0 && !nlFlag){
	//	curSize = (offset > 1025) ? 1025 : offset;
	//	offset -= curSize;

	//	fseek(myChat,offset,SEEK_SET);
	//	size_t
	//}

	//unsigned long long     postid;
	//unsigned long long        uid;
	//uint64_t            timestamp;
	//unsigned long long*    mLinks;
	//unsigned long long*    uLinks;
	//char*                txtdta[];
}

//get the permissions byte of a specific user in this chat.
unsigned char getPermOfUser(unsigned long cid, unsigned long long uid){
	FILE*                 myChat;
	char               buf[1024];
	unsigned long long   uidcmpr;
	char*                grabber;
	unsigned char           perm;

	myChat = findChat(cid);
	fgets(buf, 1024, myChat);
	while(strncmp(buf,"Messages:\n",10) != 0){
		uidcmpr = 0;
		for(int i = 1; isdigit(buf[i]); i++){
			uidcmpr *= 10;
			uidcmpr += buf[i] - '0';
			grabber = &buf[i];
		}
		if(uidcmpr == uid){
			grabber += 3;
			perm = 0;
			for(int i = 0; i < 8; i++){
				perm   <<= 1;
				perm    += *grabber - '0';
				grabber += 1;
			}
			fclose(myChat);
			return perm;
		}
		fgets(buf,1024,myChat);
	}
	fclose(myChat);
	return 255;
}

//get the nickname of a specific user in this chat
char* getNicknameOfUser(unsigned long cid, unsigned long long uid){
	FILE*                 myChat;
	char               buf[1024];
	unsigned long long   uidcmpr;
	char*                grabber;
	char*                   nick;
	int                     nlen;

	myChat = findChat(cid);
	fgets(buf, 1024, myChat); 
	while(strcmp(buf,"Messages:\n") != 0){
		uidcmpr = 0;
		for(int i = 1; isdigit(buf[i]); i++){
			uidcmpr *= 10;
			uidcmpr += buf[i] - '0';
			grabber = &buf[i];
		}
		if(uidcmpr == uid){
			grabber += 13;
			nlen = 1;
			nick = malloc(nlen);
			while(*grabber != '\n'){
				nick[nlen-1] = *grabber;
				nlen += 1;
				nick = realloc(nick,nlen);
				grabber += 1;
			}
			nick[nlen-1] = 0x00;
			fclose(myChat);
			return nick;
		}
		fgets(buf,1024,myChat);
	}
	fclose(myChat);
	nick = malloc(1);
	nick[0] = 0x00;
	return nick;
}
