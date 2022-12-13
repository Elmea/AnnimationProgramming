#pragma once

#include "src/Transform.h"

#include <string>

class AnimBone
{
public :
	std::string name = "";
	int parentIndex = -1; //No parent by default
	int index = 0;
};
