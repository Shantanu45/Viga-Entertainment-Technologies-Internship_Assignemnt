#include "imports.h"

// ------------------------Globals----------------------------
const float toRadians = 3.14159265f / 180.0f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader Path
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader Path
static const char* fShader = "Shaders/shader.frag";


static std::vector<Mesh*> meshList;
static std::vector<Shader> shaderList;

//------------------------------------------------------------


// ----------------------Declarations ------------------------

void DeltaTime(GLfloat& deltaTime, GLfloat& lastTime);
void CreateObjects();
void CreateShaders();

//------------------------------------------------------------



// MAIN======================================================>>>

int main()
{
	// Create Window
	Window mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	// Camera
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), 
							glm::vec3(0.0f, 1.0f, 0.0f), 
							-90.0f, 0.0f, 5.0f, 0.08f);

	// Textures
	Texture SkinTexture = Texture("Textures/Human-skin-texture-free-download-preview.jpg");
	SkinTexture.LoadTexture(GL_RGB);

	Texture SkinNormalTexture = Texture("Textures/Screenshot (714)_NRM.png");
	SkinNormalTexture.LoadTexture(GL_RGBA);

	Texture SkinDispTexture = Texture("Textures/Screenshot (714)_COLOR.png");
	SkinDispTexture.LoadTexture(GL_RGBA);

	Texture PadTexture = Texture("Textures/Sci-Fi_Padded_Fabric_003_basecolor.jpg");
	PadTexture.LoadTexture(GL_RGB);

	Texture PadNormalTexture = Texture("Textures/Sci-Fi_Padded_Fabric_003_normal.jpg");
	PadNormalTexture.LoadTexture(GL_RGB);

	Texture PadDispTexture = Texture("Textures/Sci-Fi_Padded_Fabric_003_height.png");
	PadDispTexture.LoadTexture(GL_RGBA);

	Texture FabTexture = Texture("Textures/Fabric_029_basecolor.jpg");
	FabTexture.LoadTexture(GL_RGB);

	Texture OrgNormalTexture = Texture("Textures/Abstract_Organic_002_NORM.jpg");
	OrgNormalTexture.LoadTexture(GL_RGB);

	Texture OrgDispTexture = Texture("Textures/Abstract_Organic_002_DISP.png");
	OrgDispTexture.LoadTexture(GL_RGBA);

	Texture GTexture = Texture("Textures/brickwall.jpg");
	GTexture.LoadTexture(GL_RGB);

	Texture GNormalTexture = Texture("Textures/brickwall_normal.jpg");
	GNormalTexture.LoadTexture(GL_RGB);

	Texture GDispTexture = Texture("");
	GDispTexture.LoadTexture(GL_RGBA);

	// Materials
	Material SkinMaterial = Material(0.91f, 16);
	Material PadMaterial = Material(1.0f, 50);
	Material OrgMaterial = Material(1.0f, 70);
	Material GMaterial = Material(0.92f, 32);

	// Lights
	Light mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f,
		-0.5f, 3.0f, 1.0f, 0.9f);

	// Projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		(GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
		0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get + Handle user input events
		glfwPollEvents();

		DeltaTime(deltaTime, lastTime);
		camera.getAllCameraControls(mainWindow.getsKeys(), deltaTime, mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shaderList[0].UseShader();


		mainLight.UseLight(shaderList[0].GetUniformLocation("directionalLight.ambientIntensity"), 
							shaderList[0].GetUniformLocation("directionalLight.colour"),
							shaderList[0].GetUniformLocation("directionalLight.diffuseIntensity"),
							shaderList[0].GetUniformLocation("directionalLight.direction"));

		// Model Indentity Matrix
		glm::mat4 model(1.0f);

		// Transformations
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		// Handling Uniforms
		// model
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("model"), 
							1, GL_FALSE, 
							glm::value_ptr(model));						// Transformations of Mesh 1 Passes here

		// projection
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("projection"), 
							1, GL_FALSE, 
							glm::value_ptr(projection));

		//view
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("view"), 
							1, GL_FALSE, 
							glm::value_ptr(camera.calculateViewMatrix()));

		glUniform3f(shaderList[0].GetUniformLocation("eyePosition"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glUniform1i(shaderList[0].GetUniformLocation("theTexture"), 0);
		glUniform1i(shaderList[0].GetUniformLocation("theNormalTexture"), 1);
		glUniform1i(shaderList[0].GetUniformLocation("theDispTexture"), 2);

		SkinTexture.UseTexture(GL_TEXTURE0);
		SkinNormalTexture.UseTexture(GL_TEXTURE1);
		SkinDispTexture.UseTexture(GL_TEXTURE2);
		SkinMaterial.UseMaterial(shaderList[0].GetUniformLocation("material.specularIntensity"), 
								shaderList[0].GetUniformLocation("material.shininess"));
		// Rendring Mesh
		meshList[0]->RenderMesh();	

		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));

		// model
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("model"),
			1, GL_FALSE,
			glm::value_ptr(model));						

		PadTexture.UseTexture(GL_TEXTURE0);
		PadNormalTexture.UseTexture(GL_TEXTURE1);
		PadDispTexture.UseTexture(GL_TEXTURE2);
		PadMaterial.UseMaterial(shaderList[0].GetUniformLocation("material.specularIntensity"),
			shaderList[0].GetUniformLocation("material.shininess"));

		meshList[1]->RenderMesh();

		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));

		// model
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("model"),
			1, GL_FALSE,
			glm::value_ptr(model));						

		FabTexture.UseTexture(GL_TEXTURE0);
		OrgNormalTexture.UseTexture(GL_TEXTURE1);
		OrgDispTexture.UseTexture(GL_TEXTURE2);
		OrgMaterial.UseMaterial(shaderList[0].GetUniformLocation("material.specularIntensity"),
			shaderList[0].GetUniformLocation("material.shininess"));

		meshList[2]->RenderMesh();

		model = glm::translate(model, glm::vec3(+5.0f, 0.0f, 0.0f));

		// model
		glUniformMatrix4fv(shaderList[0].GetUniformLocation("model"),
			1, GL_FALSE,
			glm::value_ptr(model));						

		GTexture.UseTexture(GL_TEXTURE0);
		GNormalTexture.UseTexture(GL_TEXTURE1);
		GDispTexture.UseTexture(GL_TEXTURE2);
		GMaterial.UseMaterial(shaderList[0].GetUniformLocation("material.specularIntensity"),
			shaderList[0].GetUniformLocation("material.shininess"));

		meshList[3]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}






	return 0;
}

// ===============================================================================>>>




// Function for Adding Mesh
void CreateObjects()
{
	// positions
	glm::vec3 pos1(-2.0f,	2.0f,	0.0f);
	glm::vec3 pos2( -2.0f,   -2.0f,	0.0f);
	glm::vec3 pos3(	2.0f,   -2.0f,	 0.0f);
	glm::vec3 pos4(	2.0f,	2.0f,	 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,

	};

	GLfloat vertices[] = {
	// positions            // normal         // texcoords  // tangent                          // bitangent
	pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 56, 6);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 56, 6);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(vertices, indices, 56, 6);
	meshList.push_back(obj3);


	// Setting Up Ground

	// positions
	glm::vec3 posG1(-8.0f, -2.5f, -8.0f);
	glm::vec3 posG2( 8.0f, -2.5f, -8.0f);
	glm::vec3 posG3( 8.0f, -2.5f,  8.0f);
	glm::vec3 posG4(-8.0f, -2.5f,  8.0f);

	// texture coordinates
	glm::vec2 uvG1(0.0f, 1.0f);
	glm::vec2 uvG2(0.0f, 0.0f);
	glm::vec2 uvG3(1.0f, 0.0f);
	glm::vec2 uvG4(1.0f, 1.0f);

	// normal vector
	glm::vec3 nmG(0.0f, 1.0f, 0.0f);

	GLfloat vertices2[] = {
	// positions				// normal			// texcoords	// tangent                          // bitangent
	posG1.x, posG1.y, posG1.z, nmG.x, nmG.y, nmG.z, uvG1.x, uvG1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	posG2.x, posG2.y, posG2.z, nmG.x, nmG.y, nmG.z, uvG2.x, uvG2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	posG3.x, posG3.y, posG3.z, nmG.x, nmG.y, nmG.z, uvG3.x, uvG3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	posG4.x, posG4.y, posG4.z, nmG.x, nmG.y, nmG.z, uvG4.x, uvG4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};

	Mesh* ground = new Mesh();
	ground->CreateMesh(vertices2, indices, 56, 6);
	meshList.push_back(ground);
}

// Funciton for adding shader
void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

// Function for handling simple deltaTime
void DeltaTime(GLfloat& deltaTime, GLfloat& lastTime) {
	GLfloat now = glfwGetTime(); 
	deltaTime = now - lastTime;
	lastTime = now;
}



