bool PointInsideRect(vec2 point, Rect rect)
{
	if(point.x > rect.pos.x && point.x < rect.pos.x + rect.width &&
	   point.y > rect.pos.y && point.y < rect.pos.y + rect.height)
		return true;

	return false;
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
