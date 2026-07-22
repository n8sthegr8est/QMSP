#include "Messages.h"
#include <stdlib.h>
#include <string.h>

//Get the size of a data message header after serialization
unsigned int getSerializedSizeOfDataMessageHeader(struct DataMessageHeader dmh){
	unsigned int mySize;
	
	mySize = 1 + (dmh.Stub.ChatIDLength + 1) + (dmh.Stub.MessageNumberLength + 1);//These values are largely fixed, so easy to check
	if(dmh.SourceID >> 62){//if the first two bits are occupied with data
		return 0;//The Source ID is too big.
	}
	mySize += getSizeOfLVIToPack(dmh.SourceID);//now add on the calculated size of the source id
	return mySize;
}

//Get the size of a control message header after serialization
unsigned int getSerializedSizeOfControlMessageHeader(struct ControlMessageHeader cmh){
	unsigned int mySize;

	//Works same as above, but with 4 extra bytes for the version number
	mySize = 1 + 4 + (cmh.Stub.ChatIDLength + 1) + (cmh.Stub.MessageNumberLength + 1);
	if(cmh.SourceID >> 62){
		return 0;//The Source ID is too big.
	}
	mySize += getSizeOfLVIToPack(cmh.SourceID);

	return mySize;
}

//turn the message stub into a serialized char. The stub is the first byte.
unsigned char SerializeMessageStub(struct MessageStub stub){
	unsigned char  stubByte;
	unsigned char*  grabber;
	unsigned char      hold;

	grabber = (unsigned char*) &stub;
	stubByte = *grabber;
	if(isLittleEndian()){//Weirdly, in little endian the order of the data values in the byte is reversed from our intended order. We'll need to manually reverse it.
		hold       = 0;//initialize
		hold      += stubByte & 1;//grab the MessageClass
		stubByte >>= 1;
		hold     <<= 2;//make space for the ControlType
		hold      += stubByte & 3;//grab the ControlType
		stubByte >>= 2;
		hold     <<= 2;//make space for the MessageNumberLength
		hold      += stubByte & 3;//grab the MessageNumberLength
		stubByte >>= 2;
		hold     <<= 2;//make space for the ChatIDLength
		hold      += stubByte & 3;//grab the ChatIDLength
		stubByte >>= 2;
		hold     <<= 1;//make space for the DataFlag
		hold      += stubByte & 1;//grab the DataFlag
		stubByte   = hold;//The byte has been properly ordered
	}
	return stubByte;
}

//Serialize a data message header into a series of bytes
unsigned char* SerializeDataMessageHeader(struct DataMessageHeader dmh){
	unsigned char*            mySerial;
	unsigned int                mySize;
	int                 mySourceIDSize;
	unsigned long long SourceIDConvert;
	unsigned long        ChatIDConvert;
	unsigned long        MsgNumConvert;
	unsigned char           myStubByte;
	
	mySize = getSerializedSizeOfDataMessageHeader(dmh);//get the size so we can malloc it
	if(mySize == 0){
		return NULL;//There was an error in the header
	}

	mySerial = malloc(mySize);
	if(mySerial == NULL){
		return mySerial;//Malloc failed. We're forced to bail.
	}

	//pack all the variable integers to copy
	mySourceIDSize  = getSizeOfLVIToPack(dmh.SourceID);
	SourceIDConvert = packLogVarInt(dmh.SourceID);
	ChatIDConvert   = packDirVarInt(dmh.ChatID);
	MsgNumConvert   = packDirVarInt(dmh.MessageNumber);
	myStubByte      = SerializeMessageStub(dmh.Stub);

	//now mem copy all our packed values into our new byte stream
	memcpy(mySerial                                         , &myStubByte     , 1                               );
	memcpy(mySerial+1                                       , &ChatIDConvert  , dmh.Stub.ChatIDLength + 1       );
	memcpy(mySerial+1+dmh.Stub.ChatIDLength+1               , &SourceIDConvert, mySourceIDSize                  );
	memcpy(mySerial+1+dmh.Stub.ChatIDLength+1+mySourceIDSize, &MsgNumConvert  , dmh.Stub.MessageNumberLength + 1);

	return mySerial;
}

//Serialize a control message header into a series of bytes
unsigned char* SerializeControlMessageHeader(struct ControlMessageHeader cmh){
	unsigned char*            mySerial;
	unsigned int                mySize;
	int                 mySourceIDSize;
	unsigned long long SourceIDConvert;
	unsigned long        ChatIDConvert;
	unsigned long        MsgNumConvert;
	unsigned char           myStubByte;
	unsigned long            myVersion;

	//Works the same as above, but adds the version
	mySize = getSerializedSizeOfControlMessageHeader(cmh);
	if(mySize == 0){
		return NULL;//There was an error in the header
	}

	mySerial = malloc(mySize);
	if(mySerial == NULL){
		return mySerial;//Malloc failed. We're forced to bail.
	}

	mySourceIDSize  = getSizeOfLVIToPack(cmh.SourceID);
	SourceIDConvert = packLogVarInt(cmh.SourceID);
	ChatIDConvert   = packDirVarInt(cmh.ChatID);
	MsgNumConvert   = packDirVarInt(cmh.MessageNumber);
	myStubByte      = SerializeMessageStub(cmh.Stub);
	myVersion       = cmh.Version;
	//like with all the others, we'll need to reverse the version value to properly serialize.
	if(isLittleEndian()){
		myVersion = __builtin_bswap32(myVersion);
	}

	memcpy(mySerial                                         , &myStubByte     , 1                               );
	memcpy(mySerial+1                                       , &myVersion      , 4                               );
	memcpy(mySerial+5                                       , &ChatIDConvert  , cmh.Stub.ChatIDLength + 1       );
	memcpy(mySerial+5+cmh.Stub.ChatIDLength+1               , &SourceIDConvert, mySourceIDSize                  );
	memcpy(mySerial+5+cmh.Stub.ChatIDLength+1+mySourceIDSize, &MsgNumConvert  , cmh.Stub.MessageNumberLength + 1);

	return mySerial;
}

//get whether it's a data message or control message, which is determined by the first bit
int getMessageClass(unsigned char* serialized){
	return *serialized >> 7;
}

//rebuild the message stub from the first byte of a stream
struct MessageStub rebuildMessageStub(unsigned char* serialized){
	struct MessageStub Stub;
	
	Stub.MessageClass        = *serialized >> 7;
	Stub.ControlType         = (*serialized >> 5) & 3;
	Stub.MessageNumberLength = (*serialized >> 3) & 3;
	Stub.ChatIDLength        = (*serialized >> 1) & 3;
	Stub.DataFlag            = *serialized & 1;

	return Stub;
}

//rebuild the data message header from a byte stream
struct DataMessageHeader rebuildDataMessage(unsigned char* serialized){
	struct DataMessageHeader dmh;
	unsigned char*           loc;
	int                      inc;
	
	loc = serialized;
	
	dmh.Stub = rebuildMessageStub(loc);
	loc += 1;
	
	dmh.ChatID = unpackDirVarInt(loc,dmh.Stub.ChatIDLength+1);
	loc += dmh.Stub.ChatIDLength+1;

	dmh.SourceID = unpackLogVarInt(loc);
	inc = getSizeOfLVIToPack(dmh.SourceID);
	loc += inc;

	dmh.MessageNumber = unpackDirVarInt(loc,dmh.Stub.MessageNumberLength+1);

	return dmh;
}

//rebuild the control message header from a byte stream
struct ControlMessageHeader rebuildControlMessage(unsigned char* serialized){
	struct ControlMessageHeader cmh;
	unsigned char*              loc;
	int                         inc;

	loc = serialized;

	cmh.Stub = rebuildMessageStub(loc);
	loc += 1;

	cmh.Version   =       *loc;
	cmh.Version <<=          8;
	cmh.Version  += *(loc + 1);
	cmh.Version <<=          8;
	cmh.Version  += *(loc + 2);
	cmh.Version <<=          8;
	cmh.Version  += *(loc + 3);
	loc += 4;

	cmh.ChatID = unpackDirVarInt(loc,cmh.Stub.ChatIDLength+1);
	loc += cmh.Stub.ChatIDLength+1;

	cmh.SourceID = unpackLogVarInt(loc);
	inc = getSizeOfLVIToPack(cmh.SourceID);
	loc += inc;

	cmh.MessageNumber = unpackDirVarInt(loc,cmh.Stub.MessageNumberLength+1);

	return cmh;
}

//Create a data message header with the given information
struct DataMessageHeader constructDataMessageHeader(unsigned long ChatID, unsigned long long SourceID, unsigned long MessageNumber){
	struct DataMessageHeader dmh;

	dmh.Stub.MessageClass        = 0;
	dmh.Stub.ControlType         = 0;
	dmh.Stub.MessageNumberLength = getSizeOfDVIToPack(MessageNumber)-1;
	dmh.Stub.ChatIDLength        = getSizeOfDVIToPack(ChatID)-1;
	dmh.Stub.DataFlag            = 0;
	dmh.ChatID                   = ChatID;
	dmh.SourceID                 = SourceID;
	dmh.MessageNumber            = MessageNumber;

	return dmh;
}

//Create a control message header with the given information
struct ControlMessageHeader constructControlMessageHeader(char controlType, unsigned long Version, unsigned long ChatID, unsigned long long SourceID, unsigned long MessageNumber, int hasData){
	struct ControlMessageHeader cmh;

	cmh.Stub.MessageClass        = 1;
	cmh.Stub.ControlType         = controlType & 3;
	cmh.Stub.MessageNumberLength = getSizeOfDVIToPack(MessageNumber)-1;
	cmh.Stub.ChatIDLength        = getSizeOfDVIToPack(ChatID)-1;
	cmh.Stub.DataFlag            = hasData ? 1 : 0;
	cmh.Version                  = Version;
	cmh.ChatID                   = ChatID;
	cmh.SourceID                 = SourceID;
	cmh.MessageNumber            = MessageNumber;

	return cmh;
}
