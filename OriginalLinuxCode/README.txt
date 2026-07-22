Bug Notice - Code does not properly establish connection at present. Will be fixed in the future.

Additionally, code was written for Linux OS and assumes GCC is the compiler. May not work properly on other OS or compilers.


In order to run the program, simply type make. You can then run ./QUICchat to try running the program. If you get an error on the first attempt, enter the command "export LD_LIBRARY_PATH=./msquiclib/lib" and try again.


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