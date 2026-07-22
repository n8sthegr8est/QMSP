#include <stdint.h>
#include "VariableLengthInt.h"

#pragma pack(push,1)
struct Frame{
	unsigned long long  type  ;
	uint8_t             Data[];
};

struct TransitMetadataFrame{
	unsigned long long                 type;
	uint32_t                  PostTransitID;
	unsigned long long    TotalMessageCount;
	unsigned long long SpecificMessageCount;
};


struct LinkageIndex{
	uint8_t            LinkType;
	unsigned long long   LinkID;
};

struct PostMetadataFrame{
	unsigned long long       type;
	unsigned long long     PostID;
	uint64_t            Timestamp;
	struct LinkageIndex Linkage[];
};

struct PlaintextFrame{
	unsigned long long     type;
	unsigned long long   Length;
	char               txtdta[];
};

struct FTextFrame{
	unsigned long long     type;
	unsigned long long   Length;
	char               txtdta[];
};

struct FormattingIndex{
	uint8_t         Index;
	uint8_t  SimpleFormat;
	uint8_t ExtraFormat[];
};

struct FormattingFrame{
	unsigned long long                       type;
	uint8_t                            IndexCount;
	struct FormattingIndex* FormattingIndexList[];
};

struct LoadFrame{
	unsigned long long                  type;
	unsigned long long  LastSequentialPostID;
	uint32_t             TimestampOfLastPost;
};

struct EncodingNegotiationFrame{
	unsigned long long            type;
	uint8_t            EncodingSupport;
};

struct UserDatagramAspect{
	unsigned long long Aspect;
	uint8_t            Data[];
};

struct UserDatagramFrame{
	unsigned long long               type;
	uint8_t                        Length;
	struct UserDatagramAspect* Datagram[];
};

struct ChangeDenialFrame{
	unsigned long long             type;
	uint8_t                 Permissions;
	uint8_t                      Length;
	unsigned long long* DeniedChanges[];
};

struct ChangeApprovalFrame{
	unsigned long long               type;
	uint8_t                        Length;
	unsigned long long* ApprovedChanges[];
};

struct LoginFrame{
	unsigned long long     type;
};

struct ConnectionAcknowledgementFrame{
	unsigned long long             type;
	uint8_t            ConnectionStatus;
};

struct LogoutFrame{
	unsigned long long type;
};
#pragma pack(pop)


void** extractFrames(unsigned long long byteCount, unsigned char* bytes);

unsigned long long getSerializedSizeOfFrame(void* frame, unsigned long long type);

unsigned char* SerializeFrame(void* frame, unsigned long long frameType);

unsigned char* SerializeFrameInto(void* frame, unsigned long long frameType, unsigned char* loc);
