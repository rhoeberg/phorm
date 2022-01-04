#pragma once

struct Ray {
	vec3 o;
	vec3 d;
};

struct Rect {
	vec2 pos;
	float width;
	float height;

	Rect() {}

	// create rect from two points
	// Point A should be in top left side
	Rect(vec2 a, vec2 b) {
		pos = a;
		if(a.x > b.x || a.y > b.y) {
			width = 0.0f;
			height = 0.0f;
			return;
		}

		width = b.x - a.x;
		height = b.y - a.y;
	}
};

double Sin(double d);
double Fmod(double numer, double denom);
float Min(float a, float b);
float Max(float a, float b);
bool PointInsideRect(vec2 point, Rect rect);
vec2 GetRectCenter(Rect rect);
float Noise2D(float x, float y, i32 octaves, double persistance, double frequency);
float Noise3D(float x, float y, float z, i32 octaves, double persistance, double frequency);
bool IntersectSegmentCylinder(vec3 sa, vec3 sb, vec3 p, vec3 q, float  r, float *t);
