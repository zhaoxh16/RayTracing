#pragma once
#include "Scene.h"
#include "kdtree.hpp"

class PPMEngine {
public:
	PPMEngine();
	void setScene(Scene* scene);
	void run();

private:
	Scene * scene;
	void emitViewRay();//从Camera发射光线，将交点记录在kd树中
	void emitPhoton();//从光源发射光子，将光子的能量分配给交点

};