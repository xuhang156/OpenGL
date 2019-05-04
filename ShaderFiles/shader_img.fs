#version 330 core
//向几何着色器输出的颜色
out vec4 FragColor;
//从顶点着色器传递过来的变量
in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
// 创建多个纹理单元能够实现图片的叠加渲染，一般计算机允许使用16个纹理单元
void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}