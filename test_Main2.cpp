#if 1
//͸�ӷ���
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//���ö�������
	//����
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
		���ܣ����������Ʒ�ʽ
		GL_TEXTURE_2D��2dͼ��
		GL_TEXTURE_WRAP_S��s����
		GL_REPEAT������Ĭ����Ϊ���ظ�����ͼ��
		GL_MIRRORED_REPEAT���ظ������������
		GL_CLAMP_TO_BORDER����������ʹ�ñ�Ե��ɫ��䣨��Ҫָ����Ե��ɫ��
		---------------------------------------------------------------
		������ˣ�
		GL_LINEAR�����Թ��ˣ������������긽�����ؼ����ֵ�����Ʊ�ʾ
		GL_TEXTURE_MIN_FILTER��������Сʱ
		GL_TEXTURE_MAG_FILTER�����зŴ�ʱ
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//��������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.datas);
	//�Զ����ɶ༶��Զ����
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
	ourshader.setUniformInt("texture1", 0);
	ourshader.setUniformInt("texture2", 1);
	while (!glfwWindowShouldClose(window))
	{

		glActiveTexture(GL_TEXTURE0);			//ʹ�õ�һ������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture1);	//�󶨵���һ��������

		glActiveTexture(GL_TEXTURE1);			//ʹ�õ�һ������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture2);	//�󶨵���һ��������

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

#endif