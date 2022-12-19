#include "Transform.h"

#include "MathsLib/EmMaths.hpp"

Transform Transform::Lerp(const Transform& a, const Transform& b, const float& time)
{
    Transform result;

    result.position = EmMaths::Float3::Lerp(a.position, b.position, time);
    result.rotation = EmMaths::Quaternion::NLerp(a.rotation, b.rotation, time);
    result.scale = EmMaths::Float3::Lerp(a.scale, b.scale, time);

    return result;
}
