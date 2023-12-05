#pragma once
#include <GL/glew.h>

enum Figure : int {
	CubeCT,
	CubeTT,
	Tetrahedron,
	Circle
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

	static const char* figureLabels[4];
};
