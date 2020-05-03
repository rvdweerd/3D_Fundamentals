#pragma once

#include <vector>
#include "Vec3.h"

struct IndextedTriangleList
{
	std::vector<Vec3> vertices;
	std::vector<size_t> indices;
};