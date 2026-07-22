#include "Frames.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//counts the amount of bytes in a frame with no set length nor a required length parameter
uint64_t countUntilTerminator(unsigned char* start){
	uint64_t       counter;
	unsigned char* pointer;
	
	counter = 0;
	pointer = start;
	while(*pointer != 0x00){
		counter++;
		pointer++;
	}

	return counter;
}

//extract a transit metadata frame from a series of bytes.
struct TransitMetadataFrame* unpackTransitMetadata(uint64_t byteCount, unsigned char* frameStart, unsigned char** myEnd){
	unsigned char*               curLoc;
	struct TransitMetadataFrame* newTMD;

	curLoc = frameStart;
	
	newTMD = malloc(sizeof(struct TransitMetadataFrame));//Transit Metadata Frames are a fixed size, so we know how big it needs to be.
	if(newTMD == NULL){
		return NULL;
	}

	newTMD->type = unpackLogVarInt(curLoc);//grab the type at the frame's start
	curLoc += getSizeOfLVIToPack(newTMD->type);//move to the first byte after the type

	//extract the four-byte-long post transit id
	newTMD->PostTransitID   = *curLoc;
	newTMD->PostTransitID <<= 8;
	curLoc                 += 1;
	newTMD->PostTransitID  += *curLoc;
	newTMD->PostTransitID <<= 8;
	curLoc                 += 1;
	newTMD->PostTransitID  += *curLoc;
	newTMD->PostTransitID <<= 8;
	curLoc                 += 1;
	newTMD->PostTransitID  += *curLoc;
	curLoc                 += 1;

	newTMD->TotalMessageCount = unpackLogVarInt(curLoc);//grab the total message count
	curLoc += getSizeOfLVIToPack(newTMD->TotalMessageCount);//move to the first byte after the total message count
	newTMD->SpecificMessageCount = unpackLogVarInt(curLoc);//grab the specific message count

	*myEnd = curLoc + getSizeOfLVIToPack(newTMD->SpecificMessageCount)-1;//point the end of frame pointer to the end of this frame as a side effect
									     //to aid the frame unpacking function.
	return newTMD;
}

struct PostMetadataFrame* unpackPostMetadata(uint64_t byteCount, unsigned char* frameStart, unsigned char** myEnd){
	unsigned char*            curLoc;
	unsigned char*            cpyLoc;
	struct PostMetadataFrame* newPMD;
	struct PostMetadataFrame*  tempy;
	uint64_t                  lcount;
	uint64_t                  linkid;
	int                       lidsze;

	curLoc = frameStart;
	
	newPMD = malloc(sizeof(struct PostMetadataFrame));
	if(newPMD == NULL){
		return NULL;
	}

	newPMD->type = unpackLogVarInt(curLoc);
	curLoc += getSizeOfLVIToPack(newPMD->type);

	newPMD->PostID = unpackLogVarInt(curLoc);
	curLoc += getSizeOfLVIToPack(newPMD->PostID);

	newPMD->Timestamp = unpackLogVarInt(curLoc);
	curLoc += getSizeOfLVIToPack(newPMD->Timestamp);

	tempy = realloc(newPMD,sizeof(newPMD)+1);
	if(tempy == NULL){
		free(newPMD);
		return NULL;
	}

	lcount = 0;
	newPMD = tempy;
	cpyLoc = tempy + (curLoc-frameStart) + 1;
	
	while(*curLoc != 0x00){
		lcount++;
		tempy = realloc(newPMD,sizeof(newPMD) + 1 + (sizeof(struct LinkageIndex) * lcount));
		
		if(tempy == NULL){
			free(newPMD);
			return NULL;
		}
		newPMD = tempy;
		cpyLoc = tempy + (curLoc-frameStart)+1;
		
		*cpyLoc = *curLoc;
		cpyLoc++;
		curLoc++;

		linkid = unpackLogVarInt(curLoc);
		lidsze = getSizeOfLVIToPack(linkid);

		memcpy(cpyLoc,&linkid,lidsze);
		curLoc += lidsze;
		cpyLoc += lidsze;
	}
	*cpyLoc = 0x00;
	
	*myEnd = curLoc;
	return newPMD;
}

//extract a plaintext frame from a series of bytes.
struct PlaintextFrame* unpackPlaintext(uint64_t byteCount, unsigned char* frameStart, unsigned char** myEnd){
	unsigned char*         curLoc;
	struct PlaintextFrame* newPTF;
	struct PlaintextFrame*  tempy;
	uint64_t                 hold;

	curLoc = frameStart;
	
	newPTF = malloc(sizeof(struct PlaintextFrame));//allocate the size of the fixed part of a plaintext frame
	if(newPTF == NULL){
		return NULL;
	}

	newPTF->type = unpackLogVarInt(curLoc);//unpack the type and move to the first byte after it
	curLoc += getSizeOfLVIToPack(newPTF->type);

	if(newPTF->type & 1){//if this is a type 0x03 Plaintext frame, the Length is specified
		newPTF->Length = unpackLogVarInt(curLoc);//unpack the length
		curLoc += getSizeOfLVIToPack(newPTF->Length);
		hold = newPTF->Length;//set the number of bytes to copy for the remainder of the frame
	}
	else{
		hold = countUntilTerminator(curLoc);//set the number of bytes to copy for the remainder of the frame
		newPTF->Length = hold;//set that as the length
	}

	tempy = realloc(newPTF,(sizeof(struct PlaintextFrame) + hold));//reallocate the frame with the now known size
	if(tempy==NULL){
		free(newPTF);
		return NULL;
	}
	newPTF = tempy;

	memcpy(newPTF->txtdta, curLoc, hold);//copy the remaining bytes into the text data of the frame.
	
	*myEnd = curLoc + hold - 1;
	return newPTF;
}

//extract all frames from this series of bytes.
void** extractFrames(uint64_t byteCount, unsigned char* bytes){
	unsigned char*   curLoc;
	unsigned char*  lastEnd;
	int            frmCount;
	void**         myFrames;

	uint64_t      typeValue;

	curLoc  = bytes;//prepare the pointers to extract the frames
	lastEnd = bytes;
	frmCount = 1;
	myFrames = NULL;

	while(lastEnd - bytes + 2 < byteCount){
		myFrames = realloc(myFrames,frmCount*sizeof(char*));//add a new frame pointer to the list
		typeValue = unpackLogVarInt(curLoc);//get the type of the next frame
		switch(typeValue){//call the specific unpacking function for each type
			case 0x00:
				myFrames[frmCount-1] = unpackTransitMetadata(byteCount - (lastEnd - bytes + 2), curLoc, &lastEnd);
				break;
			case 0x01:
				myFrames[frmCount-1] = unpackPostMetadata(byteCount-(lastEnd - bytes + 2), curLoc, &lastEnd);
				break;
			case 0x02:
			case 0x03:
				myFrames[frmCount-1] = unpackPlaintext(byteCount - (lastEnd - bytes + 2), curLoc, &lastEnd);
				break;
			case 0x04:
			case 0x05:
				//unpack FText
				break;
			case 0x06:
				//unpack Formatting
				break;
			case 0x07:
				//unpack Load
				break;
			case 0x08:
				//unpack Encoding Negotiation
				break;
			case 0x09:
				//unpack UserDatagram
				break;
			case 0x0A:
				//unpack ChangeDenial
				break;
			case 0x0B:
				//unpack ChangeApproval
				break;
			case 0x0C:
				//unpack Login
				break;
			case 0x0D:
				//unpack Connection Acknowledgement
				break;
			case 0x0E:
				//unpack Logout
				break;
			default:
				//undefined frame type
				break;
		}
		frmCount++;//add another frame
		curLoc = lastEnd+1; //reset the position of the next frame pointer
	}
	return myFrames;
}

//Get the size of a transit metadata frame once it has been serialized
uint64_t getSerializedSizeOfTMD(struct TransitMetadataFrame* frame){
	uint64_t mySize;

	mySize  = 1;//the type is always one byte in this frame type, 0x00.
	mySize += 4;//the post id is four bytes.
	mySize += getSizeOfLVIToPack(frame->TotalMessageCount);
	mySize += getSizeOfLVIToPack(frame->SpecificMessageCount);

	return mySize;
}

uint64_t getSerializedSizeOfPMD(struct PostMetadataFrame* frame){
	uint64_t       mySize;
	unsigned char* lnkptr;
	int            lndsze;

	mySize = 1;//the type is always one byte in this frame type, 0x01.
	mySize += getSizeOfLVIToPack(frame->PostID);
	mySize += getSizeOfLVIToPack(frame->Timestamp);
	//mySize += countUntilTerminator((char*) frame + mySize);

	lnkptr = frame + (sizeof(uint64_t) * 3);//point the link pointer to the first index of the Linkage array.

	while(*lnkptr != 0x00){//0x00 marks the end of the linkage list, so keep adding to the size until we hit it.
		mySize += 1;//for the link type
		lndsze  = getSizeOfLVIToPack(lnkptr+1);//get the size of the link id once it is serialized
		mySize += lndsze;//add this to our total size
		lnkptr += lndsze + 1;//now point to the next link type index and loop again.
	}
	mySize += 1;//the terminator also needs to be sent over the wire, so add the one byte it takes into the size.

	return mySize;
}

//Get the size of a transit metadata frame once it has been serialized
uint64_t getSerializedSizeOfPTF(struct PlaintextFrame* frame){
	uint64_t       totalBytesCounted;
	unsigned char*    counterpointer;
	
	if(frame->type & 1){//there is a length frame if the least significant bit is set
		return frame->Length + getSizeOfLVIToPack(frame->Length) + 1;//the type of the frame is for sure a single byte.
	}
	else{
		totalBytesCounted = 1;//start at 1, because the flag to stop is also part of the data and won't be counted otherwise.
		counterpointer = ((unsigned char*) frame) + (2 * sizeof(uint64_t));
		//since there's no length parameter, or rather that we should ignore it for packing, we need to actively count the bytes. We'll know to stop when we hit 0x00.
		while(*counterpointer!=0x00){
			counterpointer += 1;
			totalBytesCounted += 1;
		}
		return 1 + totalBytesCounted;
	}
}

//Get the size of an Encoding Negotiation frame once it has been serialized
uint64_t getSerializedSizeOfENF(struct EncodingNegotiationFrame* frame){
	return 2;//Might change if there is ever a need to allow a new standard encoding mode. For now only a single type byte and a 
}

//Get the size of a generic frame once it has been serialized
uint64_t getSerializedSizeOfFrame(void* frame, uint64_t FrameType){
	struct TransitMetadataFrame* TMD;
	struct PlaintextFrame*       PTF;
	//More To Be Added
	
	switch(FrameType){
		case 0x00:
			TMD = (struct TransitMetadataFrame*) frame;
			return getSerializedSizeOfTMD(TMD);
		case 0x01:
			//getSizeOfPMD
		case 0x02:
		case 0x03:
			PTF = (struct PlaintextFrame*) frame;
			return getSerializedSizeOfPTF(PTF);
		case 0x04:
		case 0x05:
			//getSizeOfFText
		case 0x06:
			//getSizeOfFormatting
		case 0x07:
			//getSizeOfLoad
		case 0x08:
			//ENF = frame;
			//return getSerializedSizeOfENF(ENF);
		case 0x09:
			//getSizeOfUserDatagram
		case 0x0A:
			//getSizeOfChangeDenial
		case 0x0B:
			//getSizeOfChangeApproval
		case 0x0C:
			return 1;//Login
		case 0x0D:
			return 2;//Connection Acknowledgement
		case 0x0E:
			return 1;//Logout
		default:
			return 0;//undefined frame type
	}
}

//Serialize a transit metadata frame into a series of bytes
unsigned char* SerializeTMD(struct TransitMetadataFrame* frame){
	unsigned char*   mySerial;
	uint64_t       packedType;
	uint64_t       packedTMsg;
	uint64_t       packedSMsg;
	uint32_t       packedTrID;
	int              typeSize;
	int              tMsgSize;
	int              sMsgSize;

	//get the sizes for mallocing
	typeSize   = getSizeOfLVIToPack(frame->type);
	tMsgSize   = getSizeOfLVIToPack(frame->TotalMessageCount);
	sMsgSize   = getSizeOfLVIToPack(frame->SpecificMessageCount);

	//pack all the values
	packedType = packLogVarInt(frame->type);
	packedTMsg = packLogVarInt(frame->TotalMessageCount);
	packedSMsg = packLogVarInt(frame->SpecificMessageCount);
	packedTrID = frame->PostTransitID;
	if(isLittleEndian()){
		packedTrID = rev_32(packedTrID);
		//packedTrID = __builtin_bswap32(packedTrID);
	}

	mySerial = malloc(getSerializedSizeOfTMD(frame));

	//copy the packed data into a byte stream
	memcpy(mySerial                    , &packedType, typeSize);
	memcpy(mySerial+typeSize           , &packedTrID, 4       );
	memcpy(mySerial+typeSize+4         , &packedTMsg, tMsgSize);
	memcpy(mySerial+typeSize+4+tMsgSize, &packedSMsg, sMsgSize);

	return mySerial;
}

//Serialize a generic frame into a series of bytes
unsigned char* SerializeFrame(void* frame, uint64_t frameType){
	switch(frameType){
		case 0x00:
			return SerializeTMD((struct TransitMetadataFrame*) frame);
		case 0x01:
			//serialize PMD
		case 0x02:
		case 0x03:
			//return SerializePTF((struct PlaintextFrame*) frame);
			//serialize Plaintext
		case 0x04:
		case 0x05:
			//serialize FText
		case 0x06:
			//serialize Formatting
		case 0x07:
			//serialize Load
		case 0x08:
			//ENF = frame;
			//return getSerializedSizeOfENF(ENF);
		case 0x09:
			//serialize UserDatagram
		case 0x0A:
			//serialize ChangeDenial
		case 0x0B:
			//serialize ChangeApproval
		case 0x0C:
			//Login
		case 0x0D:
			//Connection Acknowledgement
		case 0x0E:
			//Logout
		default:
			return NULL;
			//undefined frame type
	}
}

//Serialize a transit metadata frame into a series of bytes in an existing buffer
unsigned char* SerializeTMDInto(struct TransitMetadataFrame* frame, unsigned char* loc){
	unsigned char*   mySerial;
	uint64_t       packedType;
	uint64_t       packedTMsg;
	uint64_t       packedSMsg;
	uint32_t       packedTrID;
	int              typeSize;
	int              tMsgSize;
	int              sMsgSize;

	//get size to malloc
	typeSize   = getSizeOfLVIToPack(frame->type);
	tMsgSize   = getSizeOfLVIToPack(frame->TotalMessageCount);
	sMsgSize   = getSizeOfLVIToPack(frame->SpecificMessageCount);

	//pack all values for copying
	packedType = packLogVarInt(frame->type);
	packedTMsg = packLogVarInt(frame->TotalMessageCount);
	packedSMsg = packLogVarInt(frame->SpecificMessageCount);
	packedTrID = frame->PostTransitID;
	if(isLittleEndian()){
		packedTrID = rev_32(packedTrID);
		//packedTrID = __builtin_bswap32(packedTrID);
	}

	//set up the copying location
	mySerial = loc;

	//copy bytes into location
	memcpy(mySerial                    , &packedType, typeSize);
	memcpy(mySerial+typeSize           , &packedTrID, 4       );
	memcpy(mySerial+typeSize+4         , &packedTMsg, tMsgSize);
	memcpy(mySerial+typeSize+4+tMsgSize, &packedSMsg, sMsgSize);

	//return the pointer to the next memory address after the serialized data
	return mySerial+typeSize+4+tMsgSize+sMsgSize;
}

//Serialize a post metadata frame into a series of bytes in an existing buffer
unsigned char* SerializePMDInto(struct PostMetadataFrame* frame, unsigned char* loc){
	unsigned char*   mySerial;
	uint64_t       packedType;
	uint64_t       packedPsID;
	uint64_t       packedTime;
	uint64_t       packedLink;
	int              typeSize;
	int              psIDSize;
	int              timeSize;
	int              linkSize;
	unsigned char* linkageptr;

	//get size to malloc
	typeSize   = getSizeOfLVIToPack(frame->type);
	psIDSize   = getSizeOfLVIToPack(frame->PostID);
	timeSize   = getSizeOfLVIToPack(frame->Timestamp);

	//pack all values for copying
	packedType = packLogVarInt(frame->type);
	packedPsID = packLogVarInt(frame->PostID);
	packedTime = packLogVarInt(frame->Timestamp);
	//packedTrID = frame->PostTransitID;
	//if(isLittleEndian()){
	//	packedTrID = __builtin_bswap32(packedTrID);
	//}

	//set up the copying location
	mySerial = loc;

	//copy bytes into location
	memcpy(mySerial                    , &packedType, typeSize);
	memcpy(mySerial+typeSize           , &packedPsID, psIDSize);
	memcpy(mySerial+typeSize+psIDSize  , &packedTime, timeSize);
	//memcpy(mySerial+typeSize+4+tMsgSize, &packedSMsg, sMsgSize);

	//move the copy location to the end of what we just copied to set up for copying the linkage
	mySerial += typeSize + psIDSize + timeSize;

	//point the linkage pointer to the beginning of the linkage data
	linkageptr = frame + (sizeof(uint64_t)*3);

	//while we haven't hit the terminator flag, continually pack and copy the next link
	while(*linkageptr!=0x00){
		memcpy(mySerial, linkageptr, 1);
		
		linkageptr += 1;
		linkSize = getSizeOfLVIToPack(*linkageptr);
		packedLink = packLogVarInt(linkageptr);

		memcpy(mySerial+1, &packedLink, linkSize);
		
		mySerial += linkSize + 1;
		linkageptr += linkSize;
	}

	//now copy the terminator flag into the buffer to mark the end of the message
	memcpy(mySerial, linkageptr, 1);

	//return the pointer to the next memory address after the serialized data
	return mySerial + 1;
}

//Serialize a plaintext frame into a series of bytes in an existing buffer
unsigned char* SerializePTFInto(struct PlaintextFrame* frame, unsigned char* loc){
	unsigned char*   mySerial;
	uint64_t       packedType;
	uint64_t        packedLen;
	int              typeSize;
	int               lenSize;
	uint64_t         textSize;

	//get size
	typeSize = getSizeOfLVIToPack(frame->type);

	//pack the type
	packedType = packLogVarInt(frame->type);

	//setup copying location
	mySerial = loc;

	if(frame->type & 1){
		//if it needs a length field, pack it
		lenSize   = getSizeOfLVIToPack(frame->Length);
		packedLen = packLogVarInt(frame->Length);
	}
	else{
		//otherwise it is omitted
		lenSize = 0;
		packedLen = 0;
	}
	//get the size of the text
	textSize = getSerializedSizeOfPTF(frame)-1-lenSize;

	//copy data into stream
	memcpy(mySerial                 , &packedType   , typeSize);
	memcpy(mySerial+typeSize        , &packedLen    , lenSize );
	memcpy(mySerial+typeSize+lenSize, &frame->txtdta, textSize);
	
	//return the pointer to the next memory address after the serialized data
	return mySerial+typeSize+lenSize+textSize;
}

//Serialize a generic frame into a series of bytes in an existing buffer
unsigned char* SerializeFrameInto(void* frame, uint64_t frameType, unsigned char* loc){
	switch(frameType){
		case 0x00:
			return SerializeTMDInto((struct TransitMetadataFrame*) frame,loc);
		case 0x01:
			return SerializePMDInto((struct PostMetadataFrame*) frame,loc);
		case 0x02:
		case 0x03:
			return SerializePTFInto((struct PlaintextFrame*) frame,loc);
		case 0x04:
		case 0x05:
			//return FText
		case 0x06:
			//return Formatting
		case 0x07:
			//return Load
		case 0x08:
			//return EncodingNegotiation;
		case 0x09:
			//return UserDatagram
		case 0x0A:
			//return ChangeDenial
		case 0x0B:
			//retuurn ChangeApproval
		case 0x0C:
			//return Login
		case 0x0D:
			//return Connection Acknowledgement
		case 0x0E:
			//return Logout
		default:
			return NULL;//undefined frame type
	}
}
