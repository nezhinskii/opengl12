#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <GL/glew.h>

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>

#include "painter.h"
#include "painter_state.h";
#include "lib/ImGuiFileDialog/ImGuiFileDialog.h";
#include "lib/stb_image.h";

using namespace sf;

int main() {
	sf::RenderWindow window(sf::VideoMode(600, 600), "My OpenGL window", sf::Style::Default, sf::ContextSettings(24));
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	auto painter = Painter();
	TextureData texture1, texture2;
	int channels;
	texture1.data = stbi_load("texture1.jpg", &texture1.width, &texture1.height, &channels, 0);
	texture2.data = stbi_load("texture2.jpg", &texture2.width, &texture2.height, &channels, 0);
	painter.Init(texture1, texture2);

	if (!ImGui::SFML::Init(window)) return -1;

	sf::Clock deltaClock;		
	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized) {
				glViewport(
					event.size.width/2 - 300, 
					event.size.height/2 - 300, 
					600,
					600
				);
			}
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code)
				{
				case sf::Keyboard::Down:
					if (painter.state.figure == Figure::CubeCT || painter.state.figure == Figure::CubeTT) {
						if (painter.state.mixRatio > 0) {
							painter.state.mixRatio -= 0.02;
						}
					}
					else if (painter.state.figure == Figure::Circle) {
						painter.state.circleCoef[painter.state.circleAxis] -= 0.1;
					}
					else if (painter.state.figure == Figure::Tetrahedron) {
						painter.state.tetrahedronOffset[painter.state.tetrahedronAxis] -= 0.1;
					}
					break;
				case sf::Keyboard::Up:
					if (painter.state.figure == Figure::CubeCT || painter.state.figure == Figure::CubeTT) {
						if (painter.state.mixRatio < 1) {
							painter.state.mixRatio += 0.02;
						}
					}
					else if (painter.state.figure == Figure::Circle) {
						painter.state.circleCoef[painter.state.circleAxis] += 0.1;
					}
					else if (painter.state.figure == Figure::Tetrahedron) {
						painter.state.tetrahedronOffset[painter.state.tetrahedronAxis] += 0.1;
					}
					break;
				default:
					break;
				}
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Lab 12");
		
		ImGui::RadioButton(label(Figure::CubeCT), &painter.state.figure, Figure::CubeCT);
		ImGui::RadioButton(label(Figure::CubeTT), &painter.state.figure, Figure::CubeTT);
		ImGui::RadioButton(label(Figure::Tetrahedron), &painter.state.figure, Figure::Tetrahedron);
		ImGui::RadioButton(label(Figure::Circle), &painter.state.figure, Figure::Circle);

		switch (painter.state.figure) {
		case Figure::Circle:
			ImGui::Text("Axis:");
			ImGui::RadioButton("X", &painter.state.circleAxis, Axis2D::X2);
			ImGui::RadioButton("Y", &painter.state.circleAxis, Axis2D::Y2);
			break;
		case Figure::Tetrahedron:
			ImGui::Text("Axis:");
			ImGui::RadioButton("X", &painter.state.tetrahedronAxis, Axis3D::X3);
			ImGui::RadioButton("Y", &painter.state.tetrahedronAxis, Axis3D::Y3);
			ImGui::RadioButton("Z", &painter.state.tetrahedronAxis, Axis3D::Z3);
			break;
		}

		window.clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		painter.Draw();
		ImGui::SFML::Render(window);
	    

		window.display();
	}

	ImGui::SFML::Shutdown();
	painter.Release();
	return 0;
}