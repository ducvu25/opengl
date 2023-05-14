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
"void main()\n"
"{\n"
"gl_Position= vec4(position.x+0.1,position.y,position.z,1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color= vec4(1.0f,1.0f,0.0f,1.0f);\n"
"}\0";
int main(void)
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tao 1 cua so GLFWwindow 
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
	glfwGetFramebufferSize(window, &width, &height);
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
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
		std::cout << "loi bien dich Fragment Shader\n" << infoLog << std::endl;
	}

	// lien ket Shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexsharder);
	glAttachShader(shaderProgram, fragmentSharder);
	glLinkProgram(shaderProgram);
	//check loi qua trinh link
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "loi link Shader Program\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexsharder);
	glDeleteShader(fragmentSharder);

	// khai bao vertex input

	// 2 hinh tam giac
	
	//GLfloat  vertices[] = {
	//-0.8f,0.0f,0.0f, //left		
	//-0.2f,0.0f,0.0f, //right
	//-0.5f,0.4f,0.0f,  //top
	//-0.2f,0.0f,0.0f, //left		
	//0.4f,0.0f,0.0f, //right
	//0.1f,0.4f,0.0f,  //top
	//};

	//hinh vuong
	GLfloat  vertices[] = {
		-0.5f,0.0f,0.0f, //left		
		0.0f,0.0f,0.0f, //right
		0.0f,0.5f,0.0f,  //top

		0.0f,0.5f,0.0f,
		-0.5f,0.0f,0.0f,
		-0.5f,0.5f,0.0f,
	};


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//bind VAO truoc khi ma chung ta Bind VBO - cau hinh thuoc tinh dinh (vertex attribute)
	glBindVertexArray(VAO);
	//
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Sao chep du lieu vao VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//lien ket (link) Vettex Attribute -> vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//un bind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// check su kien  (an phim ban phi­m, chuot,...)
		glfwPollEvents();
		//Render
		//xoa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Buoc 3 Ve hinh 1 hinh tam giac

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		///swap
		glfwSwapBuffers(window);
	}
	//Terminate GLFW, xoa va don dep tai nguyen sau khi thoat
	glfwTerminate();
	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
