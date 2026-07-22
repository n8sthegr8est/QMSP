CC = gcc
CFLAGS = -Wall -g
TARGET = QUICchat
OBJS = VariableLengthInt.o Messages.o Frames.o chatReader.o chatWriter.o MessageReader.o chatCache.o QMSP_Runner.o
LDFLAGS := -L./msquiclib/lib -lmsquic -lm -lpthread
#ALLOBJS = VariableLengthInt.o Messages.o Frames.o MessageReader.o
#SRVOBJS = chatReader.o chatWriter.o
#CLIOBJS = placeholder.o 

$(TARGET): gencert $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

#dependencies
gencert:
	./certs/gencert.sh

VariableLengthInt.o: VariableLengthInt.c
	$(CC) $(CFLAGS) -c VariableLengthInt.c

Messages.o: Messages.c
	$(CC) $(CFLAGS) -c Messages.c

Frames.o: Frames.c
	$(CC) $(CFLAGS) -c Frames.c

MessageReader.o: MessageReader.c
	$(CC) $(CFLAGS) -c MessageReader.c

chatCache.o: chatCache.c
	$(CC) $(CFLAGS) -c chatCache.c

chatReader.o: chatReader.c
	$(CC) $(CFLAGS) -c chatReader.c

chatWriter.o: chatWriter.c
	$(CC) $(CFLAGS) -c chatWriter.c

QMSP_Runner.o: QMSP_Runner.c
	$(CC) $(CFLAGS) -c QMSP_Runner.c

#Tests:
testMessageHeaderConstruct: VariableLengthInt.o Messages.o MessageHeadConstructionTest.o
	$(CC) $(CFLAGS) -o TestMessageHeaderConstruct VariableLengthInt.o Messages.o MessageHeadConstructionTest.o

MessageHeadConstructionTest.o: MessageHeadConstructionTest.c
	$(CC) $(CFLAGS) -c MessageHeadConstructionTest.c

testMessageApply: VariableLengthInt.o Messages.o Frames.o chatReader.o chatWriter.o MessageReader.o messagePackReadApplyTest.o
	$(CC) $(CFLAGS) -o TestMessageApply VariableLengthInt.o Messages.o Frames.o chatReader.o chatWriter.o MessageReader.o messagePackReadApplyTest.o

messagePackReadApplyTest.o: messagePackReadApplyTest.c
	$(CC) $(CFLAGS) -c messagePackReadApplyTest.c

testMessagePackVarPTF: VariableLengthInt.o Messages.o Frames.o MessageReader.o MessagePackTest_NoLenPTF.o
	$(CC) $(CFLAGS) -o TestMessagePack VariableLengthInt.o Messages.o Frames.o MessageReader.o MessagePackTest_NoLenPTF.o

MessagePackTest_NoLenPTF.o:
	$(CC) $(CFLAGS) -c MessagePackTest_NoLenPTF.c

testChatCache: chatCache.o chatCacheTest.o
	$(CC) $(CFLAGS) -o TestChatCache chatCache.o chatCacheTest.o

chatCacheTest.o: chatCacheTest.c
	$(CC) $(CFLAGS) -c chatCacheTest.c

testChatWrite: chatReader.o chatWriter.o chatWriteTest.o
	$(CC) $(CFLAGS) -o TestChatWrite chatReader.o chatWriter.o chatWriteTest.o

chatWriteTest.o: chatWriteTest.c
	$(CC) $(CFLAGS) -c chatWriteTest.c

testChatRead: chatReader.o chatReadTest.o
	$(CC) $(CFLAGS) -o TestChatRead chatReader.o chatReadTest.o

chatReadTest.o: chatReadTest.c
	$(CC) $(CFLAGS) -c chatReadTest.c

testMessagePack: VariableLengthInt.o Messages.o Frames.o chatReader.o chatWriter.o MessageReader.o MessagePackTest.o
	$(CC) $(CFLAGS) -o TestMessagePack VariableLengthInt.o Messages.o Frames.o chatReader.o chatWriter.o MessageReader.o MessagePackTest.o

MessagePackTest.o: MessagePackTest.c
	$(CC) $(CFLAGS) -c MessagePackTest.c

testFrameSerial: VariableLengthInt.o Frames.o TMDFrameTest.o
	$(CC) $(CFLAGS) -o TestFrameSerial VariableLengthInt.o Frames.o TMDFrameTest.o

TMDFrameTest.o: TMDFrameTest.c
	$(CC) $(CFLAGS) -c TMDFrameTest.c

testMsgSerial: VariableLengthInt.o Messages.o MessageHeaderSerialTest.o
	$(CC) $(CFLAGS) -o TestMessageSerial VariableLengthInt.o Messages.o MessageHeaderSerialTest.o

MessageHeaderSerialTest.o: MessageHeaderSerialTest.c
	$(CC) $(CFLAGS) -c MessageHeaderSerialTest.c

testVarInts: VariableLengthInt.o VarIntTest.o
	$(CC) $(CFLAGS) -o TestVarInts VariableLengthInt.o VarIntTest.o

VarIntTest.o: VarIntTest.c
	$(CC) $(CFLAGS) -c VarIntTest.c

#clean
cleanTests:
	rm -f TestVarInts TestMessageSerial TestFrameSerial TestMessagePack TestChatRead TestChatWrite TestChatCache TestMessageApply TestMessageHeaderConstruct *.o

clean:
	rm -f $(TARGET) *.o
	rm *.pem
