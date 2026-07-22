#include "Frames.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	struct TransitMetadataFrame   myTMD;
	unsigned char*               serial;
	struct TransitMetadataFrame*  reTMD;

	myTMD.type = 0;
	myTMD.PostTransitID = 0;
	myTMD.TotalMessageCount = 1;
	myTMD.SpecificMessageCount = 1;

	//serial = SerializeFrame(&myTMD,getSerializedSizeOfFrame(&myTMD,myTMD.type));

	serial = SerializeFrame(&myTMD,myTMD.type);
	//serial = SerializeTMD(&myTMD);

	printf("Serialization Tests:\n");
	printf("Expected Bytes: 0x0 0x0 0x0 0x0 0x0 0x1 0x1\n");
	printf("Actual   Bytes: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",(unsigned char) *serial,(unsigned char) *(serial+1),(unsigned char) *(serial+2),(unsigned char) *(serial+3),(unsigned char) *(serial+4),(unsigned char) *(serial+5),(unsigned char) *(serial+6));
	
	reTMD = (struct TransitMetadataFrame*) extractFrames(getSerializedSizeOfFrame(&myTMD,myTMD.type),serial)[0];
	free(serial);

	printf("\nRebuild Tests:\n");
	printf("Type: e0 a%lli\n"  ,reTMD->type);
	printf("TrID: e0 a%lli\n",reTMD->PostTransitID);
	printf("TtMC: e1 a%lli\n",reTMD->TotalMessageCount);
	printf("SpMC: e1 a%lli\n",reTMD->SpecificMessageCount);
	
	free(reTMD);
}
