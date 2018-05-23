#pragma once
#include "Camera.h"
#include "Primitive.h"
#include <vector>
#include "Image.h"

using namespace std;

class Scene {
public:
	Scene();
	~Scene();
	void addP(Primitive* pri);
	void addL(Light* light);
	vector<Primitive*>* primitive();
	vector<Light*>* light();
	int lNum();
	int pNum();
	void setCamera(Vector3d camera);
	void setSize(Vector3d size);
	Camera* camera();
	Vector3d size();
	void compute();//当所有的东西都设置好后执行该函数进行渲染
	void show();//显示图片

private:
	Camera m_camera;//视点位置
	Vector3d m_size;//场景大小
	int m_priNum;//primitive数量
	vector<Primitive*> m_primitive;//所有场景中primitive的数组的头指针
	int m_lightNum;//光源数量
	vector<Light*> m_light;//光源位置
	Image m_image;//要显示的图片

};