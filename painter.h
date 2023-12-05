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
		void main() {
			gl_Position = vec4(coord, 1.0);
		}
		)",

		R"(
		#version 330 core
		layout (location = 0) in vec3 coord;
		void main() {
			gl_Position = vec4(coord, 1.0);
		}
		)",

		R"(
		#version 330 core
		layout (location = 0) in vec3 coord;
		void main() {
			gl_Position = vec4(coord, 1.0);
		}
		)"
	};

	const char* FragShaderSources[4] = { 
		R"(
		#version 330 core
		out vec4 fragColor;

		in vec4 color;
		in vec2 textureCoord;

		uniform bool useTexture;
		uniform sampler2D textureSampler;
		void main() {
			fragColor = texture(textureSampler, textureCoord);
		}
		)",

		R"(
		#version 330 core
		out vec4 fragColor;
		void main() {
			fragColor = vec4(1, 0, 0, 1);
		}
		)",

		R"(
		#version 330 core
		out vec4 fragColor;
		void main() {
			fragColor = vec4(1, 0, 0, 1);
		}
		)",

		R"(
		#version 330 core
		out vec4 fragColor;
		void main() {
			fragColor = vec4(1, 0, 0, 1);
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

	float randF(float start, float end) {
		return (float)((rand() % (int)((end - start) * 10)) / 10.0 + start);
	}

	GLuint texture1, texture2;
	glm::mat4 cubeRotationMatrix;
	std::vector<MyColor> cubePointColors{
		MyColor{ 1.0f, 0.0f, 0.0f, 1.0 },
		MyColor{ 0.0f, 1.0f, 0.0f, 1.0 },
		MyColor{ 0.0f, 0.0f, 1.0f, 1.0 },
		MyColor{ 1.0f, 1.0f, 1.0f, 1.0 },

		MyColor{ 1.0f, 1.0f, 0.0f, 1.0 },
		MyColor{ 0.0f, 1.0f, 1.0f, 1.0 },
		MyColor{ 1.0f, 0.0f, 1.0f, 1.0 },
		MyColor{ 0.0f, 0.0f, 0.0f, 1.0 },
	};
	std::vector<MyVertex> cubePoints{
		MyVertex{ 0.5f, -0.5f, -0.5f },
		MyVertex{ 0.5f, 0.5f, -0.5f },
		MyVertex{ -0.5f, 0.5f, -0.5f },
		MyVertex{ -0.5f, -0.5f, -0.5f },

		MyVertex{ -0.5f, -0.5f, 0.5f },
		MyVertex{ -0.5f, 0.5f, 0.5f },
		MyVertex{ 0.5f, 0.5f, 0.5f },
		MyVertex{ 0.5f, -0.5f, 0.5f },
	};

	void addCubeTriangle(
		std::vector<MyVertex>& vert,
		std::vector<MyColor>& color,
		std::vector<MyTexCoord>& texture,
		int i1,
		int i2,
		int i3,
		int i4
	) {
		vert.push_back(cubePoints[i1]);
		vert.push_back(cubePoints[i4]);
		vert.push_back(cubePoints[i3]);
		vert.push_back(cubePoints[i3]);
		vert.push_back(cubePoints[i2]);
		vert.push_back(cubePoints[i1]);

		color.push_back(cubePointColors[i1]);
		color.push_back(cubePointColors[i4]);
		color.push_back(cubePointColors[i3]);
		color.push_back(cubePointColors[i3]);
		color.push_back(cubePointColors[i2]);
		color.push_back(cubePointColors[i1]);

		texture.push_back(MyTexCoord{ 0.0f, 0.0f });
		texture.push_back(MyTexCoord{ 1.0f, 0.0f });
		texture.push_back(MyTexCoord{ 1.0f, 1.0f });
		texture.push_back(MyTexCoord{ 1.0f, 1.0f });
		texture.push_back(MyTexCoord{ 0.0f, 1.0f });
		texture.push_back(MyTexCoord{ 0.0f, 0.0f });
	}

	void InitVBO() {
		srand(time(0));
		
		InitCube();
	}

	void InitCube() {
		GLfloat cube[] = {
			-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
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

		for (int i = 0; i < 3; i++) {
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
			const char* attr_name = "coord"; //��� � �������
			Attrib_vertex[0] = glGetAttribLocation(Programs[i], attr_name);
			if (Attrib_vertex[0] == -1) {
				std::cout << "could not bind attrib " << attr_name << std::endl;
				return;
			}
		}
	}

	void InitTextures(TextureData texture1Data, TextureData texture2Data) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture1Data.width, texture2Data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2Data.data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// ������������ ��������
	void ReleaseShader() {
		// ��������� ����, �� ��������� ��������� ���������
		glUseProgram(0);
		// ������� ��������� ���������
		for (int i = 0; i < 3; i++) {
			glDeleteProgram(Programs[i]);
		}
	}

	// ������������ ������
	//void ReleaseVBO() {
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glDeleteBuffers(1, &quadVBO);
	//	glDeleteBuffers(1, &fanVBO);
	//}

public:
	PainterState state;

	GLfloat angle = glm::radians(0.0f);

	void Draw() {
		glEnable(GL_DEPTH_TEST);
		GLint currentAttrib;
		glUseProgram(Programs[state.figure]);

		switch (state.figure)
		{
		case Figure::CubeCT: {
			glBindVertexArray(cubeCtVAO);
			glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.5f, 0.0f));
			angle += 0.005;
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "model"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(Programs[state.figure], "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			GLint useTextureLoc = glGetUniformLocation(Programs[state.figure], "useTexture");
			glUniform1i(useTextureLoc, GL_TRUE);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			GLint textureLoc = glGetUniformLocation(Programs[state.figure], "textureSampler");
			glUniform1i(textureLoc, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			break;
		}	
		case Figure::CubeTT:
			

			break;
		case Figure::Tetrahedron:
			
			break;
		case Figure::Circle:

			break;
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
