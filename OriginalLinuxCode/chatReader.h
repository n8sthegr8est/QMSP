#include <stdint.h>
#include <stdio.h>

#pragma pack(push,1)
struct Post{
	unsigned long long     postid;
	unsigned long long        uid;
	uint64_t            timestamp;
	unsigned long long*    mLinks;
	unsigned long long*    uLinks;
	char*                txtdta[]; 
};

struct UserInfo{
	unsigned long long          uid;
	char               permsissions;
	char*                  Nickname;
};
#pragma pack(pop)

FILE* findChat(unsigned long cid);

int confirmChat(unsigned long long cid);

struct Post* getLastMessage(unsigned long cid, unsigned long long prev);

struct Post** getMessages(unsigned long cid);

unsigned char getPermOfUser(unsigned long cid, unsigned long long uid);

char* getNicknameOfUser(unsigned long cid, unsigned long long uid);
