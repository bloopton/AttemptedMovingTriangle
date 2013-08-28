#include <GL\glew.h>
#include <GLFW\glfw3.h>

const char* VertexShader=

	"#version 330 \n"//opengl 3.3.
	"layout(location = 0) in vec2 coord;"//inputs information about initial vector location
	"uniform vec2 displacement;"
	"void main()"
	"{"
	"	vec2 position = coord + displacement;"	 /*you forgot the semicolon after this statment*/
	"	gl_Position = vec4(position, 0.0, 1.0);" /*gl_Position not gl_position*/
	"}";


const char* FragmentShader=

	"#version 330 \n"
	"out vec3 color;"//exports information about the color to display on screen
	"void main()"
	"{"
	"	color = vec3(1.0, 1.0, 0.0);" /*fail, you used a : instead of a ;*/
	"}";

/*you have to realize thet because the shaders are compiled by the gpu and not visual studio you
  won't be warned/told by the computer of any errors in your shader code so revise it carefully*/

int main()
{
	glfwInit();
	
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); //window is resizable


		GLFWwindow*openglWindow;//creates pointer called openglWindow

	openglWindow=glfwCreateWindow(800, 600, "OpenGl Window", NULL, NULL); //window parameters
	if(!openglWindow)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(openglWindow);
	
	/*initiate glew after the window is set to current*/
	glewExperimental = true;
	glewInit();


	GLuint simpleVertexShader= glCreateShader (GL_VERTEX_SHADER); //creates an GLuint  shader using the vertex shader variable
	GLuint simpleFragmentShader= glCreateShader (GL_FRAGMENT_SHADER);//same but for fragment shader

	glShaderSource(simpleVertexShader, 1, &VertexShader, 0);//tells the newly created vertex shader to go up to where VertexShader is and read the strings
	glCompileShader(simpleVertexShader);//compiles the shader

	glShaderSource(simpleFragmentShader, 1, &FragmentShader, 0);
	glCompileShader(simpleFragmentShader);

	GLuint simpleShaderProgram = glCreateProgram(); //creates a program to process the shaders
	glAttachShader(simpleShaderProgram, simpleVertexShader); //links the Vertex Shader to the shader program
	glAttachShader(simpleShaderProgram, simpleFragmentShader);

	glLinkProgram(simpleShaderProgram);//compiles and creates the actual program out of the two attached shaders

	glDeleteShader(simpleVertexShader);//now that they are in the program, the individual shaders are no longer needed
	glDeleteProgram(simpleFragmentShader);

	glUseProgram(simpleShaderProgram); //sets simpleShaderProgram as the current shader program


	GLuint TriangleVertexArray;//creates a variable to hold a vertex array
	glGenVertexArrays(1, &TriangleVertexArray);//means ONE vertex array us created, and it will be assigned to TriangleVertexArray
	glBindVertexArray(TriangleVertexArray);//similar to glUseProgram, sets TriangleVertexArray as the current Vertex Array

	GLfloat TriangleVertices[] = 
	{
		-0.1f,-0.1f,
		0.0f, 0.1f,
		0.1f, -0.1f /*changed .1 to -.1*/
	};


	GLuint TriangleBuffer; //creates a variable to hold the value of buffer
	glGenBuffers(1,&TriangleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, TriangleBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, 0);

	

	float verticalDisplacement = 0;
	float horizontalDisplacement = 0;

	GLuint displacementHandle = glGetUniformLocation(simpleShaderProgram,"displacement");

	while (!glfwWindowShouldClose(openglWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniform2f(displacementHandle, horizontalDisplacement, verticalDisplacement);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		if(glfwGetKey(openglWindow, GLFW_KEY_W)) verticalDisplacement += .0001f;
		if(glfwGetKey(openglWindow, GLFW_KEY_S)) verticalDisplacement -= .0001f;
		if(glfwGetKey(openglWindow, GLFW_KEY_D)) horizontalDisplacement += .0001f;
		if(glfwGetKey(openglWindow, GLFW_KEY_A)) horizontalDisplacement -= .0001f;

		if(verticalDisplacement > 1.2f) verticalDisplacement=-1.1f;
		if(verticalDisplacement < -1.2f) verticalDisplacement=1.1f; /*you used a > symbol instead of a < causing the triangle to constantly move up*/
		if(horizontalDisplacement > 1.2f) horizontalDisplacement=-1.1f;
		if(horizontalDisplacement < -1.2f) horizontalDisplacement=1.1f; /*you used a > symbol instead of a < causing the triangle to constantly move left*/



	glfwSwapBuffers(openglWindow);
	glfwPollEvents();
	}

	glDeleteProgram(simpleShaderProgram);
	glDeleteBuffers(1,&TriangleBuffer);
	glDeleteVertexArrays(1,&TriangleVertexArray);
	glfwTerminate();
	return 0;
}


