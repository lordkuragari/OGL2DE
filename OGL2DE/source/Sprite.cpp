#include "Sprite.h"

Sprite::Sprite(void)
{
	LoadVertShader("../resources/exampleVert.glsl");
	LoadFragShader("../resources/exampleFrag.glsl");
	LinkShaders();

	m_v4SpriteColor = Vector4(1,1,1,1);

	m_aoVerts[0].Pos = Vector3(	-0.5f,  0.5f,  0.0f);
	m_aoVerts[1].Pos = Vector3(	0.5f,  0.5f,  0.0f);
	m_aoVerts[2].Pos = Vector3(	-0.5f,  -0.5f,  0.0f);
	m_aoVerts[3].Pos = Vector3(	0.5f,  -0.5f,  0.0f);
	
	/*
	m_aoVerts[0].Color = Vector4(1.0f,  0.0f,  0.0f, 1.0f);
	m_aoVerts[1].Color = Vector4(0.0f,  1.0f,  0.0f, 1.0f);
	m_aoVerts[2].Color = Vector4(0.0f,  0.0f,  1.0f, 1.0f);
	m_aoVerts[3].Color = Vector4(1.0f,  1.0f,  1.0f, 1.0f);
	*/


	m_aoVerts[0].Color = m_v4SpriteColor;
	m_aoVerts[1].Color = m_v4SpriteColor;
	m_aoVerts[2].Color = m_v4SpriteColor;
	m_aoVerts[3].Color = m_v4SpriteColor;



	m_aoVerts[0].UV = Vector2(0.0f,  0.0f);
	m_aoVerts[1].UV = Vector2(0.0f,  1.0f);
	m_aoVerts[2].UV = Vector2(1.0f,  0.0f);
	m_aoVerts[3].UV = Vector2(1.0f,  1.0f);

	GLuint elements[] =
	{
		0,1,2,3
	};

	//Gen Buffers
	glGenBuffers(1,&m_VBO);
	glGenBuffers(1,&m_EBO);
	glGenVertexArrays(1, &m_VAO);

	//Bind Buffers
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	//Put Data into buffers
	glBufferData(GL_ARRAY_BUFFER, 4* sizeof(Vertex), m_aoVerts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//Enable some attributes
	GLint posAttrib = glGetAttribLocation(m_ShaderProgram,"position");
	glEnableVertexAttribArray(posAttrib);
	GLint colAttrib = glGetAttribLocation(m_ShaderProgram,"color");
	glEnableVertexAttribArray(colAttrib);
	GLint uvAttrib = glGetAttribLocation(m_ShaderProgram,"texcoord");
	glEnableVertexAttribArray(uvAttrib);


	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));

	glBindVertexArray(0);
	m_v2Scale = Vector2(1,1);


	m_v3Position = Vector3(g_gl_width/2,g_gl_height/2,0);

	modelMatrix = new Matrix4();
	
	*modelMatrix   = Matrix4::IdentityMatrix();
	
	matrix_location = glGetUniformLocation (m_ShaderProgram, "matrix");


	m_uiTexture = 0;

	m_uSourceBlendMode	= GL_SRC_ALPHA;
	m_uDestinationBlendMode = GL_ONE_MINUS_SRC_ALPHA;

	glGenTextures(1, &m_uiTexture);
	glActiveTexture (GL_TEXTURE0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	tex_location = glGetUniformLocation (m_ShaderProgram, "diffuseTexture");

	m_minUVCoords = Vector2( 0.f, 0.f );
	m_maxUVCoords = Vector2( 1.f, 1.f );
	m_uvScale = Vector2( 1.f, 1.f );
	

}


Sprite::~Sprite(void)
{
	delete modelMatrix;
}
Sprite::Sprite( const char* a_pTexture, int a_iWidth, int a_iHeight, Vector4 a_v4Color ,GLFWwindow * window)
{

	GameWindow = window;
	
	LoadVertShader("../resources/exampleVert.glsl");
	LoadFragShader("../resources/exampleFrag.glsl");
	LinkShaders();

	m_v4SpriteColor = a_v4Color;

	m_aoVerts[0].Pos = Vector3(	-0.5f,  0.5f,  0.0f);
	m_aoVerts[1].Pos = Vector3(	0.5f,  0.5f,  0.0f);
	m_aoVerts[2].Pos = Vector3(	-0.5f,  -0.5f,  0.0f);
	m_aoVerts[3].Pos = Vector3(	0.5f,  -0.5f,  0.0f);
	
	/*
	m_aoVerts[0].Color = Vector4(1.0f,  0.0f,  0.0f, 1.0f);
	m_aoVerts[1].Color = Vector4(0.0f,  1.0f,  0.0f, 1.0f);
	m_aoVerts[2].Color = Vector4(0.0f,  0.0f,  1.0f, 1.0f);
	m_aoVerts[3].Color = Vector4(1.0f,  1.0f,  1.0f, 1.0f);
	*/


	m_aoVerts[0].Color = m_v4SpriteColor;
	m_aoVerts[1].Color = m_v4SpriteColor;
	m_aoVerts[2].Color = m_v4SpriteColor;
	m_aoVerts[3].Color = m_v4SpriteColor;



	m_aoVerts[0].UV = Vector2(0.0f,  0.0f);
	m_aoVerts[1].UV = Vector2(0.0f,  1.0f);
	m_aoVerts[2].UV = Vector2(1.0f,  0.0f);
	m_aoVerts[3].UV = Vector2(1.0f,  1.0f);

	GLuint elements[] =
	{
		0,1,2,3
	};

	//Gen Buffers
	glGenBuffers(1,&m_VBO);
	glGenBuffers(1,&m_EBO);
	glGenVertexArrays(1, &m_VAO);

	//Bind Buffers
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	//Put Data into buffers
	glBufferData(GL_ARRAY_BUFFER, 4* sizeof(Vertex), m_aoVerts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//Enable some attributes
	GLint posAttrib = glGetAttribLocation(m_ShaderProgram,"position");
	glEnableVertexAttribArray(posAttrib);
	GLint colAttrib = glGetAttribLocation(m_ShaderProgram,"color");
	glEnableVertexAttribArray(colAttrib);
	GLint uvAttrib = glGetAttribLocation(m_ShaderProgram,"texcoord");
	glEnableVertexAttribArray(uvAttrib);


	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));

	glBindVertexArray(0);
	m_v2Scale = Vector2(a_iWidth,a_iHeight);


	m_v3Position = Vector3(g_gl_width/2,g_gl_height/2,0);

	modelMatrix = new Matrix4();
	
	*modelMatrix   = Matrix4::IdentityMatrix();
	
	matrix_location = glGetUniformLocation (m_ShaderProgram, "matrix");


	m_uiTexture = 0;

	m_uSourceBlendMode	= GL_SRC_ALPHA;
	m_uDestinationBlendMode = GL_ONE_MINUS_SRC_ALPHA;

	glGenTextures(1, &m_uiTexture);
	glActiveTexture (GL_TEXTURE0);

	int width, height;
	unsigned char* image = SOIL_load_image(a_pTexture, &width, &height, 0, SOIL_LOAD_RGBA);
	glBindTexture( GL_TEXTURE_2D,m_uiTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	tex_location = glGetUniformLocation (m_ShaderProgram, "diffuseTexture");

	m_minUVCoords = Vector2( 0.f, 0.f );
	m_maxUVCoords = Vector2( 1.f, 1.f );
	m_uvScale = Vector2( 1.f, 1.f );
	
}

void Sprite::Draw()
{
	glBlendFunc (m_uSourceBlendMode, m_uDestinationBlendMode);
	glUseProgram(m_ShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i (tex_location, 0); 
	
	
	modelMatrix->m_afArray[0]  = m_v2Scale.m_fX;
	modelMatrix->m_afArray[5]  = m_v2Scale.m_fY;
	modelMatrix->m_afArray[12] = m_v3Position.m_fX;
	modelMatrix->m_afArray[13] = m_v3Position.m_fY;
	modelMatrix->m_afArray[14] = m_v3Position.m_fZ;


Matrix4 MVP =  (*Ortho * *modelMatrix) ;

	
//	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, modelMatrix->m_afArray);
//	glUniformMatrix4fv (view_location, 1, GL_FALSE, viewMatrix->m_afArray);
//	glUniformMatrix4fv (proj_location, 1, GL_FALSE, Ortho->m_afArray);

	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, MVP.m_afArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBindVertexArray(m_VAO);


	glDrawElements(GL_TRIANGLE_STRIP, 4,GL_UNSIGNED_INT,0);	
}

void Sprite::Input()
{
	  if (GLFW_PRESS == glfwGetKey(GameWindow, GLFW_KEY_W))
        {
			m_v3Position += Vector3(0.0f, 1.f, 0.0f);
	  }

        if (GLFW_PRESS == glfwGetKey(GameWindow, GLFW_KEY_A))
        {
                m_v3Position += Vector3(-1.f, 0.0f, 0.0f);
        }

        if (GLFW_PRESS == glfwGetKey(GameWindow, GLFW_KEY_S))
        {
			m_v3Position += Vector3(0.0f, -1.f, 0.0f);
		}

        if (GLFW_PRESS == glfwGetKey(GameWindow, GLFW_KEY_D))
        {
                 m_v3Position += Vector3(1.f, 0.0f, 0.0f);
        }
		 if (GLFW_PRESS == glfwGetKey(GameWindow, GLFW_KEY_C))
        {
                 m_v2Scale *= 1 - getDeltaTime();
        }
		  if (GLFW_PRESS == glfwGetKey(GameWindow, GLFW_KEY_Z))
        {
                 m_v2Scale *= 1 + getDeltaTime();
        }

}
void	Sprite::SetUVData()
{
	m_aoVerts[0].UV = Vector2(m_minUVCoords.m_fX/m_uvScale.m_fX,m_minUVCoords.m_fY/m_uvScale.m_fY);
	m_aoVerts[1].UV = Vector2(m_minUVCoords.m_fX/m_uvScale.m_fX,m_maxUVCoords.m_fY/m_uvScale.m_fY);
	m_aoVerts[2].UV = Vector2(m_maxUVCoords.m_fX/m_uvScale.m_fX,m_minUVCoords.m_fY/m_uvScale.m_fY);
	m_aoVerts[3].UV = Vector2(m_maxUVCoords.m_fX/m_uvScale.m_fX,m_maxUVCoords.m_fY/m_uvScale.m_fY);

}

