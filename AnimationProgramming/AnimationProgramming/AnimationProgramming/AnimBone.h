#pragma once

#include "src/Transform.h"

#include <string>

class AnimBone
{
public :
	std::string name = "";
	int parentIndex = 0;
	int index = 0;
};
