#include "Messages.h"
#include <stdio.h>

int main(void){
	struct DataMessageHeader    dmh;
	struct ControlMessageHeader cmh;

	dmh = constructDataMessageHeader(0xF45, 0x34215, 0x43);
	cmh = constructControlMessageHeader(0x00, 0x01, 0x23421, 0x342A3B3, 0x23, 1);

	printf("DMH:\n");
	printf("Stub:\n");
	printf("Class: e0 a%i\n",dmh.Stub.MessageClass);
	printf("cType: e0 a%i\n",dmh.Stub.ControlType);
	printf("mnlen: e0 a%i\n",dmh.Stub.MessageNumberLength);
	printf("cIDln: e1 a%i\n",dmh.Stub.ChatIDLength);
	printf("dFlag: e0 a%i\n",dmh.Stub.DataFlag);
	printf("Expected ChatID: 0xF45\n");
	printf("Actual   ChatID: 0x%lX\n",dmh.ChatID);
	printf("Expected SourceID: 0x34215\n");
	printf("Actual   SourceID: 0x%lX\n",dmh.SourceID);
	printf("Expected MsgNum: 0x43\n");
	printf("Actual   MsgNum: 0x%lX\n",dmh.MessageNumber);
	
	printf("\nCMH:\n");
	printf("Stub:\n");
	printf("Class: e1 a%i\n",cmh.Stub.MessageClass);
	printf("cType: e0 a%i\n",cmh.Stub.ControlType);
	printf("mnlen: e0 a%i\n",cmh.Stub.MessageNumberLength);
	printf("cIDln: e2 a%i\n",cmh.Stub.ChatIDLength);
	printf("dFlag: e1 a%i\n",cmh.Stub.DataFlag);
	printf("Expected Version: 0x1\n");
	printf("Actual   Version: 0x%lX\n",cmh.Version);
	printf("Expected ChatID: 0x23421\n");
	printf("Actual   ChatID: 0x%lX\n",cmh.ChatID);
	printf("Expected SourceID: 0x342A3B3\n");
	printf("Actual   SourceID: 0x%lX\n",cmh.SourceID);
	printf("Expected MsgNum: 0x23\n");
	printf("Actual   MsgNum: 0x%lX\n",cmh.MessageNumber);
}
