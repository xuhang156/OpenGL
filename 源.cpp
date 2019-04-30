
#include "Common.h"
#include <iostream>
using namespace std;
const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
	
	initGlVersion();
	windowConfig config{ WIDTH, HEIGHT, "LenrnOpenGL" };
	GLFWwindow* window = initGLFWWindow(config);
	if (window == NULL)
		return -1;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	vector<int>sharder;
	char* p = GetVertexShaderGLSL();
	if (!createSharder(GL_VERTEX_SHADER,p,sharder))
		return -1;
	p = GetFragmentShaderGLSL(2);
	if (!createSharder(GL_FRAGMENT_SHADER, p, sharder))
		return -1;
	
	//��ɫ����������Ƕ����ɫ���ϲ���������������ɵİ汾����Ҫ��������ɫ�����ӳ���ɫ���������
	int shaderProgram;
	if (!createAndLinkProgram(shaderProgram,sharder))
		return -1;
	unsigned int VAO, VBO,EBO;
	vector<Point3<float>> points;
	Point3<float> next{ 0.5f,0.5f,0.0f };
	Point3<float> right{ 0.5f, -0.5f, 0.0f };
	Point3<float> left{ -0.5f, -0.5f, 0.0f };
	Point3<float> top{ -0.5f,  0.5f, 0.0f };

	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,  // first Triangle
	1, 2, 3   // second Triangle
	};
	points.push_back(next);
	points.push_back(right);
	points.push_back(left);
	points.push_back(top);
	initVAO_ABO(points, VAO,VBO);
	createEBO(indices, EBO);
	setFilMode(GL_FILL);			//������Ⱦģʽ
	disableVertexObj();
	cout << "max vertex:" << getMaxVertexAttribute() << endl;;

	while (!glfwWindowShouldClose(window))
	{
		//glUseProgram(shaderProgram);			//������ɫ���������
		controlUniformGlobal(shaderProgram);
		glBindVertexArray(VAO);					//���ݱ�ʾ������Ӧ�Ķ����������
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	return 0;
}
