typedef enum{
    STATE_CLOSED,
    STATE_CHECK_JOIN_PERM,
    STATE_VERSION_NEGOTIATE_ALERT,
    STATE_VERSION_NEGOTIATE_AWAIT,
    STATE_NEGOTIATE_OPTIONS,
    STATE_CHECK_READ_PERM,
    STATE_RETRIEVE_POSTS,
    STATE_IDLE,
    STATE_CHECK_CHANGE_PERM,
    STATE_DENY_CHANGES,
    STATE_APPROVE_CHANGES,
    STATE_CHECK_POST_PERM,
    STATE_AMASS_POST,
    STATE_REQUEST_RETRANSMIT,
    STATE_ASSEMBLE_POST
} ProtocolState;

//int recVersionNegotiate(unsigned long version); 
void receivedVersionNegotiationResponse(unsigned char header, unsigned long versionNumber);

void receivedMessage(unsigned char header, unsigned long version, unsigned long ChatID, unsigned long long SID, unsigned long MessageNum, unsigned int FrameCount, unsigned long long FrameTypes[]);

void sentMessage(char header, int hadRetransmitFrame, int OutOfMessagesToLoad);

void givePermissions(char Permissions);

void noStoredPermissions();

void optionsError();

void wantsToChangePermissions(unsigned long long uid);

void wantsToChangeUser(unsigned long long uid);

void assembledPost();

void giveUpConnection();

void imaginaryChatAccess();
