#include "Shader.h"
#define uint unsigned int
Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	ver = "";
	fra = "";
	geo = "";
	using namespace std;
	ifstream vfile;
	ifstream ffile;
	ifstream gfile;
	vfile.exceptions(ifstream::failbit | ifstream::badbit);
	ffile.exceptions(ifstream::failbit | ifstream::badbit);
	gfile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		vfile.open(vertexPath);
		ffile.open(fragmentPath);

		stringstream vstream, fstream, gstream;
		vstream << vfile.rdbuf();
		fstream << ffile.rdbuf();
		vfile.close();
		ffile.close();
		ver = vstream.str();
		fra = fstream.str();
		if (geometryPath != nullptr)
		{
			gfile.open(geometryPath);
			gstream << gfile.rdbuf();
			gfile.close();
			geo = gstream.str();
		}
	}
	catch (ifstream::failure e)
	{
		cout << "had error: " << e.what() << endl;
		issuccess = false;
		return;
	}
	uint vertex, fragment;
	const char* v = ver.c_str();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v, NULL);
	glCompileShader(vertex);
	if (!checkErrors(vertex, "VERTEX"))
	{
		issuccess = false;
		return;
	}
	const char* f = fra.c_str();
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f, NULL);
	glCompileShader(fragment);
	if (!checkErrors(fragment, "FRAGMENT"))
	{
		issuccess = false;
		return;
	}
	uint geometry;		//几何着色器
	if (geometryPath != nullptr)
	{
		const char* g = geo.c_str();
		geometry = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(geometry, 1, &g, NULL);
		glCompileShader(geometry);
		if (!checkErrors(geometry, "GEOMETRY"))
		{
			issuccess = false;
			return;
		}
	}
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	if (!checkErrors(ID, "PROGRAM"))
	{
		issuccess = false;
		return;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}
void Shader::use()
{
	glUseProgram(ID);
}
bool Shader::checkErrors(int shader, std::string name)
{
	int success;
	char infoLog[512];
	if (name != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);				//用于检测着色器是否成功
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);				//打印错误日志
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			return false;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " <<"\n" << infoLog << "\n  -- " << std::endl;
			return false;
		}
	}
	return true;
}

void Shader::setUniformVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}