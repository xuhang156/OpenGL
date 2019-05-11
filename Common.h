#pragma once
#include<malloc.h>
#include<map>
#include<iostream>
#include<vector>
#include "glad/glad.h"		//ע��ͷ�ļ�������˳�򰡣�������ױ���
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glfw3.h"
using namespace std;
//Ԥ����������汾
#ifdef _WIN64
#ifndef _DEBUG
#pragma comment(lib, "E:/workspace/lib/GLFW/lib/Release64_2019/glfw3dll.lib")
#else
#pragma comment(lib, "E:/workspace/lib/GLFW/lib/Debug64_2019/glfw3dll.lib")				//���ؿ�·��
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
//��ɫ������
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
//��ʼ��OpenGL���汾�͸��汾
void initGlVersion(int mainVersion = 3, int minorVersion = 3);		
//��ʼ��������Ϣ
GLFWwindow* initGLFWWindow(const windowConfig&);
//������ɫ������ GLSL���Ա�д
char* GetVertexShaderGLSL(int index = 1);
//Ƭ����ɫ��
char* GetFragmentShaderGLSL(int index = 1);
//����ͨ�õ���ɫ��
bool createSharder(const int& type, char* typeGLSL, vector<int>& vecSharder);
//��ɫ������������ڰ󶨣��������е���ɫ��
bool createAndLinkProgram(int& program,vector<int>&);
//����������������
void createEBO(unsigned int* indexbuffer, unsigned int& EBO);
//�������������ת��
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
//����������������Լ����㻺�����
template<class T>
bool initVAO_ABO(vector<Point3<T> >& points, unsigned int& VAO, unsigned int& VBO)
{
	glGenVertexArrays(1, &VAO);		//���������������
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	T* p = changeToPoint(points);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int size = 3 * points.size() * sizeof(T);
	glBufferData(GL_ARRAY_BUFFER, size, p, GL_STATIC_DRAW);			//GL_STATIC_DRAW:���ݲ���� GL_DYNAMIC_DRAW�����ݻ�ı�ܶ� GL_STREAM_DRAW������ÿ�λ��ƶ���ı�
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(T), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete []p;
	return true;
}

//�������ģ��
void setFilMode(const int& mode);
void disableVertexObj();
//��ȡ��ඥ������
int getMaxVertexAttribute();
void controlUniformGlobal(int);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

bool loadImage(std::string imgpath, imgData& img);