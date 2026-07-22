#include "Frames.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//counts the amount of bytes in a frame with no set length nor a required length parameter
unsigned long long countUntilTerminator(unsigned char* start){
	unsigned long long counter;
	unsigned char*     pointer;
	
	counter = 0;
	pointer = start;
	while(*pointer != 0x00){
		counter++;
		pointer++;
	}

	return counter;
}

//extract a transit metadata frame from a series of bytes.
struct TransitMetadataFrame* unpackTransitMetadata(unsigned long long byteCount, unsigned char* frameStart, unsigned char** myEnd){
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

/*struct PostMetadataFrame* unpackPostMetadata(unsigned long long byteCount, unsigned char* frameStart, unsigned char** myEnd){
	unsigned char*            curLoc;
	struct PostMetadataFrame* newPMD;

	curLoc = frameStart;
	
	newPMD = malloc(sizeof(struct PostMetadataFrame));
	if(newPMD == NULL){
		return NULL;
	}

	newPMD->type = unpackLogVarInt(curLoc);
	curLoc += getSizeOfLVIToPack(newTMD->type);
	
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

	newTMD->TotalMessageCount = unpackLogVarInt(curLoc);
	curLoc += getSizeOfLVIToPack(newTMD->TotalMessageCount);
	newTMD->SpecificMessageCount = unpackLogVarInt(curLoc);

	*myEnd = curLoc + getSizeOfLVIToPack(newTMD->SpecificMessageCount)-1;
	return newTMD;
}*/

//extract a plaintext frame from a series of bytes.
struct PlaintextFrame* unpackPlaintext(unsigned long long byteCount, unsigned char* frameStart, unsigned char** myEnd){
	unsigned char*         curLoc;
	struct PlaintextFrame* newPTF;
	struct PlaintextFrame*  tempy;
	unsigned long long       hold;

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
void** extractFrames(unsigned long long byteCount, unsigned char* bytes){
	unsigned char*   curLoc;
	unsigned char*  lastEnd;
	int            frmCount;
	void**         myFrames;

	unsigned long long      typeValue;

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
				//unpack PMD
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
unsigned long long getSerializedSizeOfTMD(struct TransitMetadataFrame* frame){
	unsigned long long mySize;

	mySize  = 1;//the type is always one byte in this frame type, 0x00.
	mySize += 4;//the post id is four bytes.
	mySize += getSizeOfLVIToPack(frame->TotalMessageCount);
	mySize += getSizeOfLVIToPack(frame->SpecificMessageCount);

	return mySize;
}

//Get the size of a transit metadata frame once it has been serialized
unsigned long long getSerializedSizeOfPTF(struct PlaintextFrame* frame){
	unsigned long long totalBytesCounted;
	unsigned char*        counterpointer;
	
	if(frame->type & 1){//there is a length frame if the least significant bit is set
		return frame->Length + getSizeOfLVIToPack(frame->Length) + 1;//the type of the frame is for sure a single byte.
	}
	else{
		totalBytesCounted = 1;//start at 1, because the flag to stop is also part of the data and won't be counted otherwise.
		counterpointer = ((unsigned char*) frame) + (2 * sizeof(unsigned long long));
		//since there's no length parameter, or rather that we should ignore it for packing, we need to actively count the bytes. We'll know to stop when we hit 0x00.
		while(*counterpointer!=0x00){
			counterpointer += 1;
			totalBytesCounted += 1;
		}
		return 1 + totalBytesCounted;
	}
}

//Get the size of an Encoding Negotiation frame once it has been serialized
unsigned long long getSerializedSizeOfENF(struct EncodingNegotiationFrame* frame){
	return 2;//Might change if there is ever a need to allow a new standard encoding mode. For now only a single type byte and a 
}

//Get the size of a generic frame once it has been serialized
unsigned long long getSerializedSizeOfFrame(void* frame, unsigned long long FrameType){
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
	unsigned char*       mySerial;
	unsigned long long packedType;
	unsigned long long packedTMsg;
	unsigned long long packedSMsg;
	uint32_t           packedTrID;
	int                  typeSize;
	int                  tMsgSize;
	int                  sMsgSize;

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
		packedTrID = __builtin_bswap32(packedTrID);
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
unsigned char* SerializeFrame(void* frame, unsigned long long frameType){
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
	unsigned char*       mySerial;
	unsigned long long packedType;
	unsigned long long packedTMsg;
	unsigned long long packedSMsg;
	uint32_t           packedTrID;
	int                  typeSize;
	int                  tMsgSize;
	int                  sMsgSize;

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
		packedTrID = __builtin_bswap32(packedTrID);
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

//Serialize a plaintext frame into a series of bytes in an existing buffer
unsigned char* SerializePTFInto(struct PlaintextFrame* frame, unsigned char* loc){
	unsigned char*       mySerial;
	unsigned long long packedType;
	unsigned long long  packedLen;
	int                  typeSize;
	int                   lenSize;
	unsigned long long   textSize;

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
unsigned char* SerializeFrameInto(void* frame, unsigned long long frameType, unsigned char* loc){
	switch(frameType){
		case 0x00:
			return SerializeTMDInto((struct TransitMetadataFrame*) frame,loc);
		case 0x01:
			//return PostMetadata
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
