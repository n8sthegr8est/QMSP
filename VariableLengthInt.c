#include "VariableLengthInt.h"

//expand a packed "logarithmic variable integer" from a series of bytes
//A "logarithmic variable integer" is one which the first two bits of the initial byte are treated as the number of bytes to read to get the actual value in the form of a base 2 logarithm
uint64_t unpackLogVarInt(unsigned char* bytes){
	unsigned char* firstByteCheck;
	uint64_t             numValue;
	int                   numSize;
	uint8_t          firstByteVal;

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
uint32_t unpackDirVarInt(unsigned char* bytes, int length){
	uint32_t numVal;

	numVal = *bytes;
	for(int i = 1; i < length; i++){//given the length, read all the relevant bytes
		numVal <<= 8;//shift everything a byte to the left to make space for the next one
		numVal += *(bytes + i);//now add the next one in
	}

	return numVal;
}

//gets the serialized size of a "logarithmic variable integer" when it is packed
int getSizeOfLVIToPack(uint64_t lvi){
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
int getSizeOfDVIToPack(uint32_t dvi){
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
uint64_t packLogVarInt(uint64_t value){
	int       lviSize;
	uint64_t codified;
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
		codified = rev_64(codified);
	}

	return codified;
}

//packs a "direct variable integer" into a shortened serialized form.
uint32_t packDirVarInt(uint32_t value){
	int      dviSize;
	uint32_t  packed;

	dviSize = getSizeOfDVIToPack(value);
	packed = value << ((4 - dviSize) * 8);//shift the value to the most significant bits because it will copy from there first
	if(isLittleEndian()){//We need to reverse the bytes to properly serialize if we're little endian
		packed = rev_32(packed);
	}
	
	return packed;
}
