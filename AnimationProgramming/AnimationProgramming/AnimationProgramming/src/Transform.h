#pragma once

#include "../MathsLib/EmMaths.hpp"

class Transform
{
public :
	EmMaths::Float3 position;
	EmMaths::Quaternion rotation;
	EmMaths::Float3 scale;

public:
	Transform(EmMaths::Float3 position_ = 0, EmMaths::Quaternion rotation_ = 0, EmMaths::Float3 scale_ = 0) :
		position(position_),
		rotation(rotation_),
		scale(scale_)
	{
	}

	Transform operator+(const Transform& other)
	{
		return Transform(this->position + other.position, this->rotation + other.rotation, this->scale + other.scale);
	}
};

