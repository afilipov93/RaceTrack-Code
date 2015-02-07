#pragma once

#include "Renderer.h"

class SpectatorCamera
{
public:
	SpectatorCamera(Renderer& ren);
	~SpectatorCamera();

	void move(float msec);

private:
	Renderer& r;
};

