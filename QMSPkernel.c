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

ProtocolState currentState = STATE_CLOSED;

int pleaseFetchPermissions = 0;

unsigned long long SourceID = 0;

char PermissionsByte = 255;

int AmassTotal = 0;
int AmassExpect = 0;

void receivedMessage(char header){
    if(currentState == STATE_CLOSED){
        //if(it has a login frame){
        currentState = STATE_CHECK_JOIN_PERM;
        SourceID = 0;//The sourceID
        pleaseFetchPermissions = 1;
        //}
    }
    if(currentState == STATE_CHECK_JOIN_PERM){
        //if(there is a version mismatch){
        currentState = STATE_VERSION_NEGOTIATE_ALERT;
        //}
    }
    if(currentState = STATE_VERSION_NEGOTIATE_AWAIT){
        //if(it was a version negotiation packet){
        currentState = STATE_CHECK_JOIN_PERM;
        //}
    }
    if(currentState = STATE_IDLE){
        //if(it was a UserManagementMessage with a User Datagram Frame){
        currentState = STATE_CHECK_CHANGE_PERM;
        //}
        //else if(it was a DataMessage with a Load Frame){
        currentState = STATE_CHECK_READ_PERM;
        //}
        //else if(it was a DataMessage without a Load Frame){
        currentState = STATE_CHECK_POST_PERM;
        //}
        //else if(it was a ConnectionEstablishmentMessage with a Logout Frame){
        currentState = STATE_CLOSED;
        //}
    }
    if(currentState = STATE_CHECK_POST_PERM){
        if(PermissionsByte & 4){
            currentState = STATE_AMASS_POST;
            //AmassExpect = number of messages in transmission
        }
        else{
            currentState = STATE_IDLE;
        }
    }
    if(currentState = STATE_AMASS_POST){
        //if(it contains a transit metadata frame){
        AmassTotal++;
        if(AmassTotal == AmassExpect){
            currentState = STATE_ASSEMBLE_POST;
        }
        //else if(AmassTotal < AmassExpect && this is the last message in the ){
        currentState = STATE_REQUEST_RETRANSMIT;
        //}
        //}
    }
}

void sentMessage(char header){
    if(currentState == STATE_VERSION_NEGOTIATE_ALERT){
        //if(it was a version negotiation packet){
        currentState = STATE_VERSION_NEGOTIATE_AWAIT;
        //}
    }
    if(currentState == STATE_NEGOTIATE_OPTIONS){
        //if(it was a connection establishment message){
        currentState = STATE_CHECK_READ_PERM;
        //}
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
        //if(we've sent enough previous posts or have no more to send){
        currentState = STATE_IDLE;
        //}
    }
    if(currentState == STATE_REQUEST_RETRANSMIT){
        //if(it contained a Retransmit frame){
        currentState = STATE_AMASS_POST;
        //}
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
        //kick with status 2
    }
}

void wantsToChangePermissions(long long uid){
    if(PermissionsByte & 128){
        currentState = STATE_APPROVE_CHANGES;
    }
    currentState = STATE_DENY_CHANGES;
}

void wantsToChangeUser(long long uid){
    if(uid == SourceID){
        if(PermissionsByte & 32){
            currentState = STATE_APPROVE_CHANGES;
        }
        currentState = STATE_DENY_CHANGES;
    }
    if(PermissionsByte & 64){
        currentState = STATE_APPROVE_CHANGES;
    }
    currentState = STATE_DENY_CHANGES;
}

void assembledPost(){
    if(currentState == STATE_ASSEMBLE_POST){
        currentState = STATE_IDLE;
    }
}

void giveUpConnection(){
    if(currentState == STATE_CHECK_JOIN_PERM){
        currentState = STATE_CLOSED;
        //kick with status 4
    }
}