#pragma once

struct Rect {
	vec2 pos;
	float width;
	float height;
};

double Sin(double d);
bool PointInsideRect(vec2 point, Rect rect);
vec2 GetRectCenter(Rect rect);
float Noise2D(float x, float y, i32 octaves, double persistance, double frequency);
float Noise3D(float x, float y, float z, i32 octaves, double persistance, double frequency);
