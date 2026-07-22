//#include "QMSPkernel.c"
#include "chatWriter.h"
#include "chatReader.h"
#include "MessageReader.h"
#include "Messages.h"
#include "Frames.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Pack a message into a series of bytes for sending
unsigned char* PackMessage(unsigned char* DataMessageHeader, void** AllFrames, int HeadLen, int FrameCount){
	unsigned char*     mySerial;
	unsigned long long   curlen;
	unsigned long long   tpoint;
	unsigned char*       ppoint;

	//we'll reuse the already serialized message header pointer
	mySerial = DataMessageHeader;
	curlen = HeadLen;
	//get the size of all frames
	for(int i = 0; i < FrameCount; i++){
		tpoint = ((struct Frame*) *(AllFrames + i))->type;
		curlen += getSerializedSizeOfFrame(*(AllFrames+i),tpoint);
	}

	//reallocate size to accomodate frame data
	mySerial = realloc(mySerial,curlen);
	ppoint = mySerial + HeadLen;
	//serialize all frame data into our message
	for(int i = 0; i < FrameCount; i++){
		tpoint = ((struct Frame*) *(AllFrames+i))->type;
		ppoint = SerializeFrameInto(*(AllFrames+i),tpoint,ppoint);
	}

	return mySerial;
}

//extract a message from a series of bytes received
struct Message unpackMessage(unsigned char* byteStream, unsigned long long byteCount){
	struct DataMessageHeader       dmh;
	struct ControlMessageHeader    cmh;
	void**                      frames;
	unsigned char*              curLoc;
	struct Message              newMsg;

	curLoc = byteStream;
	//rebuild the message header
	if(*byteStream>>7){
		cmh = rebuildControlMessage(byteStream);
		curLoc += 1+4+(cmh.Stub.ChatIDLength+1)+getSizeOfLVIToPack(cmh.SourceID)+(cmh.Stub.MessageNumberLength+1);
		newMsg.msghead = malloc(sizeof(struct ControlMessageHeader));
		memcpy(newMsg.msghead,(void*) &cmh,sizeof(struct ControlMessageHeader));
	}
	else{
		dmh = rebuildDataMessage(byteStream);
		curLoc += 1+(dmh.Stub.ChatIDLength+1)+getSizeOfLVIToPack(dmh.SourceID)+(dmh.Stub.MessageNumberLength+1);
		newMsg.msghead = malloc(sizeof(struct ControlMessageHeader));
		memcpy(newMsg.msghead,(void*) &dmh,sizeof(struct DataMessageHeader));
	}

	//rebuild the frames
	frames = extractFrames(byteCount-(curLoc-byteStream),curLoc);
	newMsg.msgframes = frames;
	return newMsg;
}

//send the sent message to the kernel for state update
void applyPacked(){
}

//send the received message to the kernel for state update (in this implementation, just print it to the chat. Time constraints are not fun)
void applyMessage(struct Message message){
	//due to time constraints, messages can only have two frames, so we know what we're looking for
	struct DataMessageHeader*      dmh;
	struct ControlMessageHeader*   cmh;
	char*                          stb;
	unsigned long                  cid;
	unsigned long long             uid;

	stb = (char*)message.msghead;
	if(!(*stb>>7)){
		dmh= (struct DataMessageHeader*) message.msghead;
		cid = dmh->ChatID;
		uid = dmh->SourceID;
	}
	else{
		cmh = (struct ControlMessageHeader*) message.msghead;
		cid = cmh->ChatID;
		uid = cmh->SourceID;
	}

	if(getPermOfUser(cid,uid)==255){
		addNewUser(cid,uid,0,"x");
	}

	addNewPost(cid, uid, 0, ((struct PlaintextFrame*) message.msgframes[1])->txtdta);
}
