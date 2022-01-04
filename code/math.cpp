#include <math.h>

double Sin(double d)
{
	return sin(d);
}

double Fmod(double numer, double denom)
{
	return fmod(numer, denom);
}

float Min(float a, float b)
{
	return (a < b) ? a : b;
}

float Max(float a, float b)
{
	return (a > b) ? a : b;
}

vec2 GetRectCenter(Rect rect)
{
	vec2 result = {};
	result.x = rect.pos.x + rect.width / 2.0f;
	result.y = rect.pos.y + rect.height / 2.0f;
	return result;
}			

float Noise2D(float x, float y, i32 octaves, double persistance, double frequency)
{
	SimplexNoise1234 sn;

	double total = 0.0;
	double maxValue = 0.0;
	double amplitude = 1.0;

	for(i32 i = 0; i < octaves; i++) {
		total += sn.noise(x * frequency, y * frequency) * amplitude;
		maxValue += amplitude;
		amplitude *= persistance;
		frequency *= 2;
	}

	return total / maxValue;
}

float Noise3D(float x, float y, float z, i32 octaves, double persistance, double frequency)
{
	SimplexNoise1234 sn;

	double total = 0.0;
	double maxValue = 0.0;
	double amplitude = 1.0;

	for(i32 i = 0; i < octaves; i++) {
		total += sn.noise(x * frequency, y * frequency, z * frequency) * amplitude;
		maxValue += amplitude;
		amplitude *= persistance;
		frequency *= 2;
	}

	return total / maxValue;
}

bool PointInsideRect(vec2 point, Rect rect)
{
	if(point.x > rect.pos.x && point.x < rect.pos.x + rect.width &&
	   point.y > rect.pos.y && point.y < rect.pos.y + rect.height)
		return true;

	return false;
}

vec3 RayFromMouseCoord(int width, int height, int mouseX, int mouseY, mat4 projection, mat4 view)
{
	
	float x = (2.0f * mouseX) / width - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / height;

	vec3 ray_nds = vec3(x, y, -1.0f);
    vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
    vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
    vec3 ray_wor = vec3(glm::inverse(view) * ray_eye);
    ray_wor = glm::normalize(ray_wor);

	return ray_wor;
	// return origin + (ray_wor * t);
}

/*
  p = cylinder start
  q = cylinder end
  r = cylinder radius
  
  sa = segment start
  sb = segment end

  t = intersection distance along segment direction
 */
bool IntersectSegmentCylinder(vec3 sa, vec3 sb, vec3 p, vec3 q, float  r, float *t)
{
	double EPSILON = 0.00001;
    vec3 d = q - p, m = sa - p, n = sb - sa;
    float md = glm::dot(m, d);
    float nd = glm::dot(n, d);
    float dd = glm::dot(d, d);
    // Test if segment fully outside either endcap of cylinder
    if (md < 0.0f && md + nd < 0.0f) 
        return false; // Segment outside ’p’ side of cylinder 
    if (md > dd && md + nd > dd) 
        return false; // Segment outside ’q’ side of cylinder 
    
    float nn = glm::dot(n, n);
    float mn = glm::dot(m, n);
    float a = dd * nn - nd * nd;
    float k = glm::dot(m, m) - r * r;
    float c = dd * k - md * md;
    if (abs(a) < EPSILON) {
        // Segment runs parallel to cylinder axis
        if (c > 0.0f) 
            return false; // ’a’ and thus the segment lie outside cylinder 
        // Now known that segment intersects cylinder; figure out how it intersects 
        if (md < 0.0f) 
            *t = -mn / nn; // Intersect segment against ’p’ endcap
        else if (md > dd) 
            *t = (nd - mn) / nn; // Intersect segment against ’q’ endcap 
        else 
            *t = 0.0f; // ’a’ lies inside cylinder
        return true;
    }
    float b = dd * mn - nd * md;
    float discr = b * b - a * c;
    if (discr < 0.0f) return 0; // No real roots; no intersection
    *t = (-b - sqrt(discr)) / a;
    if (*t < 0.0f || *t > 1.0f) 
        return false; // Intersection lies outside segment 
    if (md + *t * nd < 0.0f) {
        // Intersection outside cylinder on ’p’ side
        if (nd <= 0.0f) 
            return false; // Segment pointing away from endcap 
        
        *t = -md / nd;
        // Keep intersection if Dot(S(t) - p, S(t) - p) <= r∧2
        return k + 2 * *t * (mn + *t * nn) <= 0.0f;
    } 
    else if (md + *t * nd > dd) {
        // Intersection outside cylinder on ’q’ side
        if (nd >= 0.0f) 
            return false; // Segment pointing away from endcap 
        *t = (dd - md) / nd;
        // Keep intersection if Dot(S(t) - q, S(t) - q) <= r^2
        return k + dd - 2 * md + *t * (2 * (mn - nd) + *t * nn) <= 0.0f;
    }
    // Segment intersects cylinder between the endcaps; t is correct
    return true; 
}
