#include "SpectatorCamera.h"
#include "RenderObject.h"
#include "Track.h"
#include <ctime>

#pragma comment(lib, "nclgl.lib")

void main(void) {
	Window w = Window(800, 600);
	Renderer r(w);
	SpectatorCamera specCam(r);
	srand(time(NULL));
	Track t;

	//Mesh*	m = Mesh::LoadMeshFile("cube.asciimesh");
	Shader* s = new Shader("basicvert.glsl", "basicFrag.glsl");
	t.genTrack(10);

	//Ground
	Mesh* cube = Mesh::GenerateSquare();
	RenderObject cubeObj(cube, s);
	cubeObj.SetModelMatrix(Matrix4::Rotation(90, Vector3(1, 0, 0))*Matrix4::Scale(Vector3(GROUND_X,GROUND_Z,0)));
	r.AddRenderObject(cubeObj);
	
	//track
	Mesh* track = Mesh::GenerateLineLoop(t.getTrackPoints());
	RenderObject trackObj(track, s);
	r.AddRenderObject(trackObj);

	//points within track
	Mesh* points = Mesh::GeneratePoints(t.getPointsWithinTrack());
	RenderObject pointsObj(points, s);
	r.AddRenderObject(pointsObj);

	r.SetProjectionMatrix(Matrix4::Perspective(1, 100, 1.33f, 90.0f));

	while (w.UpdateWindow()) {
		float msec = w.GetTimer()->GetTimedMS();

		specCam.move(msec);

		if (Keyboard::KeyHeld(KEY_R))
		{
			t.genTrack(50);
			track = Mesh::GenerateLineLoop(t.getTrackPoints());
			trackObj.SetMesh(track);
		}

		r.UpdateScene(msec);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	}

	delete cube;
	delete track;
	delete s;
}