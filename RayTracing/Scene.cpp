#include "Scene.h"

Scene::Scene() : m_camera(Point(0,0,0)),m_priNum(0),m_lightNum(0),m_size(0,0,0),m_image(0,0){};

Scene::~Scene() {}

void Scene::addP(Primitive* pri) {
	m_primitive.push_back(pri);
	m_priNum += 1;
}

void Scene::addL(Light* light) {
	m_light.push_back(light);
	m_lightNum += 1;
}

vector<Primitive*>* Scene::primitive() {
	return &m_primitive;
}

vector<Light*>* Scene::light() {
	return &m_light;
}

int Scene::lNum() { return m_lightNum;  }

int Scene::pNum() { return m_priNum; }

void Scene::setCamera(Vector3d camera) { m_camera = Camera(camera); }

void Scene::setSize(Vector3d size) {
	m_size = size;
	m_image = Image(size.x(), size.y());
}

Camera* Scene::camera() { return &m_camera; }

Vector3d Scene::size() { return m_size; }

void Scene::compute() {

}

void Scene::show() {
	m_image.showImage();
}