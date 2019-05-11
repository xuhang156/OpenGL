#pragma once
#include<malloc.h>
#include<map>
#include<iostream>
#include<vector>
#include "glad/glad.h"		//注意头文件包含的顺序啊，这块容易报错
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glfw3.h"
using namespace std;
//预编译编译器版本
#ifdef _WIN64
#ifndef _DEBUG
#pragma comment(lib, "E:/workspace/lib/GLFW/lib/Release64_2019/glfw3dll.lib")
#else
#pragma comment(lib, "E:/workspace/lib/GLFW/lib/Debug64_2019/glfw3dll.lib")				//加载库路径
#endif 
#endif 
struct imgData
{
	int width;
	int height;
	int nrChannels;
	unsigned char* datas = NULL;
};
struct windowConfig
{
	int width;
	int height;
	string title;
};
//着色器类型
struct sharderType
{
	int vetexType = GL_VERTEX_SHADER;
	int fragmentType = GL_FRAGMENT_SHADER;
};

template <class T>
struct Point3
{
	T	x;
	T	y;
	T	z;
};
template <class T>
struct Point4
{
	T	x;
	T	y;
	T	z;
	T	l;	
};
//初始化OpenGL主版本和副版本
void initGlVersion(int mainVersion = 3, int minorVersion = 3);		
//初始化窗体信息
GLFWwindow* initGLFWWindow(const windowConfig&);
//顶点着色器设置 GLSL语言编写
char* GetVertexShaderGLSL(int index = 1);
//片段着色器
char* GetFragmentShaderGLSL(int index = 1);
//创建通用的着色器
bool createSharder(const int& type, char* typeGLSL, vector<int>& vecSharder);
//着色器程序对象，用于绑定，链接所有的着色器
bool createAndLinkProgram(int& program,vector<int>&);
//创建顶点索引对象
void createEBO(unsigned int* indexbuffer, unsigned int& EBO);
//从容器到数组的转换
template<class T>
T* changeToPoint(const vector<Point3<T>>& points)
{
	int allsize = sizeof(T)*points.size();
	T* p = new T[allsize];
	T* q = p;
	for (auto iter : points)
	{
		*(p++) = iter.x;
		*(p++) = iter.y;
		*(p++) = iter.z;
	}
	return q;
}
//创建顶点数组对象以及顶点缓存对象
template<class T>
bool initVAO_ABO(vector<Point3<T> >& points, unsigned int& VAO, unsigned int& VBO)
{
	glGenVertexArrays(1, &VAO);		//创建顶点数组对象
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	T* p = changeToPoint(points);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int size = 3 * points.size() * sizeof(T);
	glBufferData(GL_ARRAY_BUFFER, size, p, GL_STATIC_DRAW);			//GL_STATIC_DRAW:数据不会变 GL_DYNAMIC_DRAW：数据会改变很多 GL_STREAM_DRAW：数据每次绘制都会改变
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(T), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete []p;
	return true;
}

//设置填充模型
void setFilMode(const int& mode);
void disableVertexObj();
//获取最多顶点属性
int getMaxVertexAttribute();
void controlUniformGlobal(int);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

bool loadImage(std::string imgpath, imgData& img);