#include "Messages.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	struct DataMessageHeader myDtaMessage;
	unsigned char*           mySerialDMsg;	
	//unsigned int             mySerialSize;
	struct DataMessageHeader reDtaMessage;

	struct ControlMessageHeader myCtlMessage;
	unsigned char*              mySerialCMsg;
	struct ControlMessageHeader reCtlMessage;

	myDtaMessage.Stub.MessageClass        = 0;
	myDtaMessage.Stub.ControlType         = 0;
	myDtaMessage.Stub.MessageNumberLength = 1;
	myDtaMessage.Stub.ChatIDLength        = 1;
	myDtaMessage.Stub.DataFlag            = 0;

	myCtlMessage.Stub.MessageClass        = 1;
	myCtlMessage.Stub.ControlType         = 2;
	myCtlMessage.Stub.MessageNumberLength = 0;
	myCtlMessage.Stub.ChatIDLength        = 0;
	myCtlMessage.Stub.DataFlag            = 0;

	myDtaMessage.ChatID        =     0x3F43;
	myDtaMessage.SourceID      = 0x1203524F;
	myDtaMessage.MessageNumber =     0x2132;
	
	myCtlMessage.Version       =       1;
	myCtlMessage.ChatID        =      17;
	myCtlMessage.SourceID      = 0x3524F;
	myCtlMessage.MessageNumber =       3;
	
	//mySerialSize = getSerializedSizeOfDataMessageHeader(myDtaMessage);
	mySerialDMsg = SerializeDataMessageHeader(myDtaMessage);
	mySerialCMsg = SerializeControlMessageHeader(myCtlMessage);

	printf("Serial Bytes Test:\n");
	printf("Expected Data Serial: 0xA 0x3F 0x43 0x92 0x3 0x52 0x4F 0x21 0x32\n");
	printf("Actual   Data Serial: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",(unsigned char) *mySerialDMsg,(unsigned char) *(mySerialDMsg+1), (unsigned char) *(mySerialDMsg+2),(unsigned char) *(mySerialDMsg+3),(unsigned char) *(mySerialDMsg+4), (unsigned char) *(mySerialDMsg+5),(unsigned char) *(mySerialDMsg+6),(unsigned char) *(mySerialDMsg+7),(unsigned char) *(mySerialDMsg+8));
	printf("Expected Cntl Serial: 0xC0 0x0 0x0 0x0 0x1 0x11 0x80 0x3 0x52 0x4F 0x3\n");
	printf("Actual   Cntl Serial: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",(unsigned char) *mySerialCMsg,(unsigned char) *(mySerialCMsg+1), (unsigned char) *(mySerialCMsg+2),(unsigned char) *(mySerialCMsg+3),(unsigned char) *(mySerialCMsg+4), (unsigned char) *(mySerialCMsg+5),(unsigned char) *(mySerialCMsg+6),(unsigned char) *(mySerialCMsg+7),(unsigned char) *(mySerialCMsg+8),(unsigned char) *(mySerialCMsg+9),(unsigned char) *(mySerialCMsg+10));

	reDtaMessage = rebuildDataMessage(mySerialDMsg);
	free(mySerialDMsg);
	reCtlMessage = rebuildControlMessage(mySerialCMsg);
	free(mySerialCMsg);

	printf("\nDM Reconstruction Test:\n");
	printf("MessageStub:\n");
	printf("class: e0 a%i\n",reDtaMessage.Stub.MessageClass);
	printf("ctype: e0 a%i\n",reDtaMessage.Stub.ControlType);
	printf("mnlen: e1 a%i\n",reDtaMessage.Stub.MessageNumberLength);
	printf("cidln: e1 a%i\n",reDtaMessage.Stub.ChatIDLength);
	printf("dflag: e0 a%i\n",reDtaMessage.Stub.DataFlag);
	printf("Expected ChatID: 0x3F43\n");
	printf("Actual   ChatID: 0x%lX\n",reDtaMessage.ChatID);
	printf("Expected SourceID: 0x1203524F\n");
	printf("Actual   SourceID: 0x%llX\n",reDtaMessage.SourceID);
	printf("Expected MNum: 0x2132\n");
	printf("Actual   MNum: 0x%lX\n",reDtaMessage.MessageNumber);
	
	printf("\nCM Reconstruction Test:\n");
	printf("MessageStub:\n");
	printf("class: e1 a%i\n",reCtlMessage.Stub.MessageClass);
	printf("ctype: e2 a%i\n",reCtlMessage.Stub.ControlType);
	printf("mnlen: e0 a%i\n",reCtlMessage.Stub.MessageNumberLength);
	printf("cidln: e0 a%i\n",reCtlMessage.Stub.ChatIDLength);
	printf("dflag: e0 a%i\n",reCtlMessage.Stub.DataFlag);
	printf("Expected Version: 0x1\n");
	printf("Actual   Version: 0x%lX\n",reCtlMessage.Version);
	printf("Expected ChatID: 0x11\n");
	printf("Actual   ChatID: 0x%lX\n",reCtlMessage.ChatID);
	printf("Expected SourceID: 0x3524F\n");
	printf("Actual   SourceID: 0x%llX\n",reCtlMessage.SourceID);
	printf("Expected MNum: 0x3\n");
	printf("Actual   MNum: 0x%lX\n",reCtlMessage.MessageNumber);
}
