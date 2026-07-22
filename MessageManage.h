#include "Messages.h"
#include "Frames.h"

void* pullFirstFrame(struct Message);

void* pullFrame(struct Message, uint16_t);

void* peekFirstFrame(struct Message);

void* peekFrame(struct Message, uint16_t);

void* mergeMessages(struct Message, struct Message);