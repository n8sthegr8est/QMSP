#include "QMSPkernel.h"

//I unfortunately did not have time to complete this, and it would have needed a severe overhaul.

ProtocolState currentState = STATE_CLOSED;

int pleaseFetchPermissions = 0;
int pleaseGiveAmassAmount  = 0;

unsigned long long SourceID = 0;
unsigned long protocolVersion = 1;

unsigned long supportedAltVersions[1] = {0};
unsigned long usingVersion = 1;

char PermissionsByte = 255;

unsigned long long AmassTotal  = 0;
unsigned long long AmassExpect = 0;

int PSendAtOnceLimit = 50;
int PSentInLastLoad  = 50;

char KickReason = 0;

void receivedVersionNegotiationResponse(unsigned char header, unsigned long versionNumber){
	if(currentState == STATE_VERSION_NEGOTIATE_AWAIT){
		if(header >> 5 == 7){//version negotiation messages have the first three bits all set to 1. if this is not 7, then we got a data message or a different control message type.
			for(int i = 0; i < (sizeof(supportedAltVersions)/sizeof(unsigned long)); i++){
				if(supportedAltVersions[i] == versionNumber){
					currentState = STATE_CHECK_JOIN_PERM;
					usingVersion = versionNumber;
					return;
				}
			}
			currentState = STATE_CLOSED;
			KickReason = 2;
			//Kick with status 2
			//currentState = STATE_CHECK_JOIN_PERM;
		}
	}
}

void receivedMessage(unsigned char header, unsigned long version, unsigned long ChatID, unsigned long long SID, unsigned long MessageNum, unsigned int FrameCount, unsigned long long FrameTypes[]){
	if(currentState == STATE_CLOSED){
		if(header >> 5 != 4){
			return;
		}
		for(int i = 0; i < FrameCount; i++){
			if(FrameTypes[i] == 0x0c){
				currentState = STATE_CHECK_JOIN_PERM;
				SourceID = SID;
				pleaseFetchPermissions = 1;
				usingVersion = protocolVersion;
				break;
			}
		}
	}
	if(currentState == STATE_CHECK_JOIN_PERM){
		if(version != protocolVersion){
			currentState = STATE_VERSION_NEGOTIATE_ALERT;
		}
	}
	//if(currentState == STATE_VERSION_NEGOTIATE_AWAIT){
	//	if(header >> 5 == 7){//version negotiation messages have the first three bits all set to 1. if this is not 7, then we got a data message or a different control message type.
	//		currentState = STATE_CHECK_JOIN_PERM;
	//	}
	//}
	if(currentState == STATE_IDLE){
		if(header >> 5 == 5){
			for(int i = 0; i < FrameCount; i++){
				if(FrameTypes[i] == 0x09){
					currentState = STATE_CHECK_CHANGE_PERM;
				}
			}
		}
		else if(header >> 5 == 4){
			for(int i = 0; i < FrameCount; i++){
				if(FrameTypes[i] == 0x0E){
					currentState = STATE_CLOSED;
				}
			}
		}
		else{
			for(int i = 0; i < FrameCount; i++){
				if(FrameTypes[i] == 0x07){
					currentState = STATE_CHECK_READ_PERM;
					break;
				}
				else{
					currentState = STATE_CHECK_POST_PERM;
				}
			}
		}
		//else if(it was a DataMessage without a Load Frame){
		//	currentState = STATE_CHECK_POST_PERM;
		//}
		//else if(it was a ConnectionEstablishmentMessage with a Logout Frame){
		//	currentState = STATE_CLOSED;
		//}
	}
	if(currentState == STATE_CHECK_READ_PERM){
		if(PermissionsByte & 2){
			currentState = STATE_RETRIEVE_POSTS;
			PSentInLastLoad = 0;
		}
		else{
			currentState = STATE_IDLE;
		}
	}
	if(currentState == STATE_CHECK_POST_PERM){
		if(PermissionsByte & 4){
			currentState = STATE_AMASS_POST;
			//AmassExpect = number of messages in transmission
			AmassTotal = 0;
		}
		else{
			currentState = STATE_IDLE;
		}
	}
	if(currentState == STATE_AMASS_POST){
		//if(it contains a transit metadata frame){
			//AmassTotal++;
			if(AmassTotal == AmassExpect){
				currentState = STATE_ASSEMBLE_POST;
			}
			//else if(AmassTotal < AmassExpect && this is the last message in the ){
				currentState = STATE_REQUEST_RETRANSMIT;
			//}
		//}
	}
}

void sentMessage(char header, int hadRetransmitFrame, int OutOfMessagesToLoad){
	if(currentState == STATE_VERSION_NEGOTIATE_ALERT){
		if(header >> 5 == 7){
			currentState = STATE_VERSION_NEGOTIATE_AWAIT;
		}
	}
	if(currentState == STATE_NEGOTIATE_OPTIONS){
		if(header >> 5 == 4){
			currentState = STATE_CHECK_READ_PERM;
		}
	}
	if(currentState == STATE_CHECK_READ_PERM){
		if(PermissionsByte & 2){
			currentState = STATE_RETRIEVE_POSTS;
		}
		else{
			currentState = STATE_IDLE;
		}
	}
	if(currentState == STATE_RETRIEVE_POSTS){
		PSentInLastLoad++;
		if(PSentInLastLoad == PSendAtOnceLimit || OutOfMessagesToLoad){
			currentState = STATE_IDLE;
		}
	}
	if(currentState == STATE_REQUEST_RETRANSMIT){
		if(hadRetransmitFrame){
			currentState = STATE_AMASS_POST;
		}
	}
}

void givePermissions(char Permissions){
    if(currentState == STATE_CHECK_JOIN_PERM){
        if(pleaseFetchPermissions){
            pleaseFetchPermissions = 0;
            PermissionsByte = Permissions;
        }
        if(!(PermissionsByte & 1)){
            currentState = STATE_CLOSED;
	    kickReason = 3;
            //kick with status 3
        }
        else{
            currentState = STATE_NEGOTIATE_OPTIONS;
        }
    }
}

void noStoredPermissions(){
    if(currentState == STATE_CHECK_JOIN_PERM){
        if(pleaseFetchPermissions){
            pleaseFetchPermissions = 0;
            PermissionsByte = 231;
            currentState = STATE_NEGOTIATE_OPTIONS;
        }
    }
}

void optionsError(){
    if(currentState == STATE_NEGOTIATE_OPTIONS){
        currentState = STATE_CLOSED;
	kickReason = 2;
        //kick with status 2
    }
}

void wantsToChangePermissions(unsigned long long uid){
    if(PermissionsByte & 128){
        currentState = STATE_APPROVE_CHANGES;
    }
    currentState = STATE_DENY_CHANGES;
}

void wantsToChangeUser(unsigned long long uid){
    if(uid == SourceID){
        if(PermissionsByte & 32){
            currentState = STATE_APPROVE_CHANGES;
        }
	else{
            currentState = STATE_DENY_CHANGES;
	}
    }
    else if(PermissionsByte & 64){
        currentState = STATE_APPROVE_CHANGES;
    }
    else{
        currentState = STATE_DENY_CHANGES;
    }
}

void assembledPost(){
    if(currentState == STATE_ASSEMBLE_POST){
        currentState = STATE_IDLE;
    }
}

void giveUpConnection(){
    if(currentState == STATE_CHECK_JOIN_PERM){
        currentState = STATE_CLOSED;
	kickReason = 4;
        //kick with status 4
    }
}

void imaginaryChatAccess(){
	if(currentState == STATE_CHECK_JOIN_PERM){
		currentState = STATE_CLOSED;
		kickReason = 255;
	}
}
