#pragma once

class IScene
{
	virtual void init() = 0;
	virtual void render() = 0;
	//virtual void cleanUp() = 0;
};

