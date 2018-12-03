#pragma once
#include <chrono>

namespace Util
{
	long long Nanoseconds_now() {
		return std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}

	long long Milliseconds_now() {
		return std::chrono::system_clock::now().time_since_epoch().count();
	}
}