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
	void compute();//�����еĶ��������úú�ִ�иú���������Ⱦ
	void show();//��ʾͼƬ

private:
	Camera m_camera;//�ӵ�λ��
	Vector3d m_size;//������С
	int m_priNum;//primitive����
	vector<Primitive*> m_primitive;//���г�����primitive�������ͷָ��
	int m_lightNum;//��Դ����
	vector<Light*> m_light;//��Դλ��
	Image m_image;//Ҫ��ʾ��ͼƬ

};