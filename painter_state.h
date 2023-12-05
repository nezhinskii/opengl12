#pragma once
#include <GL/glew.h>

enum Figure : int {
	CubeCT,
	CubeTT,
	Tetrahedron,
	Circle
};

enum Axis2D : int {
	X2,
	Y2
};

enum Axis3D : int {
	X3,
	Y3,
	Z3
};

struct TextureData {
	int width;
	int height;
	int channels;
	unsigned char* data;
};

const char* label(Figure figure);

class PainterState {
public:
	int figure = Figure::CubeCT;
	double mixRatio = 1.0;

	int circleAxis = Axis2D::X2;
	float circleCoef[2] = {1.0f, 1.0f};

	int tetrahedronAxis = Axis3D::X3;
	float tetrahedronOffset[3] = { 0.0f, 0.0f, 0.0f };
	
	static const char* figureLabels[4];
};
