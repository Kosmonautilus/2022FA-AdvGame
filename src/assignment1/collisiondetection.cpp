bool RectPointTest(vec2 point, Rect r)
{
	return (point.x >= r.min.x && point.x <= r.max.x 
		 && point.y >= r.min.y && point.y <= r.max.y);
}

bool RectRectTest(Rect r1, Rect r2)
{
	return (r1.min.x < r2.max.x &&
			r1.max.x > r2.min.x &&
			r1.min.y < r2.max.y &&
			r1.max.y > r2.min.y);
}