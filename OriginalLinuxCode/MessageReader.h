struct Message{
	void*    msghead;
	void** msgframes;
};

unsigned char* PackMessage(unsigned char* DataMessageHeader, void** AllFrames, int HeadLen, int FrameCount);

struct Message unpackMessage(unsigned char* byteStream, unsigned long long byteCount);

void applyMessage(struct Message message);
