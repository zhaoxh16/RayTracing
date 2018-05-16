#include "Ray.h"

Ray::Ray() : origin(-1, -1, -1), direction(-1,-1,-1), nrefr(-1){}

Ray::Ray(Point origin, Direction direction, double nrefr) :
	origin(origin), direction(direction), nrefr(nrefr) {}
