#include "texture.h"

Texture::Texture() {
	stretch = 1;
}

void Texture::setImage(string fileName) {
	image = imread(fileName);
	this->rows = image.rows;
	this->cols = image.cols;
}

void Texture::setStretch(double stretch) {
	this->stretch = stretch;
}

void PlaneTexture::setStartPoint(Vector3d point) {
	this->startPoint = point;
}

void PlaneTexture::setXDirection(Vector3d XDirection) {
	this->XDirection = XDirection;
}

void PlaneTexture::setYDirection(Vector3d YDirection) {
	this->YDirection = YDirection;
}

Vector3d PlaneTexture::getColor(Vector3d point) {
	Vector3d relativePosition = point - startPoint;
	int XPosition = relativePosition.dot(XDirection);
	int YPosition = relativePosition.dot(YDirection);
	XPosition %= int(cols*stretch);
	YPosition %= int(rows*stretch);
	Vec3b color = image.at<Vec3b>(YPosition, XPosition);
	Vector3d myColor;
	for (int i = 0; i < 3; ++i) {
		myColor[i] = color[2 - i];
	}
	return myColor;
}