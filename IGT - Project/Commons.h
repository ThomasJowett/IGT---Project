#ifndef _COMMONS_H
#define _COMMONS_H

#include<glew.h>
#include <string>
#include <vector>
#include"Vector.h"
#include <windows.h>

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL_1,
	SCREEN_OPTIONS
};

enum FACING
{
	FACING_LEFT = 0,
	FACING_RIGHT
};

struct Vertex
{
	Vector3D mPosition;
	Vector2D mTexCoord;

	Vertex()
	{
		mPosition = Vector3D(0, 0, 0);
		mTexCoord = Vector2D(0, 0);
	}

	Vertex(const Vector3D& position, const Vector2D& texCoord)
		:mPosition(position), mTexCoord(texCoord) {}
};

struct PhysicsMaterial
{
	float drag;
	float elasticity;
	float friction;
	float angularDrag;
};

namespace Util
{
	static std::vector<std::string> SplitString(const std::string &s, char delim)
	{
		std::vector<std::string> elems;

		const char* cstr = s.c_str();
		unsigned int strLength = s.length();
		unsigned int start = 0;
		unsigned int end = 0;

		while (end <= strLength)
		{
			while (end <= strLength)
			{
				if (cstr[end] == delim)
					break;
				end++;
			}

			elems.push_back(s.substr(start, end - start));
			start = end + 1;
			end = start;
		}

		return elems;
	}


};
#endif // !_COMMONS_H