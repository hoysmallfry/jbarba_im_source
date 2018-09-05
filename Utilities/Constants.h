#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "SqueakyCleanWindows.h"

namespace Constants
{
	//The max size a persons name can be.
	const unsigned int nameSize = 20;
	//max amount of users that can log on.
	const unsigned int usersMax = 112;
	//max size a message can be. About three paragraphs.
	const unsigned int messageSize = 251; //2100;
	//The maximum size a user can send to others.
	const unsigned int dataSize = 2*nameSize + sizeof(int) + messageSize;
	//total size a message can carry.
	const unsigned int dataPackSize = nameSize + sizeof(int) + dataSize;

	const unsigned screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const unsigned screenHeight = GetSystemMetrics(SM_CYSCREEN);

	const unsigned centerX = screenWidth/2;
	const unsigned centerY = screenHeight/2;
}

#endif
