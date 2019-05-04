#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件
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
	// uniform工具函数
	void setUniformBool(const std::string& name, bool value) const;
	void setUniformVec4(const std::string& name, float x, float y, float z, float w) const;
	void setUniformInt(const std::string& name, int value) const;
	void setUniformFloat(const std::string& name, float value) const;
	unsigned int ID;		//着色器程序对象
private:
	std::string ver;		//顶点着色器脚本数据
	std::string fra;		//片段着色器脚本数据
	std::string geo;		//几何着色器脚本数据
	bool issuccess;
};
#endif // !SHADER_H
