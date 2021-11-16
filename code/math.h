#pragma once

struct Rect {
	vec2 pos;
	float width;
	float height;
};

bool PointInsideRect(vec2 point, Rect rect);
vec2 GetRectCenter(Rect rect);
