#include <iostream>
#include <map>
#include <queue>
#include <math.h>

#include "src/Vector2.h"

typedef struct {
	float x, y;
} vector2;

typedef struct {
	Vector2<int> v1, v2, v3;
	int color;
} Triangle;