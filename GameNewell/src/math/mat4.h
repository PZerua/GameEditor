/**
* (c) 2017 Pablo Luis Garc�a. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <cmath>
#include <ostream>

#include <src/math/vec3.h>
#include <src/math/vec4.h>
#include <src/utils/mathutils.h>

namespace math
{

struct mat4
{
	union
	{
		float m[16];
		vec4 column[4];
	};

	mat4() 
	{ 
		reset();
	}
	mat4(float identity)
	{
		reset();

		m[0 + 0 * 4] = identity;
		m[1 + 1 * 4] = identity;
		m[2 + 2 * 4] = identity;
		m[3 + 3 * 4] = identity;
	}

	void reset()
	{
		for (unsigned col = 0; col < 4; col++)
			for (unsigned row = 0; row < 4; row++)
				m[col + row * 4] = 0.0f;
	}

	inline float& operator()(unsigned row, unsigned col) { return m[col + row * 4]; }
	inline float operator()(unsigned row, unsigned col) const { return m[col + row * 4]; }

	void translate(const vec3 &value)
	{
		m[0 + 3 * 4] += value.x;
		m[1 + 3 * 4] += value.y;
		m[2 + 3 * 4] += value.z;
	}

	mat4 setTranslation(const vec3 &pos)
	{
		mat4 mat;

		mat(0, 3) = pos.x;
		mat(1, 3) = pos.y;
		mat(2, 3) = pos.z;

		return mat;
	}

	mat4 setScale(const vec3 &scale)
	{
		mat4 mat;

		mat(0, 0) = scale.x;
		mat(1, 1) = scale.y;
		mat(2, 2) = scale.z;

		return mat;
	}


	mat4 setRotation(float angle, const vec3 &axis)
	{
		mat4 mat;

		float c = cosf(utils::toRadians(angle));
		float s = sinf(utils::toRadians(angle));
		float cmo = 1.0f - c;

		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		mat(0, 0) = c + x * x * cmo;
		mat(1, 0) = y * x * cmo + z * s;
		mat(2, 0) = z * x * cmo - y * s;

		mat(0, 1) = x * y * cmo - z * s;
		mat(1, 1) = c + y * y * cmo;
		mat(2, 1) = z * y * cmo + x * s;

		mat(0, 2) = x * z * cmo + y * s;
		mat(1, 2) = y * z * cmo - x * s;
		mat(2, 2) = c + z * z * cmo;

		return mat;
	}

	static mat4 identity() { return mat4(1.0f); }
	static mat4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ)
	{
		mat4 mat;

		mat(0, 0) = 2.0f / (right - left);
		mat(1, 1) = 2.0f / (top - bottom);
		mat(2, 2) = -2.0f / (farZ - nearZ);
		mat(3, 3) = 1.0f;
		mat(3, 0) = -(right + left) / (right - left);
		mat(3, 1) = -(top + bottom) / (top - bottom);
		mat(3, 2) = -(farZ + nearZ) / (farZ - nearZ);

		return mat;

	}
	static mat4 perspective(float fov, float aspect, float nearZ, float farZ)
	{
		mat4 mat;

		float q = 1.0f / tanf(utils::toRadians(fov) / 2.0f);

		mat(0, 0) = (1.0f / aspect) * q;
		mat(1, 1) = q;
		mat(2, 2) = -(farZ + nearZ) / (farZ - nearZ);
		mat(2, 3) = -(2 * farZ * nearZ) / (farZ - nearZ);
		mat(3, 2) = -1;

		return mat;
	}
};

} // namespace math