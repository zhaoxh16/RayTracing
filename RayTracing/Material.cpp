#include "Material.h"

Material::Material() : color(-1, -1, -1), refl(-1), diff(-1), spec(-1), refr(-1), nrefr(-1) {}

Material::Material(Color color, double reflection, double diffuse, double spec, double refr, double nrefr) :
	color(color), refl(reflection), diff(diffuse), spec(spec), refr(refr), nrefr(nrefr) {
	this->color = color / 255.0;
}