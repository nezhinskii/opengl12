#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "painter_state.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace sf;

class Painter {

	struct MyTexCoord {
		GLfloat x;
		GLfloat y;
	};

	struct MyVertex {
		GLfloat x;
		GLfloat y;
		GLfloat z;
	};

	struct MyColor {
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	};


	// ID ��������� ���������
	GLuint Programs[4];

	// ID ��������
	GLint Attrib_vertex[4];

	GLuint cubeCtVAO;

	GLuint tetrahedronVAO;

	GLuint circleVAO;

	const char* VertexShaderSource[4] = {
		R"(
		#version 330 core
		layout (location = 0) in vec3 coord;
		layout (location = 1) in vec3 inColor;
		layout (location = 2) in vec2 inTextureCoord;

		out vec4 color;
		out vec2 textureCoord;

		uniform mat4 model;
		uniform mat4 view; 
		uniform mat4 projection;

		void main() {
			gl_Position = projection * view * model * vec4(coord, 1.0);
			color = vec4(inColor, 1.0);
			textureCoord = inTextureCoord;
		}
		)",

		R"(
		#version 330 core
		layout (location = 0) in vec3 coord;
		layout (location = 1) in vec3 inColor;
		layout (location = 2) in vec2 inTextureCoord;
		out vec2 textureCoord;

		uniform mat4 model;
		uniform mat4 view; 
		uniform mat4 projection;

		void main() {
			gl_Position = projection * view * model * vec4(coord, 1.0);
			textureCoord = inTextureCoord;
		}
		)",

		R"(
		#version 330 core
		layout (location = 0) in vec3 coord;
		layout (location = 1) in vec4 inColor;

		out vec4 color;

		uniform mat4 model;
		uniform mat4 view; 
		uniform mat4 projection;

		void main() {
			gl_Position = projection * view * model * vec4(coord, 1.0);
			color = inColor;
		}
		)",

		R"(
		#version 330 core
		layout (location = 0) in vec3 coord;
		layout (location = 1) in vec4 inColor;

		out vec4 color;

		void main() {
			gl_Position = vec4(coord, 1.0);
			color = inColor;
		}
		)"
	};

	const char* FragShaderSources[4] = { 
		R"(
		#version 330 core
		out vec4 fragColor;

		in vec4 color;
		in vec2 textureCoord;

		uniform float mixRatio;
		uniform sampler2D textureSampler;
		void main() {
			fragColor = mix(texture(textureSampler, textureCoord), color, mixRatio);
		}
		)",

		R"(
		#version 330 core
		out vec4 fragColor;

		in vec4 color;
		in vec2 textureCoord;

		uniform float mixRatio;
		uniform sampler2D texture1Sampler;
		uniform sampler2D texture2Sampler;
		void main() {
			fragColor = mix(texture(texture1Sampler, textureCoord), texture(texture2Sampler, textureCoord), mixRatio);
		}
		)",

		R"(
		#version 330 core
		out vec4 fragColor;

		in vec4 color;
		void main() {
			fragColor = color;
		}
		)",

		R"(
		#version 330 core
		out vec4 fragColor;

		in vec4 color;
		void main() {
			fragColor = color;
		}
		)"
	};


	void ShaderLog(unsigned int shader)
	{
		int infologLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
		if (infologLen > 1)
		{
			int charsWritten = 0;
			std::vector<char> infoLog(infologLen);
			glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
			std::cout << "InfoLog: " << infoLog.data() << std::endl;
		}
	}

	GLuint texture1, texture2;

	void InitVBO() {
		srand(time(0));
		InitCube();
		InitTetrahedron();
		InitCircle();
	}

	void InitCube() {
		GLfloat cube[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f
		};


		GLuint cubeCtVBO;
		glGenVertexArrays(1, &cubeCtVAO);
		glBindVertexArray(cubeCtVAO);
		glGenBuffers(1, &cubeCtVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeCtVBO);


		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	}

	void HSVtoRGB(float h, float s, float v, float& r, float& g, float& b) {
		int i = static_cast<int>(h * 6.0f);
		float f = h * 6.0f - i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - f * s);
		float t = v * (1.0f - (1.0f - f) * s);

		switch (i % 6) {
			case 0: r = v, g = t, b = p; break;
			case 1: r = q, g = v, b = p; break;
			case 2: r = p, g = v, b = t; break;
			case 3: r = p, g = q, b = v; break;
			case 4: r = t, g = p, b = v; break;
			case 5: r = v, g = p, b = q; break;
		}
	}

	int circleSteps = 360;

	void InitCircle() {
		std::vector<MyVertex> circleVert;
		std::vector<MyColor> circleColors;

		float radius = 0.8f;
		float angle = 3.1415f * 2.0f / circleSteps;
		int vertCount = circleSteps * 3;

		for (int i = 0; i < circleSteps; i++)
		{
			circleVert.push_back(MyVertex{ 0.0f, 0.0f, 0.0f });
			circleVert.push_back(MyVertex{ radius * cos(angle * (i % vertCount)), radius * sin(angle * (i % vertCount)), 0.0f });
			circleVert.push_back(MyVertex{ radius * cos(angle * ((i+1) % vertCount)), radius * sin(angle * ((i + 1) % vertCount)), 0.0f });

			circleColors.push_back(MyColor{ 1.0f, 1.0f, 1.0f, 1.0f });

			float r, g, b;

			float hue = static_cast<float>(i) / static_cast<float>(circleSteps);
			HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
			circleColors.push_back(MyColor{ r, g, b, 1.0f });

			hue = static_cast<float>((i + 1) % vertCount) / static_cast<float>(circleSteps);
			HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
			circleColors.push_back(MyColor{ r, g, b, 1.0f });
		}

		GLuint circleVBOs[2];
		glGenBuffers(2, circleVBOs);

		glGenVertexArrays(1, &circleVAO);
		glBindVertexArray(circleVAO);

		glBindBuffer(GL_ARRAY_BUFFER, circleVBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * vertCount, circleVert.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, circleVBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyColor) * vertCount, circleColors.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
	}

	void InitTetrahedron() {
		std::vector<MyVertex> tetrRawVert{
			MyVertex{ 0.5f, -0.5f, -0.5f },
			MyVertex{ -0.5f, -0.5f, 0.5f },
			MyVertex{ -0.5f, 0.5f, -0.5f },
			MyVertex{ 0.5f, 0.5f, 0.5f },
		};

		std::vector<MyColor> tetrRawColors{
			MyColor{ 1.0f, 0.0f, 0.0f, 1.0 },
			MyColor{ 0.0f, 1.0f, 0.0f, 1.0 },
			MyColor{ 0.0f, 0.0f, 1.0f, 1.0 },
			MyColor{ 1.0f, 1.0f, 1.0f, 1.0 },
		};

		std::vector<MyVertex> tetrVert;
		std::vector<MyColor> tetrColors;

		addTriangle(tetrRawVert, tetrRawColors, tetrVert, tetrColors, 0, 2, 1);
		addTriangle(tetrRawVert, tetrRawColors, tetrVert, tetrColors, 1, 2, 3);
		addTriangle(tetrRawVert, tetrRawColors, tetrVert, tetrColors, 0, 3, 2);
		addTriangle(tetrRawVert, tetrRawColors, tetrVert, tetrColors, 0, 1, 3);

		GLuint tetrVBOs[2];
		glGenBuffers(2, tetrVBOs);

		glGenVertexArrays(1, &tetrahedronVAO);
		glBindVertexArray(tetrahedronVAO);

		glBindBuffer(GL_ARRAY_BUFFER, tetrVBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * 12, tetrVert.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, tetrVBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyColor) * 12, tetrColors.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
	}

	void addTriangle(
		std::vector<MyVertex>& vertRaw,
		std::vector<MyColor>& colorRaw,
		std::vector<MyVertex>& vert,
		std::vector<MyColor>& color,
		int i1,
		int i2,
		int i3
	) {
		vert.push_back(vertRaw[i1]);
		vert.push_back(vertRaw[i2]);
		vert.push_back(vertRaw[i3]);

		color.push_back(colorRaw[i1]);
		color.push_back(colorRaw[i2]);
		color.push_back(colorRaw[i3]);
	}

	void InitShader() {
		//// ������� ��������� ������
		//GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		//// �������� �������� ���
		//glShaderSource(vShader, 1, &VertexShaderSource, NULL);
		//// ����������� ������
		//glCompileShader(vShader);
		//std::cout << "vertex shader \n";
		//// ������� ������ ���� �������
		//ShaderLog(vShader); //������ ������� ���� � ������������

		GLuint vShaders[4];

		for (int i = 0; i < 4; i++) {
			vShaders[i] = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vShaders[i], 1, &(VertexShaderSource[i]), NULL);

			glCompileShader(vShaders[i]);
			std::cout << "vertex shader" << i << std::endl;
			ShaderLog(vShaders[i]);
		}

		GLuint fShaders[4];

		for (int i = 0; i < 4; i++) {
			// ������� ����������� ������
			fShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);
			// �������� �������� ���
			glShaderSource(fShaders[i], 1, &(FragShaderSources[i]), NULL);
			// ����������� ������
			glCompileShader(fShaders[i]);
			std::cout << "fragment shader" << i << std::endl;
			ShaderLog(fShaders[i]); //������ ������� ���� � ������������
		}

		
		for (int i = 0; i < 4; i++) {
			// ������� ��������� � ����������� ������� � ���
			Programs[i] = glCreateProgram();
			glAttachShader(Programs[i], vShaders[i]);
			glAttachShader(Programs[i], fShaders[i]);
			// ������� ��������� ���������
			glLinkProgram(Programs[i]);
			// ��������� ������ ������
			int link_ok;
			glGetProgramiv(Programs[i], GL_LINK_STATUS, &link_ok);
			if (!link_ok) {
				std::cout << "error attach shaders \n";
				return;
			}
			// ���������� ID �������� �� ��������� ���������
			//const char* attr_name = "coord"; //��� � �������
			//Attrib_vertex[0] = glGetAttribLocation(Programs[i], attr_name);
			//if (Attrib_vertex[0] == -1) {
			//	std::cout << "could not bind attrib " << attr_name << std::endl;
			//	return;
			//}
		}
	}

	void InitTextures(TextureData texture1Data, TextureData texture2Data) {

		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture1Data.width, texture1Data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1Data.data);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture2Data.width, texture2Data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2Data.data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// ������������ ��������
	void ReleaseShader() {
		// ��������� ����, �� ��������� ��������� ���������
		glUseProgram(0);
		// ������� ��������� ���������
		for (int i = 0; i < 4; i++) {
			glDeleteProgram(Programs[i]);
		}
	}

	// ������������ ������
	//void ReleaseVBO() {
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glDeleteBuffers(1, &quadVBO);
	//	glDeleteBuffers(1, &fanVBO);
	//}
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.5f, 0.0f));

public:
	PainterState state;

	GLfloat angle = 0.0f;

	void Draw() {
		glEnable(GL_DEPTH_TEST);
		GLint currentAttrib;
		glUseProgram(Programs[state.figure]);
		angle += 0.005;
		rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.5f, 0.0f));
		switch (state.figure)
		{
		case Figure::CubeCT: {
			glBindVertexArray(cubeCtVAO);
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "model"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glUniform1f(glGetUniformLocation(Programs[state.figure], "mixRatio"), state.mixRatio);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(Programs[state.figure], "textureSampler"), 0);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			break;
		}	
		case Figure::CubeTT: {
			glBindVertexArray(cubeCtVAO);
			glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.5f, 0.0f));
			angle += 0.01;
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "model"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glUniform1f(glGetUniformLocation(Programs[state.figure], "mixRatio"), state.mixRatio);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(Programs[state.figure], "texture1Sampler"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(glGetUniformLocation(Programs[state.figure], "texture2Sampler"), 1);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			break;
		}
		case Figure::Tetrahedron:{
			glBindVertexArray(tetrahedronVAO);
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "model"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glDrawArrays(GL_TRIANGLES, 0, 12);
			glBindVertexArray(0);
			break;
		}
		case Figure::Circle: {
			glBindVertexArray(circleVAO);
			glDrawArrays(GL_TRIANGLES, 0, circleSteps * 3);
			glBindVertexArray(0);
			break;
		}
		default:
			break;
		}
	
		glUseProgram(0); // ��������� ��������� ���������
	}
	
	void Init(TextureData texture1Data, TextureData texture2Data) {

		glewInit();
		// �������
		InitShader();
		// ��������� �����
		InitVBO();
		
		InitTextures(texture1Data, texture2Data);
	}

	void Release() {
		// �������
		ReleaseShader();
		// ��������� �����
		//ReleaseVBO();
	}

};
