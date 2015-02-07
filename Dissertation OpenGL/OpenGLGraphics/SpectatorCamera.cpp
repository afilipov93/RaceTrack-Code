#include "SpectatorCamera.h"

SpectatorCamera::SpectatorCamera(Renderer& ren) :r(ren)
{
	r.SetViewMatrix(Matrix4::BuildViewMatrix(Vector3(0, 80, 70), Vector3(0, 0, 0)));
}


SpectatorCamera::~SpectatorCamera()
{
}

void SpectatorCamera::move(float msec)
{
	//TODO: make cam speed as a single parameter
	if (Keyboard::KeyDown(KEY_W)){
		r.SetViewMatrix(Matrix4::Translation(Vector3(0, 0, -0.01f*msec).Inverse())* r.GetViewMatrix());
	}
	if (Keyboard::KeyDown(KEY_S)){
		r.SetViewMatrix(Matrix4::Translation(Vector3(0, 0, 0.01f*msec).Inverse())* r.GetViewMatrix());
	}
	if (Keyboard::KeyDown(KEY_A)){
		r.SetViewMatrix(Matrix4::Translation(Vector3(-0.01f*msec, 0, 0).Inverse()) * r.GetViewMatrix());
	}
	if (Keyboard::KeyDown(KEY_D)){
		r.SetViewMatrix(Matrix4::Translation(Vector3(0.01f*msec, 0, 0).Inverse()) * r.GetViewMatrix());
	}

	if (Mouse::ButtonHeld(MOUSE_LEFT)){
		float yaw = Mouse::GetRelativePosition().x;
		float baw = Mouse::GetRelativePosition().y;

		r.SetViewMatrix(Matrix4::Rotation(yaw*msec, Vector3(0, 1, 0))*Matrix4::Rotation(baw*msec, Vector3(1, 0, 0))*r.GetViewMatrix());
		
	}
}