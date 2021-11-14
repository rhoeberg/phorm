bool PointInsideRect(vec2 point, Rect rect)
{
	if(point.x > rect.pos.x && point.x < rect.pos.x + rect.width &&
	   point.y > rect.pos.y && point.y < rect.pos.y + rect.height)
		return true;

	return false;
}
