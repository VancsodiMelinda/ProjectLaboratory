#pragma once

#include "LoadAssets.h"
#include "ProgramData.h"
#include "Instrumentor.h"

class LoadOutlines
{
private:
	LoadAssets& assets;
	ProgramContainer programContainer;
public:
	LoadOutlines(LoadAssets& assets_, ProgramContainer programContainer_);

private:
	void checkProgram(ProgramContainer programContainer_);
	void configOutline(ObjectContainer& object);
	void renderOutline(ObjectContainer& object, Kamera& kamera);

	struct UniformLocations {
		GLint MVPloc = -1;
	};
	UniformLocations uniformLocations;

public:
	void config();
	void render(Kamera& kamera);
};

