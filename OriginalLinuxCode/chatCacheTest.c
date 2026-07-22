#include "chatCache.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	struct cachedChat myTest;

	myTest = getDetails("test");

	printf("Expected Location: localhost\n");
	printf("Actual   Location: %s\n",myTest.hostloc);
	printf("Expected ChatID:   2\n");
	printf("Actual   ChatID:   %lu\n",myTest.cid);
	free(myTest.hostloc);

	printf("Caching new as localhost, 7, \"coolness\"\n");
	cacheNew("localhost",7,"coolness");
	printf("Cached new.\n");
}
