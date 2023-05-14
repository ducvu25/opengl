//Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
//#include <glm/glm.hpp>
//using namespace glm;
//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;
//bước 0.0: chuẩn bị shader
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location=0) in vec3 position;\n"
"layout (location=1) in vec3 Color;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position= vec4(position.x,position.y,position.z,1);\n"
"ourColor = Color;\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"color =vec4( ourColor,1.0f);\n"
"}\0";
int main(void)
{
	// khoi tao va cau hinh thu vien GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tạo 1 cửa sổ GLFWwindow
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vi du 1 Hello Window", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//Đặt biến glewExperimental về true  (bắt buộc)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Định nghĩa kích thước viewport
	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);    // lấy kích thước framebuffer   (chú ý)
	glViewport(0, 0, width, height);


	////// bước 0.1 Build và biên dịch shader program
	//vertex shader
	GLuint vertexsharder = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexsharder, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexsharder);

	// kiem tra loi  shader trong qua trinh compile
	int success; // kiem tra bien dich co thanh cong?
	char infoLog[512];
	glGetShaderiv(vertexsharder, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexsharder, 512, NULL, infoLog);
		std::cout << "loi bien dich Vertex Shader\n" << infoLog << std::endl;
	}


	//fragment shader
	GLuint fragmentSharder = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentSharder, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentSharder);

	// kiem tra loi  shader trong qua trinh compile
	 // kiem tra bien dich co thanh cong?

	glGetShaderiv(fragmentSharder, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentSharder, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link Shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexsharder);
	glAttachShader(shaderProgram, fragmentSharder);
	glLinkProgram(shaderProgram);
	//check loi qua trinh link
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexsharder);
	glDeleteShader(fragmentSharder);

	//bước 1: khai báo vertex input (vertex data)
	GLfloat  vertices[] = {
		-0.5f,-0.5,0.0f, //left	
		0.5f,-0.5f,0.0f, //right
		0.0f,0.5f,0.0f  //top
	};
	GLfloat color[] = {
		0.0f,1.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f
	};


	//VBO: co muc dich luu tru vertices (verdata) vao nho cua GPU
	//VAO (Vertex arrays object): su dung du lieu luu tru  cua VBO va mo ta cho OPENGL biet quy dinh cua du lieu dau vao do la nhu nao ve doi tuong

	GLuint VAO;
	GLuint VBO[2];
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	//bind VAO truoc khi ma chung ta Bind VBO - cau hinh thuoc tinh dinh (vertex attribute)
	glBindVertexArray(VAO);
	//
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//Sao chep du lieu vao VBOs
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//lien ket (link) Vettex Attribute -> vertex shader
	// set // possittions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//un bind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Buoc 3 Vẽ hình 1 hình tam giác

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		///swap
		glfwSwapBuffers(window);
	}
	//Terminate GLFW, xóa và dọn dẹp tài nguyên sau khi thoát
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO[2]);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
