#include "VariableLengthInt.h"
#include <stdint.h>

#pragma pack(push,1)
struct MessageStub{
	unsigned int MessageClass        : 1;
	unsigned int ControlType         : 2;
	unsigned int MessageNumberLength : 2;
	unsigned int ChatIDLength        : 2;
	unsigned int DataFlag            : 1;
};

struct DataMessageHeader{
	struct   MessageStub          Stub;
	uint32_t                    ChatID;
	uint64_t                  SourceID;
	uint32_t             MessageNumber;
};

struct ControlMessageHeader{
	struct   MessageStub          Stub;
	uint32_t                   Version;
	uint32_t                    ChatID;
	uint64_t                  SourceID;
	uint32_t             MessageNumber;
};
#pragma pack(pop)

unsigned int getSerializedSizeOfDataMessageHeader(struct DataMessageHeader dmh);
unsigned int getSerializedSizeOfControlMessageHeader(struct ControlMessageHeader cmh);

unsigned char* SerializeDataMessageHeader(struct DataMessageHeader dmh);
unsigned char* SerializeControlMessageHeader(struct ControlMessageHeader cmh);

int                               getMessageClass(unsigned char* serialized);
struct DataMessageHeader       rebuildDataMessage(unsigned char* serialized);
struct ControlMessageHeader rebuildControlMessage(unsigned char* serialized);

struct DataMessageHeader constructDataMessageHeader(unsigned long ChatID, unsigned long long SourceID, unsigned long MessageNumber);
struct ControlMessageHeader constructControlMessageHeader(char controlType, unsigned long Version, unsigned long ChatID, unsigned long long SourceID, unsigned long MessageNumber, int hasData);
