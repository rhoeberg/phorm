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
