#include <iostream>
#include "painter_state.h";

const char* PainterState::figureLabels[4] = {"Cube Color-Texture", "Cube Texture-Texture", "Tetrahedron", "Circle"};

const char* label(Figure figure) {
	return PainterState::figureLabels[figure];
}
