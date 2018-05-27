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
	void emitViewRay();//��Camera������ߣ��������¼��kd����
	void emitPhoton();//�ӹ�Դ������ӣ������ӵ��������������

};