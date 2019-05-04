#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	
	Shader(const char* vertexPath, const char* fragmentPath,const char* geometry = nullptr);
	bool checkErrors(int type, std::string name);
	void use();
	bool isSuccess() { return issuccess; }
	// uniform���ߺ���
	void setUniformBool(const std::string& name, bool value) const;
	void setUniformVec4(const std::string& name, float x, float y, float z, float w) const;
	void setUniformInt(const std::string& name, int value) const;
	void setUniformFloat(const std::string& name, float value) const;
	unsigned int ID;		//��ɫ���������
private:
	std::string ver;		//������ɫ���ű�����
	std::string fra;		//Ƭ����ɫ���ű�����
	std::string geo;		//������ɫ���ű�����
	bool issuccess;
};
#endif // !SHADER_H
