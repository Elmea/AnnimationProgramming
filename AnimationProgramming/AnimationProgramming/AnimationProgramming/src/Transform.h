#pragma once

#include "../MathsLib/EmMaths.hpp"

class Transform
{
public :
	EmMaths::Float3 position = { 0, 0, 0 };
	EmMaths::Quaternion rotation;
	EmMaths::Float3 scale = { 1, 1, 1 };

public:

	Transform(){};

	Transform(EmMaths::Float3 position_, EmMaths::Quaternion rotation_ , EmMaths::Float3 scale_) :
		position(position_),
		rotation(rotation_),
		scale(scale_)
	{
	}

	Transform operator+(const Transform& other)
	{
		return Transform(this->position + other.position, this->rotation + other.rotation, this->scale + other.scale);
	}


	EmMaths::Mat4 GetTransformMatrix()
	{
		return EmMaths::Mat4::CreateTransformMatrix(position, rotation, scale);
	}
};

