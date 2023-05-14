//khai báo và định thư viện stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

//
#include "Shader.h"

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

//include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;


int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tạo 1 cửa sổ GLFWwindow 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chong chong", nullptr, nullptr);
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
	int widthW, heightW;
	glfwGetFramebufferSize(window, &widthW, &heightW);    // lấy kích thước framebuffer   (chú ý)
	glViewport(0, 0, widthW, heightW);;

	//khởi tạo shader
	Shader ourShader("vShader.vertex", "fShader.frag");


	//bước 1: khai báo vertex input (vertex data)
	GLfloat vertices[] = {
		// vị trí - position      ///color         //texture coordinate (s,t)
		0.0f,0.0f,0.0f,			1.0f,0.0f,0.0f,	    0.0f,0.0f,
		0.5f,0.5f,0.0f,			0.0f,1.0f,0.0f,     0.5f,0.5f,
		0.0f,1.0f,0.0f,			1.0f,1.0f,0.0f,     0.0f,1.0f,
		0.5f,1.0f,0.0f,			0.0f,0.0f,1.0f,     0.5f,1.0f,
		0.5f,0.5f,0.0f,			0.0f,1.0f,0.0f,     0.5f,0.5f,
		0.0f,1.0f,0.0f,			1.0f,1.0f,0.0f,     0.0f,1.0f,

	};
	//GLuint indices[] = {
	//	0,1,2,  //tam giac 1
	//	3,2,0    //tam giac 2
	//};
	//Bước 2: Khởi tạo VBO, VAO
    //b2.1 VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);
	//b2.2 VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // liên kết (bind) VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//b2.3 EBO
	//GLuint EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // liên kết (bind) VBO
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Color attribute 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinate attribute 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
//unbind VAO
	glBindVertexArray(0);

	///Load và tạo 1 texture
	//khởi tạo texture
	GLuint texture_wood;
	glGenTextures(1, &texture_wood);
	glBindTexture(GL_TEXTURE_2D, texture_wood);
	//cài đặt tham số texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//cài đặt tham số texture filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//tải image, tạo texture + mipmaps
	int width, height, nrChannels;
	unsigned char* image = stbi_load("texture_wood2.jpg", &width, &height, &nrChannels, 0);
	if (image)
	{
		///tạo mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		//đưa ra thông báo lỗi load image
		std::cout << "Failed to Load texture" << std::endl;
	}
	//delete image 
	stbi_image_free(image);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);  //unbind texture




	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		////xóa color buffer
		//glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture_wood);

		//Buoc 3 Vẽ hình 1hình tam giác

		GLuint UniformLocation_Transform = glGetUniformLocation(ourShader.IDProgram, "transform");
		//sau có khi có vị trí uniform,
		ourShader.use();



			glm::mat4 Trans = glm::mat4(1.0f);
			Trans = glm::scale(Trans, glm::vec3(0.5f, 0.5f, 0.5f));
			UniformLocation_Transform = glGetUniformLocation(ourShader.IDProgram, "transform");
			glUniformMatrix4fv(UniformLocation_Transform, 1, GL_FALSE, glm::value_ptr(Trans));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES,0, 6);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);cach dung EB0
			glBindVertexArray(0);

		for (int i = 0; i < 4; i++)
		{
			glm::mat4 Trans = glm::mat4(1.0f);
			Trans = glm::scale(Trans, glm::vec3(0.5f, 0.5f, 0.5f));
			Trans = glm::rotate(Trans, glm::radians(-90.0f * i), glm::vec3(0.0f, 0.0f, 1.0f));
			Trans = glm::rotate(Trans, (float)glfwGetTime() * 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			UniformLocation_Transform = glGetUniformLocation(ourShader.IDProgram, "transform");
			glUniformMatrix4fv(UniformLocation_Transform, 1, GL_FALSE, glm::value_ptr(Trans));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES,0, 6);
			glBindVertexArray(0);
		}
		///swap
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);


	//Terminate GLFW, xóa và dọn dẹp tài nguyên sau khi thoát
	glfwTerminate();

	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
