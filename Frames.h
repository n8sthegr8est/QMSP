#include "VariableLengthInt.h"

#pragma pack(push,1)
struct Frame{
	uint64_t type  ;
	uint8_t  Data[];
};

struct TransitMetadataFrame{
	uint64_t                 type; //0x00
	uint32_t        PostTransitID;
	uint64_t    TotalMessageCount;
	uint64_t SpecificMessageCount;
};


struct LinkageIndex{
	uint8_t  LinkType;
	uint64_t   LinkID;
};

struct PostMetadataFrame{
	uint64_t                 type; //0x01
	uint64_t               PostID;
	uint64_t            Timestamp;
	struct LinkageIndex Linkage[];
};

struct PlaintextFrame{
	uint64_t     type; //0x02..0x03
	uint64_t   Length;
	char     txtdta[];
};

struct FTextFrame{
	uint64_t     type; //0x04..0x05
	uint64_t   Length;
	char     txtdta[];
};

struct FormattingIndex{
	uint8_t         Index;
	uint8_t  SimpleFormat;
	uint8_t ExtraFormat[];
};

struct FormattingFrame{
	uint64_t                                 type; //0x06
	uint8_t                            IndexCount;
	struct FormattingIndex* FormattingIndexList[];
};

struct LoadFrame{
	uint64_t                 type; //0x07
	uint64_t LastSequentialPostID;
	uint32_t  TimestampOfLastPost;
};

struct EncodingNegotiationFrame{
	uint64_t            type; //0x08
	uint8_t  EncodingSupport;
};

struct UserDatagramAspect{
	uint64_t Aspect;
	uint8_t  Data[];
};

struct UserDatagramFrame{
	uint64_t                         type; //0x09
	uint8_t                        Length;
	struct UserDatagramAspect* Datagram[];
};

struct ChangeDenialFrame{
	uint64_t             type; //0x0A
	uint8_t       Permissions;
	uint8_t            Length;
	uint64_t* DeniedChanges[];
};

struct ChangeApprovalFrame{
	uint64_t               type; //0x0B
	uint8_t              Length;
	uint64_t* ApprovedChanges[];
};

struct LoginFrame{
	uint64_t   type; //0x0C
	char     userID;
};

struct ConnectionAcknowledgementFrame{
	uint64_t             type; //0x0D
	uint8_t  ConnectionStatus;
};

struct LogoutFrame{
	uint64_t type; //0x0E
};
#pragma pack(pop)


void** extractFrames(uint64_t byteCount, unsigned char* bytes);

uint64_t getSerializedSizeOfFrame(void* frame, uint64_t type);

unsigned char* SerializeFrame(void* frame, uint64_t frameType);

unsigned char* SerializeFrameInto(void* frame, uint64_t frameType, unsigned char* loc);
