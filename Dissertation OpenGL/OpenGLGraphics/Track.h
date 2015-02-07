#ifndef Track_h
#define Track_h

#include "Renderer.h"
#include "GameVariables.h"
#include <stack>
#include <cmath>

static Vector3 firstPoint; // The Vector Point with lowest z

class Track
{
public:
	Track();
	~Track();
	// Creates the entire track
	void genTrack(int pointNum);
	// Getters
	inline std::vector<Vector3>& getPointsWithinTrack() { return trackCoords; }
	inline std::vector<Vector3>& getTrackPoints() { return trackHull; }


	
private:
	// Creates a convex Hull out of given points
	void convexHull(std::vector<Vector3>& randPoints);
	// Creates random points
	void genPoints(int pointNum);
	// Pushes points away to overcome overlapping
	void pushPointsApart(std::vector<Vector3>& points);
	// Adds additional points to the hull to increase the dynamic of the track
	void addTrackDynamic();

	std::vector<Vector3> trackCoords;
	std::vector<Vector3> trackHull;

};
#endif