#include "chatWriter.h"
#include <stdio.h>

int main(void){
	printf("Adding new user to test chat\n");
	addNewUser(2, 60, 0, "blah");
	printf("new user added\n");

	printf("Adding new post to test chat\n");
	addNewPost(2, 60, 1780709425, "Hello, fellow users!");
	printf("new post added\n");
}
