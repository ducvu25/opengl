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

	// khoi tao va cau hinh thu vien GLFW
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

	//Dat bien glewExperimental va true  (bat buoc)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Dinh nghia kích thuoc viewport
	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);    // l?y kích th??c framebuffer   (chú ý)
	width = WIDTH;
	height = HEIGHT;
	glViewport(0, 0, width, height);


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
	float  vertices_tamgiac1[] = {
		-0.4f,0.0f,0.0f, //left		
		0.0f,0.0f,0.0f, //right
		-0.2f,0.3f,0.0f,  //top

	};

	float  vertices_tamgiac2[] = {

		0.0f,0.0f,0.0f, //left		
		0.4f,0.0f,0.0f, //right
		0.2f,0.3f,0.0f,  //top
	};
	GLuint VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	//bind VAO truoc khi ma chung ta Bind VBO - cau hinh thuoc tinh dinh (vertex attribute)


	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	//Sao chep du lieu vao VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tamgiac1), vertices_tamgiac1, GL_STATIC_DRAW);
	//lien ket (link) Vettex Attribute -> vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tamgiac2), vertices_tamgiac2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	//un bind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// check su kien  (an nút bàn phím, chuot,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Buoc 3 Vẽ hình 1 hình tam giác
		// ve tam giac 1
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// ve tam giac 2

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(1);

		///swap
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	//Terminate GLFW, xóa và don dep tài nguyên sau khi thoát
	glfwTerminate();
	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

