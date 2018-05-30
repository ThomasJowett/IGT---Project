#pragma once

namespace Random
{
	static int IntInRange2D(int x, int y, int seed, int min, int max)
	{
		unsigned int hash = (unsigned int)seed;
		hash ^= (unsigned int)x;
		hash *= 0x51d7348d;
		hash ^= 0x85dbdda2;
		hash = (hash << 16) ^ (hash >> 16);
		hash *= 0x7588f287;
		hash ^= (unsigned int)y;
		hash *= 0x487a5559;
		hash ^= 0x64887219;
		hash = (hash << 16) ^ (hash >> 16);
		hash *= 0x63288691;
		return (int)((hash % (max - min))+min);
	}
}