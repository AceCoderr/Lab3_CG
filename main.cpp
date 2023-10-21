#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

const char* vertexShadersource = R"(#version 330 core
layout(location = 0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";

const char* FragmentShadersource = R"(#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(0.8f,0.3f,0.02f, 1.0f);
})";
int main()
{
	//intialize glfw
	glfwInit();

	//tell glfw what version of opengl we are using
	//in this case we are using opengl 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//tell glfw we are using core profile
	//so that means we only have modern functions.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

	//Create a GLFWwindow Object of 800 by 800 pixel
	GLFWwindow* window = glfwCreateWindow(800, 800, "Model", NULL, NULL);
	
	//Error check if the window got created correctly
	if (window == NULL)
	{
		std::cout << "Failed";
		glfwTerminate();
		return -1;
	}
	//Make the created window as current context as opengl is dumb
	glfwMakeContextCurrent(window);
	//Load Glad in opengl
	gladLoadGL();

	//Specify the viewport
	glViewport(0, 0, 800, 800);

	//Creating an reference to a  vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//adding the actual vertex shader to the reference
	glShaderSource(vertexShader, 1, &vertexShadersource, NULL);
	//Compliling the shader
	glCompileShader(vertexShader);

	//Same as above
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShadersource, NULL);
	glCompileShader(FragmentShader);

	//Creating a program to actually run those shaders
	GLuint shaderProgram = glCreateProgram();
	//Attaching which shader to run for this shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, FragmentShader);
	//Linking the program to the GPU
	glLinkProgram(shaderProgram);

	//Deleting the shader's as we used it already
	glDeleteShader(vertexShader); 
	glDeleteShader(FragmentShader);

	//To actually send the vertex data to the GPU from CPU, we use 
	//vertex buffer objects. 

	//creating an instance of the buffer and load vertex data into that

	GLuint VAO,VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//specify the color of the backgrround
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Specify the buffer we want to change/use
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window))
	{
		//specify the color of the backgrround
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Specify the buffer we want to change/use
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//We need to swap the back buffer with the front 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}