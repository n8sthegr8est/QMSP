#include "chatWriter.h"
#include "MessageReader.h"
#include "Messages.h"
#include "Frames.h"
#include <stdlib.h>
#include <stdio.h>

int main(void){
	struct DataMessageHeader     MsgHead;
	struct TransitMetadataFrame  TrMdtaF;
	unsigned char*               mySeril;

	struct PlaintextFrame* ptxtF;
	
	void** myFrames;

	struct Message reMessage;

	MsgHead.Stub.MessageClass        = 0;
	MsgHead.Stub.ControlType         = 0;
	MsgHead.Stub.MessageNumberLength = 0;
	MsgHead.Stub.ChatIDLength        = 0;
	MsgHead.Stub.DataFlag            = 0;

	MsgHead.ChatID        = 0x02;
	MsgHead.SourceID      = 0x3C;
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

	myFrames = malloc(sizeof(char*) * 2);
	myFrames[0] = &TrMdtaF;
	myFrames[1] = ptxtF;

	mySeril = SerializeDataMessageHeader(MsgHead);
	mySeril = PackMessage(mySeril, myFrames, 4, 2);
	free(ptxtF);

	reMessage = unpackMessage(mySeril, 15);
	free(mySeril);
	printf("chatID: 0x%X\n",((struct DataMessageHeader*) reMessage.msghead)->ChatID);

	printf("Adding new post to test chat\n");
	//addNewUser(2, 60, 0, "blah");
	applyMessage(reMessage);
	printf("new user added\n");
	free(reMessage.msghead);
	free(reMessage.msgframes[0]);
	free(reMessage.msgframes[1]);
	free(reMessage.msgframes);

	//printf("Adding new post to test chat\n");
	//addNewPost(2, 60, 1780709425, "Hello, fellow users!");
	//printf("new post added\n");
}
