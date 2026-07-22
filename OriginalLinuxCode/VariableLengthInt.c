#include "VariableLengthInt.h"

int   littleEndianVal =  -1;

//expand a packed "logarithmic variable integer" from a series of bytes
//A "logarithmic variable integer" is one which the first two bits of the initial byte are treated as the number of bytes to read to get the actual value in the form of a base 2 logarithm
unsigned long long unpackLogVarInt(unsigned char* bytes){
	unsigned char*     firstByteCheck;
	unsigned long long       numValue;
	int                       numSize;
	char                 firstByteVal;

	firstByteCheck = bytes;
	numSize = 1 << (*firstByteCheck >> 6);//Calculate the number of bytes we need to read to unpack this. It's equal to 2^i where i is the value of the first two bits.
	firstByteVal = *firstByteCheck & 63;//lop off the first two bits from the actual unpacked value
	numValue = firstByteVal;
	for(int i = 1; i < numSize; i++){//Now that we know the length, read the rest of the bytes
		numValue <<= 8;//shift everything a byte to the left to make space for the next one
		numValue += *(firstByteCheck + i);//now add the next one in
	}
	
	return numValue;
}

//expand a packed "direct variable integer" from a series of bytes
//A "direct variable integer" is one which the number of bytes to read is flagged somewhere else within the message
unsigned long unpackDirVarInt(unsigned char* bytes, int length){
	unsigned long numVal;

	numVal = *bytes;
	for(int i = 1; i < length; i++){//given the length, read all the relevant bytes
		numVal <<= 8;//shift everything a byte to the left to make space for the next one
		numVal += *(bytes + i);//now add the next one in
	}

	return numVal;
}

//gets the serialized size of a "logarithmic variable integer" when it is packed
int getSizeOfLVIToPack(unsigned long long lvi){
	if(lvi >> 6){//if it's more than 6 bits long
		if(lvi >> 14){//if it's more than 14 bits long
			if(lvi >> 30){//if it's more than 30 bits long
				return 8;//We need 8 bytes
			}
			return 4;//otherwise, we need 4 bytes
		}
		return 2;//otherwise we need 2 bytes
	}
	return 1;//otherwise we need only 1 byte
}

//gets the serialized size of a "direct variable integer" when it is packed
int getSizeOfDVIToPack(unsigned long dvi){
	if(dvi >> 8){//if it's more than 1 byte
		if(dvi >> 16){//if it's more than 2 bytes
			if(dvi >> 24){//if it's more than 3 bytes
				return 4;//we need 4 bytes
			}
			return 3;//otherwise we need 3 bytes
		}
		return 2;//otherwise we need 2 bytes
	}
	return 1;//otherwise we need only 1 byte
}

//pack a "logarithmic variable integer" into a shortened serialized form.
unsigned long long packLogVarInt(unsigned long long value){
	int                 lviSize;
	unsigned long long codified;
	if(value == 0){//it's already packed, and it's hard to check for later.
		return value;
	}

	if(value >> 62){
		return 0;//Value is too high to pack. return this as an error.
	}

	lviSize = getSizeOfLVIToPack(value);//get the size we need

	if(lviSize > 4){
		codified = 3ULL << 62;//set it up for a length of 8
	}
	else if(lviSize > 2){
		codified = 2ULL << 30;//set it up for a length of 4
	}
	else if(lviSize > 1){
		codified = 1ULL << 14;//set it up for a length of 2
	}
	else{
		codified = 0;//no setup needed.
	}
	codified += value;//add the rest of the value to the packed int

	while(!(codified >> 62) && !(codified >> 56)){
		codified <<= 8;
	}//move the value to the front because it will copy from there first.
	
	if(isLittleEndian()){//if we're little endian, we need to reverse the bytes to properly serialize the value
		codified = __builtin_bswap64(codified);
	}

	return codified;
}

//packs a "direct variable integer" into a shortened serialized form.
unsigned long packDirVarInt(unsigned long value){
	int dviSize;
	int  packed;

	dviSize = getSizeOfDVIToPack(value);
	packed = value << ((4 - dviSize) * 8);//shift the value to the most significant bits because it will copy from there first
	if(isLittleEndian()){//We need to reverse the bytes to properly serialize if we're little endian
		packed = __builtin_bswap32(packed);
	}
	
	return packed;
}

//Checks and sets whether the system is big or little endian, as it is important for serialization
int isLittleEndian(){
	if(littleEndianVal < 0){//if we haven't checked yet
		char*  myVal;
		int   holder;

		holder = 1;
		myVal = (char*) &holder;//set a char pointer to the start of holder
	
		littleEndianVal = *myVal;//if it's 0, we're big endian. If it's 1, we're little endian.
	}
	return littleEndianVal;//Since we know, we can return 0 (false) if we're not little endian and 1 (true) if we are
}
