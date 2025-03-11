#pragma once

struct Transform
{
private:
	
	vec3 pos = {0,0,0};
	quat rot = quat({0,0,0});
	vec3 scale = {1,1,1};

	bool ModelMatrixDirty = true;
	mat4 ModelMatrix;
public:
	explicit Transform()

	{
	}

	const mat4& GetModelMatrix()
	{
		if (ModelMatrixDirty)
		{
			mat4 T = translate(identity<mat4>(), pos);
			mat4 R(rot);
			mat4 S = glm::scale(identity<mat4>(), scale);
			ModelMatrix = T * R * S;
			ModelMatrixDirty = false;
		}
		return ModelMatrix;
		
	}

	constexpr bool isDirty() const { return ModelMatrixDirty; }


	const decltype(pos)& Position() const {return pos;}
	decltype(pos)& Position() {ModelMatrixDirty = true; return pos;}

	const decltype(rot)& Rotation() const {return rot;}
	decltype(rot)& Rotation() {ModelMatrixDirty = true; return rot;}

	const decltype(scale)& Scale() const {return scale;}
	decltype(scale)& Scale() {ModelMatrixDirty = true; return scale;}

	
};