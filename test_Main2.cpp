#if 1
//透视方法
#include "Common.h"
#include "stb_image.h"
#include "Shader.h"
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
	Shader ourshader("../ShaderFiles/shader_perspective.vs", "../ShaderFiles/shader_perspective.fs");
	if (!ourshader.isSuccess())
		return -1;
	unsigned int VAO, VBO, EBO;
	float vertices[] = {
		// positions        // texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
	  0, 1, 3, // first triangle
	  1, 2, 3  // second triangle
	};
	glGenVertexArrays(1, &VAO);			//创建一个顶点数组对象
	glGenBuffers(1, &VBO);				//创建一个顶点缓冲对象
	glGenBuffers(1, &EBO);				//创建一个顶点索引对象
	glBindVertexArray(VAO);				//对数组对象进行相应绑定
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	//绑定顶点缓冲到顶点缓冲对象上
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		//赋值数组对象数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//顶点着色器
	//0:location = 0 
	/**
		glVertexAttribPointer
		功能：解析顶点数据
		0：location = 0，设置顶点属性的位置为0
		3：指定顶点属性的大小，参数为vec3 因此为3
		8 * sizeof(float)：步长，下一个点索引的位置
		(void*)0：起始位置，起始偏移量
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//启用顶点属性
	//纹理
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	imgData img, img2;
	if (!loadImage("../img/face.jpg", img) || !loadImage("../img/wall.jpg", img2))
		return -1;
	unsigned int texture1,texture2;
	glGenTextures(1, &texture1);				
	glBindTexture(GL_TEXTURE_2D, texture1);
	/**
		glTexParameteri
		功能：设置纹理环绕方式
		GL_TEXTURE_2D：2d图像
		GL_TEXTURE_WRAP_S：s坐标
		GL_REPEAT：纹理默认行为：重复纹理图像
		GL_MIRRORED_REPEAT，重复纹理但镜像放置
		GL_CLAMP_TO_BORDER：超出部分使用边缘颜色填充（需要指定边缘颜色）
		---------------------------------------------------------------
		纹理过滤：
		GL_LINEAR：线性过滤，基于纹理坐标附近像素计算插值，近似表示
		GL_TEXTURE_MIN_FILTER：进行缩小时
		GL_TEXTURE_MAG_FILTER：进行放大时
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//生成纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.datas);
	//自动生成多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(img.datas);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img2.width, img2.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img2.datas);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(img2.datas);
	//setFilMode(GL_FILL);			//设置渲染模式
	//disableVertexObj();
	cout << "max vertex:" << getMaxVertexAttribute() << endl;;
	ourshader.use();
	ourshader.setUniformInt("texture1", 0);
	ourshader.setUniformInt("texture2", 1);
	while (!glfwWindowShouldClose(window))
	{

		glActiveTexture(GL_TEXTURE0);			//使用第一个纹理单元
		glBindTexture(GL_TEXTURE_2D, texture1);	//绑定到第一个纹理上

		glActiveTexture(GL_TEXTURE1);			//使用第一个纹理单元
		glBindTexture(GL_TEXTURE_2D, texture2);	//绑定到第一个纹理上

		ourshader.use();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		
		unsigned int modelLoc = glGetUniformLocation(ourshader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourshader.ID, "view");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourshader.ID,"projection"), 1,GL_FALSE,glm::value_ptr(projection));

		glBindVertexArray(VAO);					//根据表示符绑定相应的顶点数组对象
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	glDeleteBuffers(1, &EBO);
	return 0;
}

#endif