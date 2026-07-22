I just want to preface this with an apology. I swear to you I did everything I could, but I just didn't have enough time to get everything working, and I'm not even sure it connects properly, and I can't figure out what's going wrong with it. I also didn't have time to implement my DFA. Unfortunately, other school work got in the way heavily. I am sorry. This is what I was able to complete.


In order to run the actual program, you can simply unzip the folder, keeping everything in the same location it already is, and simply type make. You can then run ./QUICchat to try running the program. If you get an error on the first attempt, enter the command "export LD_LIBRARY_PATH=./msquiclib/lib" and try again.


You can also build my many different testing files to verify the correctness of many of my code's functions:

make testVarInts      - ./TestVarInts - To test the packing and unpacking of Logarithmic Variable Integers and Direct Variable Integers
make testMsgSerial    - ./TestMessageSerial - To test the serialization of message headers
make testFrameSerial  - ./TestFrameSerial - To test the serialization of Transit Metadata Frames
make testMessagePack  - ./TestMessagePack - To test the serialization of entire messages
make testChatRead     - ./TestChatRead - To test the reading of information from a .chat file in the localchats folder
make testChatWrite    - ./TestChatWrite - To test the writing of information to a .chat file in the localchats folder. Check the file
					   "testingChat.chat" before and after running the test to see the results
make testChatCache    - ./TestChatCache - To test the reading and writing of chat connection information to the conmem.log file in the
					   conDir directory. Observe the file's contents before and after running to see results
make testMessagePackVarPTF - ./TestMessagePack - To test the packing of a message with a Plaintext Frame without a length parameter.
make testMessageApply - ./TestMessageApply - To test the application of messages to chat files. Observe "testingChat.chat" before and after
					      running to test results.
make testMessageHeaderConstruct - ./TestMessageHeaderConstruct - To test the construction of message headers.

To clean after testing, simply run "make cleanTests". To clean after compiling the program, use "make clean".