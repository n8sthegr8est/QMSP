#include "chatReader.h"
#include <stdlib.h>
#include <stdio.h>

int main(void){
	unsigned char bbPerm;
	unsigned char tmPerm;
	unsigned char esPerm;
	unsigned char utPerm;
	unsigned char fakePm;
	
	char* bnick;
	char* tnick;
	char* enick;
	char* unick;
	char* fnick;

	bbPerm = getPermOfUser(2, 152);
	bnick  = getNicknameOfUser(2, 152);
	tmPerm = getPermOfUser(2, 146);
	tnick  = getNicknameOfUser(2, 146);
	esPerm = getPermOfUser(2, 231);
	enick  = getNicknameOfUser(2, 231);
	utPerm = getPermOfUser(2, 342);
	unick  = getNicknameOfUser(2, 342);

	fakePm = getPermOfUser(2,60);
	fnick  = getNicknameOfUser(2,60);

	printf("Nickname Tests\n");
	printf("Expected nickname of 152: Billy Bob\n");
	printf("Actual   nickname of 152: %s\n",bnick);
	printf("Expected nickname of 146: Taco Man\n");
	printf("Actual   nickname of 146: %s\n",tnick);
	printf("Expected nickname of 231: Evil Steven\n");
	printf("Actual   nickname of 231: %s\n",enick);
	printf("Expected nickname of 342: Utopia\n");
	printf("Actual   nickname of 342: %s\n",unick);
	printf("Expected nickname of  60: \n");
	printf("Actual   nickname of  60: %s\n",fnick);

	printf("\nPerms Tests\n");
	printf("Expected Perms of 152: 0xE7\n");
	printf("Actual   Perms of 152: 0x%X\n",bbPerm);
	printf("Expected Perms of 146: 0x7\n");
	printf("Actual   Perms of 146: 0x%X\n",tmPerm);
	printf("Expected Perms of 231: 0x0\n");
	printf("Actual   Perms of 231: 0x%X\n",esPerm);
	printf("Expected Perms of 342: 0x27\n");
	printf("Actual   Perms of 342: 0x%X\n",utPerm);
	printf("Expected Perms of  60: 0xFF\n");
	printf("Actual   Perms of  60: 0x%X\n",fakePm);

	free(bnick);
	free(tnick);
	free(enick);
	free(unick);

	//152, 11100111, Billy Bob
	//146, 00000111, Taco Man
	//231, 00000000, Evil Steven
	//342, 00100111, Utopia
}
