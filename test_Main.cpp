
#include "Common.h"
#include "Shader.h"
const int WIDTH = 800;
const int HEIGHT = 600;
float textCoords[] = {
	0.0f,0.0f,
	1.0f,0.0f,
	0.5f,1.0f
};

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
	Shader ourshader("../ShaderFiles/3.3shader.vs", "../ShaderFiles/3.3shader.fs");
	if (!ourshader.isSuccess())
		return -1;
	unsigned int VAO, VBO, EBO;
	float vertices[] = {
		// positions         // colors
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // top 
		 0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.5f, 1.0f, 0.0f  // bottom left
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//顶点着色器使用的范围
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	setFilMode(GL_FILL);			//设置渲染模式
	disableVertexObj();
	cout << "max vertex:" << getMaxVertexAttribute() << endl;;

	while (!glfwWindowShouldClose(window))
	{
		ourshader.use();
		glBindVertexArray(VAO);					//根据表示符绑定相应的顶点数组对象
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	//glDeleteBuffers(1, &EBO);
	return 0;
}
