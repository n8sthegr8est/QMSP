struct cachedChat{
	char*         hostloc;
	unsigned long     cid;
};

void cacheNew(char* hostloc,unsigned long cid,char* nickname);

struct cachedChat getDetails(char* nickname);
