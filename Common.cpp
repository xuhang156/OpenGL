#include "stb_image.h"
#include "Common.h"

void initGlVersion(int mainVersion , int minorVersion )
{
	glfwInit();																			//���ȵ���init����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mainVersion);							//����glfw opengl���汾��	3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);							//����glfw opengl���汾��	3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);						//ʹ��opengl����ģʽ
}

GLFWwindow* initGLFWWindow(const windowConfig& config)
{
	GLFWwindow* window = glfwCreateWindow(config.width, config.height, config.title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	return window;
}
char* GetVertexShaderGLSL(int inde)
{
	map<int, string> members;
	int index = 1;
	string str = "#version 330 core									\n"							//��ɫ�����԰汾��Ϣ
				 "layout (location = 0) in vec3 aPos;				\n"							//������ɫ�����������ڴӶ���������ֱ�ӽ������ݣ������Ҫ������ʼλ��                           
				 "out vec4 veccolor;								\n"							//����һ���������ɫ����
				 "void main()										\n"
				 "{													\n"
				 "		gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
				 "      veccolor = vec4(0.5,0.0,0.0,1.0);			\n"
				 " }";
	members[index++] = str;
	str = "#version 330 core		\n"
		"layout (location = 0) in vec3 aPos;		\n"
		"layout (location = 1) in vec3 aColor;		\n"
		"out vec3 outColor;							\n"
		"void main()								\n"
		"{											\n"
		"	gl_Position = vec4(aPos,1.0);			\n"
		"	outColor = aColor;						\n"
		"}";
	members[index++] = str;
	auto iter = members.find(inde);
	if (iter == members.end())
		return NULL;
	str = iter->second;
	int size = str.size() + 1;
	char* p = new char[size];
	memcpy(p, str.c_str(), size);
	return p;
}
char* GetFragmentShaderGLSL(int inde)
{
	map<int, string> members;
	int index = 1;
	string str = "#version 330 core						\n	"
			"in vec4  veccolor;							\n"				//������һ����ɫ�����صı���
			"out vec4 FragColor;						\n"				//Ƭ����ɫ�������һ��4����������
			"void main()								\n"
			"{											\n"
			"FragColor = veccolor;						\n"				//�����ָ����ɫ��������ɫ
			"} ";
	members[index++] = str;
	str = "#version 330 core						\n	"
		"in vec4  veccolor;							\n"				//������һ����ɫ�����صı���
		"uniform vec4 globalColor;					\n"				//����һ��ȫ�ֵı���������c++�����и�ֵ�Ȳ���					
		"out vec4 FragColor;						\n"				//Ƭ����ɫ�������һ��4����������
		"void main()								\n"
		"{											\n"
		"FragColor = globalColor;					\n"				//�����ָ����ɫ��������ɫ
		"}\0";
	members[index++] = str;
	str = "#version 330 core						\n"
		"out vec4 FragColor;						\n"
		"in vec3 outColor;							\n"
		"void main()								\n"
		"{											\n"
		"	FragColor = vec4(outColor,1.0);         \n"
		"}\0";
	members[index++] = str;
	auto iter = members.find(inde);
	if (iter == members.end())
		return NULL;
	str = iter->second;
	int size = str.size() + 1;
	char* p = new char[size];
	memcpy(p, str.c_str(), size);
	return p;
}

bool createSharder(const int& type,char*typeGLSL,vector<int>& vecSharder)
{
	int shader = glCreateShader(type);					//���ȴ���һ����=������ GL_VERTEX_SHADER ������ɫ��
	glShaderSource(shader, 1, &typeGLSL, NULL);			//������ɫ������
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);				//���ڼ����ɫ���Ƿ�ɹ�
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);				//��ӡ������־
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	vecSharder.push_back(shader);
	return true;
}
bool createAndLinkProgram(int & shaderProgram,vector<int>& vecShader)
{
	shaderProgram = glCreateProgram();
	for(auto iter: vecShader)
		glAttachShader(shaderProgram, iter);
	glLinkProgram(shaderProgram);							//����������ɫ��������	������ɫ��	Ƭ����ɫ��֮��
	// check for linking errors
	int success;
	char infoLog[520];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		shaderProgram = -1;
		return false;
	}
	for(auto iter:vecShader)
		glDeleteShader(iter);							//���Ӻ�֮��ɾ����ɫ������
	return true;
}

void setFilMode(const int& mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void createEBO(unsigned int*indexbuffer, unsigned int& EBO)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);				//�󶨵���������������һ��Ϊ���㻺�����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,24, indexbuffer, GL_STATIC_DRAW);

}
void disableVertexObj()
{
	glBindVertexArray(0);									//ȡ����������İ�
}

int getMaxVertexAttribute()
{
	int nr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr);
	return nr;
}
void controlUniformGlobal(int shaderProgram)
{
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "globalColor");
	glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (height == 0)  									// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}
	//glViewport(0, 0, width,height); // Setup viewport
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

bool loadImage(std::string imgpath,imgData& img)
{
	stbi_set_flip_vertically_on_load(true);	//����ͼ��֮ǰ��תy�ᣬopenglҪ��y��0��0λ��ͼ��ĵײ�������ʹ�ÿ��ȡ��ʱ��0��0��ͼ��Ķ����������ͼ��ת�����
	img.datas = stbi_load(imgpath.c_str(), &img.width, &img.height, &img.nrChannels, 0);
	if (img.datas == NULL)
		return false;
	return true;
}