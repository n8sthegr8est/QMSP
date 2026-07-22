#include "MessageReader.h"
#include "Messages.h"
#include "Frames.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	struct DataMessageHeader     MsgHead;
	struct TransitMetadataFrame  TrMdtaF;
	unsigned char*               mySeril;
	//struct TransitMetadataFrame*     ptr;

	struct PlaintextFrame* ptxtF;
	
	void** myFrames;

	struct Message reMessage;

	MsgHead.Stub.MessageClass        = 0;
	MsgHead.Stub.ControlType         = 0;
	MsgHead.Stub.MessageNumberLength = 0;
	MsgHead.Stub.ChatIDLength        = 0;
	MsgHead.Stub.DataFlag            = 0;

	MsgHead.ChatID        = 0x11;
	MsgHead.SourceID      = 0x21;
	MsgHead.MessageNumber = 0x01;

	TrMdtaF.type                 = 0x00;
	TrMdtaF.PostTransitID        = 0x00;
	TrMdtaF.TotalMessageCount    = 0x01;
	TrMdtaF.SpecificMessageCount = 0x01;

	ptxtF = malloc(sizeof(struct PlaintextFrame()) + 5);
	ptxtF->type   = 0x03;
	ptxtF->Length = 0x05;
	ptxtF->txtdta[0] = 0x48;
	ptxtF->txtdta[1] = 0x65;
	ptxtF->txtdta[2] = 0x6C;
	ptxtF->txtdta[3] = 0x6C;
	ptxtF->txtdta[4] = 0x6F;

	//ptr = &TrMdtaF;
	myFrames = malloc(sizeof(char*) * 2);
	myFrames[0] = &TrMdtaF;
	myFrames[1] = ptxtF;

	mySeril = SerializeDataMessageHeader(MsgHead);
	//mySeril = PackMessage(mySeril, &ptr, 4, 1);
	mySeril = PackMessage(mySeril, myFrames, 4, 2);
	free(ptxtF);

	printf("Pack Test:\n");

	printf("Expected Bytes: 0x0 0x11 0x21 0x1 0x0 0x0 0x0 0x0 0x0 0x1 0x1 0x3 0x5 0x48 0x65 0x6C 0x6C 0x6F\n");
	printf("Actual   Bytes: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",(unsigned char) *mySeril,(unsigned char) *(mySeril+1),(unsigned char) *(mySeril+2),(unsigned char) *(mySeril+3),(unsigned char) *(mySeril+4),(unsigned char) *(mySeril+5),(unsigned char) *(mySeril+6),(unsigned char) *(mySeril+7),(unsigned char) *(mySeril+8),(unsigned char) *(mySeril+9),(unsigned char) *(mySeril+10),(unsigned char) *(mySeril+11),(unsigned char) *(mySeril+12),(unsigned char) *(mySeril+13),(unsigned char) *(mySeril+14),(unsigned char) *(mySeril+15),(unsigned char) *(mySeril+16),(unsigned char) *(mySeril+17));
	//free(mySeril);
	reMessage = unpackMessage(mySeril, 18);
	free(mySeril);

	printf("\nUnpack Test:\n");

	printf("Header:\n");
	printf("Stub:\n");
	printf("Class: e0 a%i\n",((struct DataMessageHeader*) reMessage.msghead)->Stub.MessageClass);
	printf("Ctltp: e0 a%i\n",((struct DataMessageHeader*) reMessage.msghead)->Stub.ControlType);
	printf("MNLen: e0 a%i\n",((struct DataMessageHeader*) reMessage.msghead)->Stub.MessageNumberLength);
	printf("CIDln: e0 a%i\n",((struct DataMessageHeader*) reMessage.msghead)->Stub.ChatIDLength);
	printf("Dflag: e0 a%i\n",((struct DataMessageHeader*) reMessage.msghead)->Stub.DataFlag);
	printf("Expected ChatID: 0x11\n");
	printf("Actual   ChatID: 0x%llX\n",((struct DataMessageHeader*) reMessage.msghead)->ChatID);
	printf("Expected SourceID: 0x21\n");
	printf("Actual   SourceID: 0x%llX\n",((struct DataMessageHeader*) reMessage.msghead)->SourceID);
	printf("Expected MsgNum: 0x1\n");
	printf("Actual   MsgNum: 0x%llX\n",((struct DataMessageHeader*) reMessage.msghead)->MessageNumber);
	printf("Frame 1:\n");
	printf("Expected FrType: 0x0\n");
	printf("Actual   FrType: 0x%llX\n",((struct Frame*) reMessage.msgframes[0])->type);
	printf("Expected TrID: 0x0\n");
	printf("Actual   TrID: 0x%lX\n",((struct TransitMetadataFrame*) reMessage.msgframes[0])->PostTransitID);
	printf("Expected tCount: 0x1\n");
	printf("Actual   tCount: 0x%llX\n",((struct TransitMetadataFrame*) reMessage.msgframes[0])->TotalMessageCount);
	printf("Expected sCount: 0x1\n");
	printf("Actual   sCount: 0x%llX\n",((struct TransitMetadataFrame*) reMessage.msgframes[0])->SpecificMessageCount);
	printf("Frame 2:\n");
	printf("Expected FrType: 0x3\n");
	printf("Actual   FrType: 0x%llX\n",((struct Frame*) reMessage.msgframes[1])->type);
	printf("Expected Length: 0x5\n");
	printf("Actual   Length: 0x%llX\n",((struct PlaintextFrame*) reMessage.msgframes[1])->Length);
	printf("Expected String: Hello\n");
	printf("Actual   String: %c%c%c%c%c\n",((struct PlaintextFrame*) reMessage.msgframes[1])->txtdta[0],((struct PlaintextFrame*) reMessage.msgframes[1])->txtdta[1],((struct PlaintextFrame*) reMessage.msgframes[1])->txtdta[2],((struct PlaintextFrame*) reMessage.msgframes[1])->txtdta[3],((struct PlaintextFrame*) reMessage.msgframes[1])->txtdta[4]);


	free(reMessage.msghead);
	free(reMessage.msgframes[0]);
	free(reMessage.msgframes[1]);
	free(reMessage.msgframes);
}
