#include "stb_image.h"
#include "Common.h"

void initGlVersion(int mainVersion , int minorVersion )
{
	glfwInit();																			//首先调用init函数
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mainVersion);							//告诉glfw opengl主版本号	3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);							//告诉glfw opengl副版本号	3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);						//使用opengl核心模式
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
	string str = "#version 330 core									\n"							//着色器语言版本信息
				 "layout (location = 0) in vec3 aPos;				\n"							//顶点着色器特殊性在于从顶点数据中直接接受数据，因此需要定义起始位置                           
				 "out vec4 veccolor;								\n"							//声明一个输出的颜色变量
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
			"in vec4  veccolor;							\n"				//捕获上一个着色器返回的变量
			"out vec4 FragColor;						\n"				//片段着色器会输出一个4分量的数据
			"void main()								\n"
			"{											\n"
			"FragColor = veccolor;						\n"				//如果不指定颜色则会输出黑色
			"} ";
	members[index++] = str;
	str = "#version 330 core						\n	"
		"in vec4  veccolor;							\n"				//捕获上一个着色器返回的变量
		"uniform vec4 globalColor;					\n"				//定义一个全局的变量，可在c++程序中赋值等操作					
		"out vec4 FragColor;						\n"				//片段着色器会输出一个4分量的数据
		"void main()								\n"
		"{											\n"
		"FragColor = globalColor;					\n"				//如果不指定颜色则会输出黑色
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
	int shader = glCreateShader(type);					//首先创建一个着=器对象 GL_VERTEX_SHADER 顶点着色器
	glShaderSource(shader, 1, &typeGLSL, NULL);			//附加着色器对象
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);				//用于检测着色器是否成功
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);				//打印错误日志
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
	glLinkProgram(shaderProgram);							//建立各个着色器的链接	顶点着色器	片段着色器之间
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
		glDeleteShader(iter);							//链接好之后删除着色器对象
	return true;
}

void setFilMode(const int& mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void createEBO(unsigned int*indexbuffer, unsigned int& EBO)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);				//绑定到顶点索引对象（另一个为顶点缓冲对象）
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,24, indexbuffer, GL_STATIC_DRAW);

}
void disableVertexObj()
{
	glBindVertexArray(0);									//取消顶点数组的绑定
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
	stbi_set_flip_vertically_on_load(true);	//加载图像之前反转y轴，opengl要求y轴0，0位于图像的底部，但是使用库读取的时候0，0是图像的顶部，会造成图像反转的情况
	img.datas = stbi_load(imgpath.c_str(), &img.width, &img.height, &img.nrChannels, 0);
	if (img.datas == NULL)
		return false;
	return true;
}