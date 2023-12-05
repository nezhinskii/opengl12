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

void texturePickerWidget(std::string title, std::string* path, TextureData* texture, Painter* painter, int ind) {
	if (ImGui::Button(title.c_str()))
		ImGuiFileDialog::Instance()->OpenDialog(title.c_str(), "Choose Texture", ".png,.jpg,.jpeg", ".");
	if ((*path).empty()) {
		ImGui::Text("Empty");
	}
	else {
		ImGui::Text((*path).c_str());
	}

	if (ImGuiFileDialog::Instance()->Display(title.c_str()))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			(*path) = filePathName;
			(*texture).data = stbi_load(filePathName.c_str(), &(*texture).width, &(*texture).height, &(*texture).channels, 0);
			(*painter).loadTexture(ind);
		}
		else {
			(*path).clear();
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode(600, 600), "My OpenGL window", sf::Style::Default, sf::ContextSettings(24));
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	auto painter = Painter();
	painter.Init();
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
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Lab 12");
		
		ImGui::RadioButton(label(Figure::CubeCT), &painter.state.figure, Figure::CubeCT);
		ImGui::RadioButton(label(Figure::CubeTT), &painter.state.figure, Figure::CubeTT);
		ImGui::RadioButton(label(Figure::Tetrahedron), &painter.state.figure, Figure::Tetrahedron);
		ImGui::RadioButton(label(Figure::Circle), &painter.state.figure, Figure::Circle);
			
		if (painter.state.figure == Figure::CubeCT) {
			texturePickerWidget("Pick texture", &painter.state.texturePath, &painter.state.texture, &painter, 0);
		}
		if (painter.state.figure == Figure::CubeTT) {
			texturePickerWidget("Pick texture 1", &painter.state.texture1Path, &painter.state.texture1, &painter, 1);
			texturePickerWidget("Pick texture 2", &painter.state.texture2Path, &painter.state.texture2, &painter, 2);
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