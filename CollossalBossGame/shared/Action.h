#pragma once

struct controllerstatus {
	bool A, B, X, Y;
	bool UP, Down, Left, Right;
	bool Start, Back, LShoulder, RShoulder;
	bool LTrigger, RTrigger, LPress, RPress;
	float x1, y1;
	float x2, y2;
};