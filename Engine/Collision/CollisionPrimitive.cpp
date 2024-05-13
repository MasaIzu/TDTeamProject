#include "CollisionPrimitive.h"


void Triangle::ComputeNormal()
{

	Vector4 p0_p1 = p1 - p0;
	Vector4 p0_p2 = p2 - p0;

	normal = p0_p1.Vector3Cross(p0_p2);
	normal = normal.Vector3Normal();

}

