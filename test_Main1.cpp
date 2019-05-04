
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
	Shader ourshader("../ShaderFiles/shader_img.vs", "../ShaderFiles/shader_img.fs");
	if (!ourshader.isSuccess())
		return -1;
	unsigned int VAO, VBO, EBO;
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
	  0, 1, 3, // first triangle
	  1, 2, 3  // second triangle
	};
	glGenVertexArrays(1, &VAO);			//����һ�������������
	glGenBuffers(1, &VBO);				//����һ�����㻺�����
	glGenBuffers(1, &EBO);				//����һ��������������
	glBindVertexArray(VAO);				//��������������Ӧ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	//�󶨶��㻺�嵽���㻺�������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		//��ֵ�����������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//������ɫ��
	//0:location = 0 
	/**
		glVertexAttribPointer
		���ܣ�������������
		0��location = 0�����ö������Ե�λ��Ϊ0
		3��ָ���������ԵĴ�С������Ϊvec3 ���Ϊ3
		8 * sizeof(float)����������һ����������λ��
		(void*)0����ʼλ�ã���ʼƫ����
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//���ö�������
	//��ɫ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	imgData img, img2;
	if (!loadImage("../img/face.jpg", img) || !loadImage("../img/wall.jpg", img2))
		return -1;
	unsigned int texture1,texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.datas);
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
	//setFilMode(GL_FILL);			//������Ⱦģʽ
	//disableVertexObj();
	cout << "max vertex:" << getMaxVertexAttribute() << endl;;
	ourshader.use();
	glUniform1i(glGetUniformLocation(ourshader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(ourshader.ID, "texture2"), 1);
	while (!glfwWindowShouldClose(window))
	{

		glActiveTexture(GL_TEXTURE0);			//ʹ�õ�һ��������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture1);	//�󶨵���һ��������

		glActiveTexture(GL_TEXTURE1);			//ʹ�õ�һ��������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture2);	//�󶨵���һ��������

		ourshader.use();
		glBindVertexArray(VAO);					//���ݱ�ʾ������Ӧ�Ķ����������
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