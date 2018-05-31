#pragma once
#include <memory>
#include <vector>

#include "iComponents.h"
class test
{
public:
	std::vector<std::unique_ptr<Component>> myComponents;

	test() {}
	~test() {}
};
