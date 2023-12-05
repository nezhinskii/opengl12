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
	std::string texturePath;
	TextureData texture;

	std::string texture1Path;
	std::string texture2Path;
	TextureData texture1;
	TextureData texture2;

	static const char* figureLabels[4];
};
