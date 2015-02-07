#include "Track.h"


Track::Track()
{
}


Track::~Track()
{
}

void Track::genTrack(int pointNum)
{
	trackCoords.clear();
	trackHull.clear();
	this->genPoints(pointNum);
	this->convexHull(trackCoords);
	for (int i = 0; i < TRACK_REFINEMENT; i++)
	{
		this->pushPointsApart(trackHull);
	}
	this->addTrackDynamic();

}

void Track::genPoints(int pointNum)
{
	for (int i = 0; i < pointNum; ++i)
	{
		// Keep the random points with a PADDING to the ground end  
		float x = (float)(rand() % GROUND_X * 2 - GROUND_X);
		x = (x < 0) ? x + PADDING : x - PADDING;
		float y = (float)(rand() % (GROUND_Z + GROUND_X) / 20 + 1);
		float z = (float)(rand() % GROUND_Z * 2 - GROUND_Z);
		z = (z < 0) ? z + PADDING : z - PADDING;
		trackCoords.push_back(Vector3(x, y, z));

	}
}


// A utility function to find next to top in a stack
Vector3 nextToTop(stack<Vector3> &stack)
{
	Vector3 p = stack.top();
	stack.pop();
	Vector3 res = stack.top();
	stack.push(p);
	return res;
}

// Swap two Vectors Points
void swap(Vector3 &p1, Vector3 &p2)
{
	Vector3 temp = p1;
	p1 = p2;
	p2 = temp;
}

// dist between two Vector Points
float dist(Vector3 p1, Vector3 p2)
{
	float xd = p1.x - p2.x;
	float zd = p1.z - p2.z;

	return xd*xd + zd*zd;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Vector3 p, Vector3 q, Vector3 r)
{
	float val = (q.z - p.z) * (r.x - q.x) -
		(q.x - p.x) * (r.z - q.z);

	if (val == 0) return 0;  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// Used to define how Vector Points are going to be sorted in qsort()
int compare(const void * vp1, const void * vp2)
{
	const Vector3 *p1 = (const Vector3 *)vp1;
	const Vector3 *p2 = (const Vector3 *)vp2;

	int o = orientation(firstPoint, *p1, *p2);
	if (o == 0)
		return (dist(firstPoint, *p2) >= dist(firstPoint, *p1)) ? -1 : 1;
	return (o == 2) ? -1 : 1;
}

void Track::convexHull(std::vector<Vector3> &randPoints)
{
	float zmin = randPoints[0].z;
	int min = 0;
	float z;

	int randPointsSize = randPoints.size();

	for (int i = 1; i < randPointsSize; i++)
	{
		z = randPoints[i].z;

		// If current is smaller -> pick it. If it is equal to the smallest -> pick the one
		// with smaller x
		if ((z < zmin) || (zmin == z && randPoints[i].x < randPoints[min].x))
		{
			zmin = randPoints[i].z;
			min = i;
		}

	}

	// Place the bottom-most point at first position
	swap(randPoints[0], randPoints[min]);

	// Sort N-1 points in an anti-clockwise order
	firstPoint = randPoints[0];
	qsort(&randPoints[1], randPointsSize - 1, sizeof(Vector3), compare);



	// push the first three points
	std::stack<Vector3> stack;
	stack.push(randPoints[0]);
	stack.push(randPoints[1]);
	stack.push(randPoints[2]);

	// Sort remaining points
	for (int i = 3; i < randPointsSize; i++)
	{
		// Keep removing top while the angle formed by points next-to-top, 
		// top, and points[i] makes a non-left turn
		// TODO: AT SOME CASES THE STACK BECOMES EMPTY AND NEXT TO TOP FUNCTION GIVES EXCEPTEION WHEN .TOP() IS CALLED
		while (orientation(nextToTop(stack), stack.top(), randPoints[i]) != 2)
		{
			stack.pop();
		}
		stack.push(randPoints[i]);
	}

	// Now stack has the output points, print contents of stack
	while (!stack.empty())
	{
		trackHull.push_back(stack.top());
		stack.pop();

	}

}

void Track::pushPointsApart(std::vector<Vector3>& points)
{
	// Calculate the push distance and its squared value
	float distance = (GROUND_X + GROUND_Z) / 15.0f;
	float distanceSQ = distance*distance;
	int pSize = points.size();

	// Check if a point is too close to other points
	for (int i = 0; i < pSize; ++i)
	{
		for (int j = i + 1; j < pSize; ++j)
		{
			if (dist(points[i], points[j]) < distanceSQ)
			{
				float pushX = points[j].x - points[i].x;
				float pushZ = points[j].z - points[i].z;
				float hl = (float)sqrt(pushX*pushX + pushZ*pushZ);
				pushX /= hl;
				pushZ /= hl;
				float dif = distance - hl;
				pushX *= dif;
				pushZ *= dif;
				points[j].x += pushX;
				points[j].z += pushZ;
				points[i].x -= pushX;
				points[i].z -= pushZ;
			}
		}
	}
}


void Track::addTrackDynamic()
{
	std::cout << "Points in HULL " << trackHull.size() << std::endl;
	// The point between two other points
	Vector4 intermPoint;
	float maxDisplacement = PADDING / 2;
	std::cout << " MAX displacement " << maxDisplacement << std::endl;

	for (std::vector<Vector3>::iterator it = trackHull.begin(); it != trackHull.end()-2; it++)
	{
		// Rotate Randomly around local space
		intermPoint = Vector4(0, 0, 0, 1);
		int angle = rand() % 361;
		intermPoint = Matrix4::Rotation(angle, Vector3(0, 1, 0))*intermPoint;

		// Raises a random 0.0f - 1.0f to the track Difficulty and multiplies it by the given range of displacement
		float displaceVal = pow((static_cast <float> (rand()) / static_cast <float>(RAND_MAX)), TRACK_DIFFICULTY) * maxDisplacement;
		std::cout << "Current dwisplacement " << displaceVal << std::endl;
		intermPoint = Vector4((it->x + (it + 1)->x) / 2,
			(it->y + (it + 1)->y) / 2,
			(it->z + (it + 1)->z) / 2, 1.0f);

		// Translate the vector 
		intermPoint = Matrix4::Translation(Vector3(displaceVal, 0, displaceVal))*intermPoint;
		it++;
		it = trackHull.insert(it, Vector3(intermPoint.x, intermPoint.y, intermPoint.z));
		std::cout << "New trackHull size: " << trackHull.size() << std::endl;
	}

	for (int i = 0; i < TRACK_REFINEMENT; i++)
	{
		this->pushPointsApart(trackHull);
	}
	std::cout << "Points in HULL " << trackHull.size() << std::endl;
}