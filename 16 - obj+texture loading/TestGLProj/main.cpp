#include <GL/glew.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"
#include "Shader.h"
#include "QuatCamera.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace glm;
Shader shader;
Model *mesh;
Model* mesh2;
Model* mesh3;
Model* mesh4;
Model* mesh5;
Model* meshspot;
Model *cube;
Model *torus;
Model *sphere;
Model *cylinder;
Model *cylinder2;
Model *cylinder3;
Model *plane;
Model *plane2;
Model* Wall[4];
Model *gun;
Model* monkeysphere;
Model* t1;
Model* t2;
Model* t3;
Model* t4;
Model* t5;
Model* t6;
Model* t7;
Model* lamp;
Model* lamp2;
Model* lamp3;
Model* lamp4;
Model* reticle;
Model* bullet;
Model* coin;
glm::mat4 shootModel;
glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
bool drawTorus = true;
bool spin = false;
float rotation = 0.0f;
float up_down = 1.0f;
float down_up = 0.5f;
float right_left = 0.0f;
bool limit_up = false;
bool limit_down = false;
bool on = false;
bool shoot = false;
bool start = true;
bool tflag1 = false;
bool tflag2 = false;
bool tflag3 = false;
bool tflag4 = false;
bool tflag5 = false;
bool tflag6 = false;
bool tflag7 = false;
glm::vec3 shootDir;
glm::vec4 lightPosition = glm::vec4(0.0f,7.0f,-10.0f,1.0f);
glm::vec4 lightPosition2 = glm::vec4(7.0f, 5.7f, 20.0f, 1.0f);
glm::vec4 lightPosition3 = glm::vec4(-7.0f, 5.7f, 20.0f, 1.0f);
glm::vec4 lightPosition4 = glm::vec4(-22.0f, 5.7f, -50.0f, 1.0f);
glm::vec4 lightPosition5 = glm::vec4(22.0f, 5.7f, -50.0f, 1.0f);
glm::vec4 lightPositionSpot = glm::vec4(0.0f, 0.0f, -4.0f, 1.0f);

QuatCamera * camera;

/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
	  std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}

void initShader(void)
{
	shader.InitializeFromFile("shaders/phong3.vert", "shaders/phong3.frag");
	checkError ("initShader");
}


void initRendering(void)
{
	glClearColor (0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue
	checkError ("initRendering");
}

void init(void) 
{	
	// View  positioned back -5 on the z axis, looking into the screen.
	glm::vec3 initpos = glm::vec3(0.0f, 0.0f, -10.0f);
	glm::vec3 initlookatpnt = glm::vec3(.0f, .0f, -1.0f);
	camera = new QuatCamera(800,600,initpos, initlookatpnt, glm::vec3(0.0f, 1.0f, 0.0f));
	// Perspective projection matrix.
	projection = glm::perspective(45.0f, 800.0f/600.0f, 1.0f, 1000.0f);

	// Load identity matrix into model matrix.
	model = glm::mat4();

	initShader ();
	initRendering ();
}

void dumpInfo(void)
{
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//camera->OnRender();

	view = glm::lookAt(camera->GetPos(), camera->GetLookAtPoint(), camera->GetUp());
	
	 rotation += 0.05f; // Update rotation angle if rotation is enabled.

	 if (down_up >= 1.5f)
	 {
		 limit_up = true;
	 }
	 else if (down_up <= -0.5f)
	 {
		 limit_up = false;
	 }

	 if (up_down <= 0.0f)
	 {
		 limit_down = true;
	 }
	 else if (up_down >= 2.0f)
	 {
		 limit_down = false;
	 }

	 if (limit_up)
	 {
		 down_up -= 0.005f;
	 }
	 else
	 {
		 down_up += 0.005f;
	 }

	 if (limit_down)
	 {
		 up_down += 0.005f;
	 }
	 else
	 {
		 up_down -= 0.005f;
	 }
	 
	

	 glm::vec4 lightPos = glm::rotate(0.0f,0.0f, 0.0f, 1.0f) * lightPosition;
	 glm::vec4 lightPos2 = glm::rotate(0.0f, 0.0f, 0.0f, 1.0f) * lightPosition2;
	 glm::vec4 lightPos3 = glm::rotate(0.0f, 0.0f, 0.0f, 1.0f) * lightPosition3;
	 glm::vec4 lightPos4 = glm::rotate(0.0f, 0.0f, 0.0f, 1.0f) * lightPosition4;
	 glm::vec4 lightPos5 = glm::rotate(0.0f, 0.0f, 0.0f, 1.0f) * lightPosition5;
	 lightPositionSpot = glm::vec4(0.0f, 0.0f, 10.5f, 1.0f);
	 glm::vec4 lightPosSpot = glm::rotate(0.0f, 0.0f, 0.0f, 1.0f) * lightPositionSpot;
	
	shader.Activate(); // Bind shader.
	shader.SetUniform("lightPosition", view*lightPos);
	shader.SetUniform("lightPosition2", view * lightPos2);
	shader.SetUniform("lightPosition3", view * lightPos3);
	shader.SetUniform("lightPosition4", view * lightPos4);
	shader.SetUniform("lightPosition5", view * lightPos5);
	shader.SetUniform("lightDiffuse", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("linearAttenuationCoefficient", .3f);
	shader.SetUniform("lightPositionSpot", lightPosSpot);
	shader.SetUniform("lightDiffuseSpot", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightSpecularSpot", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightAmbientSpot", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("shininessSpot", 20.0f);
	shader.SetUniform("spotExponent", 100.0f);
	shader.SetUniform("linearAttenuationCoefficientSpot", .1f);

	if (on)
	{
		shader.SetUniform("on", 1.0f);
	}
	else
	{
		shader.SetUniform("on", 0.0f);
	}

	shader.SetUniform("cutOffAngle", glm::radians(10.f));
	shader.SetUniform("d", glm::vec4(0.0, 0.0, -1.0, 0.0));

bool useMat = false;
	
	//plane2->render(view * model * glm::rotate(-45.0f,1.0f,0.0f,0.0f),projection, true); // Render current active model

	/*
	cylinder->setOverrideDiffuseMaterial( glm::vec4(1.0, 0.0, 0.0, 1.0));
	cylinder->setOverrideAmbientMaterial(  glm::vec4(0.0, 0.0, 0.0, 1.0));
	cylinder->setOverrideSpecularMaterial( glm::vec4(1.0, 1.0, 1.0, 1.0));
	cylinder->setOverrideSpecularShininessMaterial( 90.0f);
	cylinder->setOverrideEmissiveMaterial(  glm::vec4(0.0, 0.0, 0.0, 1.0));
	cylinder->render(view*glm::translate(0.0f,5.0f,0.0f)*glm::rotate(180.0f,1.0f,0.0f,0.0f), projection, useMat);
	*/
	cylinder2->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	cylinder2->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	cylinder2->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	cylinder2->setOverrideSpecularShininessMaterial(90.0f);
	cylinder2->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	for (int i = 7; i < 22; i += 5) {
		cylinder2->render(view * glm::translate((float)i, 3.0f, 20.0f) * glm::scale(5.9f, 5.0f, 5.7f) *glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	}
	for (int i = -20; i < 0; i += 5) {
		cylinder2->render(view * glm::translate(7.0f, 3.0f, -(float)i) * glm::scale(5.9f, 5.0f, 5.7f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	}
	//cylinder2->render(view * glm::translate(3.0f, 0.0f, 0.0f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	for (int i = -20; i < 50; i += 5) {
		cylinder2->render(view * glm::translate(22.0f, 3.0f, -(float)i) * glm::scale(5.9f, 5.0f, 5.7f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	}
	for (int i = 22; i > 0; i -= 5) {
		cylinder2->render(view * glm::translate((float)i, 3.0f, -50.0f) * glm::scale(5.9f, 5.0f, 5.7f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	}

	cylinder2->render(view* glm::translate(3.44f, -1.5f, 4.0f) * glm::scale(2.6f, 2.0f, 2.8f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	cylinder2->render(view * glm::translate(1.17f, -1.5f, 4.0f) * glm::scale(2.6f, 2.0f, 2.8f) *glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);

	cylinder3->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	cylinder3->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	cylinder3->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	cylinder3->setOverrideSpecularShininessMaterial(90.0f);
	cylinder3->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));

	for (int i = -7; i > -22; i -= 5) {
		cylinder3->render(view * glm::translate((float)i, 3.0f, 20.0f) * glm::scale(5.9f, 5.0f, 5.7f)* glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	}
	for (int i = -20; i < 0; i += 5) {
		cylinder3->render(view * glm::translate(-7.0f, 3.0f, -(float)i) * glm::scale(5.9f, 5.0f, 5.7f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	}
	for (int i = -20; i < 50; i += 5) {
		cylinder3->render(view * glm::translate(-22.0f, 3.0f, -(float)i) * glm::scale(5.9f, 5.0f, 5.7f)* glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	}
	for (int i = -22; i < 0; i += 5) {
		cylinder3->render(view * glm::translate((float)i, 3.0f, -50.0f) * glm::scale(5.9f, 5.0f, 5.8f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	}
	//cylinder3->render(view * glm::translate(-3.0f, 0.0f, 0.0f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);

	cylinder3->render(view * glm::translate(-3.4f, -1.5f, 4.0f) * glm::scale(2.6f, 2.0f, 2.8f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);
	cylinder3->render(view * glm::translate(-1.1f, -1.5f, 4.0f) * glm::scale(2.6f, 2.0f, 2.8f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f), projection, useMat);

	lamp->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	lamp->setOverrideAmbientMaterial(glm::vec4(.01, 0.01, 0.01, 1.0));
	lamp->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	lamp->setOverrideSpecularShininessMaterial(90.0f);
	lamp->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));

	lamp->render(view * glm::translate(7.0f, 4.8f, 20.0f) * glm::scale(10.0f, 10.f, 10.0f) * glm::rotate(0.0f, 1.0f, 0.0f, 0.0f), projection, useMat);

	lamp2->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 0.0, 1.0));
	lamp2->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	lamp2->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	lamp2->setOverrideSpecularShininessMaterial(90.0f);
	lamp2->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));

	lamp2->render(view * glm::translate(-7.0f, 4.8f, 20.0f) * glm::scale(10.0f, 10.f, 10.0f) * glm::rotate(0.0f, 1.0f, 0.0f, 0.0f), projection, useMat);

	lamp3->setOverrideDiffuseMaterial(glm::vec4(1.0, 0.0, 1.0, 1.0));
	lamp3->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	lamp3->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	lamp3->setOverrideSpecularShininessMaterial(90.0f);
	lamp3->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));

	lamp3->render(view * glm::translate(22.0f, 4.8f, -50.0f) * glm::scale(10.0f, 10.f, 10.0f) * glm::rotate(0.0f, 1.0f, 0.0f, 0.0f), projection, useMat);

	lamp4->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 0.0, 1.0));
	lamp4->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	lamp4->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	lamp4->setOverrideSpecularShininessMaterial(90.0f);
	lamp4->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));

	lamp4->render(view * glm::translate(-22.0f, 4.8f, -50.0f) * glm::scale(10.0f, 10.f, 10.0f) * glm::rotate(0.0f, 1.0f, 0.0f, 0.0f), projection, useMat);

	plane->setOverrideDiffuseMaterial( glm::vec4(1., 1.0, 1.0, 1.0));
	plane->setOverrideAmbientMaterial(  glm::vec4(0.0 , 0.0, 0.0, 1.0));
	plane->setOverrideSpecularMaterial( glm::vec4(1.0, 1.0, 1.0, 1.0));
	plane->setOverrideSpecularShininessMaterial( 90.0f);
	plane->setOverrideEmissiveMaterial( glm::vec4(0.0, 0.0, 0.0, 1.0));
	plane->render(view*glm::translate(-20.0f,-5.0f,-48.0f)*glm::scale(40.0f,1.0f,50.0f), projection, useMat);


	plane2->setOverrideDiffuseMaterial(glm::vec4(1., 1.0, 1.0, 1.0));
	plane2->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	plane2->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	plane2->setOverrideSpecularShininessMaterial(90.0f);
	plane2->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	plane2->render(view * glm::translate(-20.0f, 15.0f, 30.0f) * glm::rotate(180.0f, 1.0f, 0.0f, 0.0f) * glm::scale(40.0f, 1.0f, 50.0f), projection, useMat);
	
	//Front wall
	Wall[0]->setOverrideDiffuseMaterial(glm::vec4(1., 1.0, 1.0, 1.0));
	Wall[0]->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	Wall[0]->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	Wall[0]->setOverrideSpecularShininessMaterial(90.0f);
	Wall[0] ->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	Wall[0] ->render(view * glm::translate(-20.0f, -5.0f, 41.f) * glm::rotate(270.0f, 1.0f, 0.0f, 0.0f) * glm::scale(40.0f, 1.f, 15.0f), projection, useMat);

	//Back Wall
	Wall[1]->setOverrideDiffuseMaterial(glm::vec4(1., 1.0, 1.0, 1.0));
	Wall[1]->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	Wall[1]->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	Wall[1]->setOverrideSpecularShininessMaterial(90.0f);
	Wall[1]->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	Wall[1]->render(view * glm::translate(-20.0f, 15.0f, -60.f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f) * glm::scale(40.0f, 1.f, 15.0f), projection, useMat);

	//Right Wall 
	Wall[2]->setOverrideDiffuseMaterial(glm::vec4(1., 1.0, 1.0, 1.0));
	Wall[2]->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	Wall[2]->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	Wall[2]->setOverrideSpecularShininessMaterial(90.0f);
	Wall[2]->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	Wall[2]->render(view* glm::translate(-40.0f, 20.0f, 10.f)* glm::rotate(90.0f, 1.0f, 0.0f, 0.0f)* glm::rotate(-90.0f, 0.0f, 0.0f, 1.0f) * glm::scale(60.0f, 1.f, 15.0f), projection, useMat);


	//Left Wall 
	Wall[3]->setOverrideDiffuseMaterial(glm::vec4(1., 1.0, 1.0, 1.0));
	Wall[3]->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	Wall[3]->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	Wall[3]->setOverrideSpecularShininessMaterial(90.0f);
	Wall[3]->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	Wall[3]->render(view * glm::translate(40.0f, -10.0f, 10.f) * glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::rotate(90.0f, 0.0f, 0.0f, 1.0f) * glm::scale(60.0f, 1.f, 15.0f), projection, useMat);


	mesh->setOverrideEmissiveMaterial( glm::vec4(1.0, 1.0, 1.0, 1.0));
	//mesh->render(view * glm::translate(lightPos.x,lightPos.y, lightPos.z)*glm::scale(.1f,.1f,.1f), projection, false);

	mesh2->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	mesh2->render(view * glm::translate(lightPos2.x, lightPos2.y, lightPos2.z) , projection, false);

	mesh3->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	mesh3->render(view * glm::translate(lightPos3.x, lightPos3.y, lightPos3.z) , projection, false);

	mesh4->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	mesh4->render(view * glm::translate(lightPos4.x, lightPos4.y, lightPos4.z) , projection, false);

	mesh5->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	mesh5->render(view * glm::translate(lightPos5.x, lightPos5.y, lightPos5.z) , projection, false);

	//mesh2->render(view * model, projection, true); // Render current active model.

	if (shoot) {
		shootDir += vec3(0.0, 0.0, 0.0);
		float scale = .5;
		shootModel *= translate(shootDir * scale);
		//shader.SetUniform("surfaceDiffuse", glm::vec4(1.0, 0.0, 1.0, 1.0));
		bullet->setOverrideDiffuseMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));
		//shader.SetUniform("surfaceAmbient", glm::vec4(0.0, 0.0, 1.0, 1.0));
		bullet->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));

		vec4 bulletPos = shootModel[3];
		vec4 targetPos = glm::vec4(2.8f, up_down, 18.0f, 1.0f);
		vec4 targetPos2 = glm::vec4(-3.f, up_down, 20.0f,1.0f);
		vec4 targetPos3 = glm::vec4(-0.1f, down_up, 23.0f, 1.0f);
		vec4 targetPos4 = glm::vec4(14.0f, 1.0f, 15.0f, 1.0f);
		vec4 targetPos5 = glm::vec4(-14.0f, -2.0f, 15.0f, 1.0f);
		vec4 targetPos6 = glm::vec4(-2.0f, 1.0f, -45.0f, 1.0f);
		vec4 targetPos7 = glm::vec4(2.0f, -2.5f, -45.0f, 1.0f);
		float dist = glm::length(bulletPos - targetPos);
		float dist2 = glm::length(bulletPos - targetPos2);
		float dist3 = glm::length(bulletPos - targetPos3);
		float dist4 = glm::length(bulletPos - targetPos4);
		float dist5 = glm::length(bulletPos - targetPos5);
		float dist6 = glm::length(bulletPos - targetPos6);
		float dist7 = glm::length(bulletPos - targetPos7);
		//printf("%f| %f, %f, %f \n", dist, bulletPos.x, bulletPos.y, bulletPos.z );
		if (dist < 1.0) { // collision detection!
			//shader.SetUniform("surfaceEmissive", glm::vec4(1.0, 0.0, 1.0, 1.0));
			t1->setOverrideEmissiveMaterial(glm::vec4(1.0, 0.0, 1.0, 1.0));
			tflag1 = true;
		}

		if (dist2 < 1.0) { // collision detection!
			//shader.SetUniform("surfaceEmissive", glm::vec4(1.0, 0.0, 1.0, 1.0));
			t2->setOverrideEmissiveMaterial(glm::vec4(1.0, 0.0, 1.0, 1.0));
			tflag2 = true;
		}


		if (dist3 < 1.0) { // collision detection!
			//shader.SetUniform("surfaceEmissive", glm::vec4(1.0, 0.0, 1.0, 1.0));
			t3->setOverrideEmissiveMaterial(glm::vec4(1.0, 0.0, 1.0, 1.0));
			tflag3 = true;;
		}

		if (dist4 < 1.0) { // collision detection!
			//shader.SetUniform("surfaceEmissive", glm::vec4(1.0, 0.0, 1.0, 1.0));
			t4->setOverrideEmissiveMaterial(glm::vec4(1.0, 0.0, 1.0, 1.0));
			tflag4 = true;
		}

		if (dist5 < 1.0) { // collision detection!
			//shader.SetUniform("surfaceEmissive", glm::vec4(1.0, 0.0, 1.0, 1.0));
			t5->setOverrideEmissiveMaterial(glm::vec4(1.0, 0.0, 1.0, 1.0));
			tflag5 = true;
		}

		if (dist6 < 1.0) { // collision detection!
			//shader.SetUniform("surfaceEmissive", glm::vec4(1.0, 0.0, 1.0, 1.0));
			t6->setOverrideEmissiveMaterial(glm::vec4(1.0, 0.0, 1.0, 1.0));
			tflag6 = true;
		}

		if (dist7 < 1.0) { // collision detection!
			//shader.SetUniform("surfaceEmissive", glm::vec4(1.0, 0.0, 1.0, 1.0));
			t7->setOverrideEmissiveMaterial(glm::vec4(1.0, 0.0, 1.0, 1.0));
			tflag7 = true;
		}

		bullet->render(view * shootModel * glm::scale(0.05f, 0.05f, 0.05f), projection, false);

	}
	if (start)
	{
		t1->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		t2->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		t3->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		t4->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		t5->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		t6->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		t7->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		start = false;
	}
		if (tflag1 && tflag2 && tflag3 && tflag4 && tflag5 && tflag6 && tflag7)
		//if (true)
	{
		coin->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		coin->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 0.0, 1.0));
		coin->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
		coin->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		coin->setOverrideSpecularShininessMaterial(90.0f);
		coin->render(view * glm::translate(18.0f,10.0f, 25.0f)  * glm::rotate(90.0f, 1.0f, .0f, 0.0f) * glm::rotate(180.0f, .0f, .0f, 1.0f) * glm::scale(9.5f, 9.5f, 9.5f), projection, useMat);
	}



	t1->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t1->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	t1->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t1->setOverrideSpecularShininessMaterial(90.0f);
	t1->render(view * glm::translate(2.8f, up_down, 18.0f) * glm::rotate(90.0f, 1.0f,.0f, 0.0f) * glm::rotate(180.0f, .0f, .0f, 1.0f) * glm::scale(0.3f, 0.3f, 0.3f), projection, useMat);

	t2->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t2->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	t2->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t2->setOverrideSpecularShininessMaterial(90.0f);
	t2->render(view * glm::translate(-3.f, up_down, 20.0f)* glm::rotate(90.0f, 1.0f, .0f, 0.0f)* glm::rotate(180.0f, .0f, .0f, 1.0f)* glm::scale(0.3f, 0.3f, 0.3f), projection, useMat);

	t3->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t3->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	t3->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t3->setOverrideSpecularShininessMaterial(90.0f);
	t3->render(view * glm::translate(-0.1f, down_up, 23.0f)* glm::rotate(90.0f, 1.0f, .0f, 0.0f)* glm::rotate(180.0f, .0f, .0f, 1.0f)* glm::scale(0.3f, 0.3f, 0.3f), projection, useMat);

	t4->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t4->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	t4->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t4->setOverrideSpecularShininessMaterial(90.0f);
	t4->render(view * glm::translate(14.0f, 1.0f, 15.0f)* glm::rotate(90.0f, 1.0f, .0f, 0.0f)* glm::rotate(180.0f, .0f, .0f, 1.0f)* glm::scale(0.3f, 0.3f, 0.3f), projection, useMat);

	t5->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t5->setOverrideAmbientMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	t5->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t5->setOverrideSpecularShininessMaterial(90.0f);
	t5->render(view * glm::translate(-14.0f, -2.0f, 15.0f)* glm::rotate(90.0f, 1.0f, .0f, 0.0f)* glm::rotate(180.0f, .0f, .0f, 1.0f)* glm::scale(0.3f, 0.3f, 0.3f), projection, useMat);

	t6->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t6->setOverrideAmbientMaterial(glm::vec4(0.01, 0.01, 0.01, 1.0));
	t6->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t6->setOverrideSpecularShininessMaterial(90.0f);
	t6->render(view * glm::translate(-2.0f, 1.0f, -45.0f) * glm::rotate(90.0f, 1.0f, .0f, 0.0f) * glm::scale(0.3f, 0.3f, 0.3f), projection, useMat);

	t7->setOverrideDiffuseMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t7->setOverrideAmbientMaterial(glm::vec4(0.01, 0.01, 0.01, 1.0));
	t7->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	t7->setOverrideSpecularShininessMaterial(90.0f);
	t7->render(view * glm::translate(2.0f, -2.5f, -45.0f) * glm::rotate(90.0f, 1.0f, .0f, 0.0f) * glm::scale(0.3f, 0.3f, 0.3f), projection, useMat);

	// Render current active model.

	glDisable(GL_DEPTH_TEST);
	meshspot->setOverrideEmissiveMaterial(glm::vec4(1.0, 0.0, 0.0, 1.0));
	meshspot->render(glm::translate(1.0f, -1.0f, -4.5f)* glm::scale(.1f, .1f, .1f), projection, false);
	reticle->setOverrideEmissiveMaterial(glm::vec4(1.0, 1.0, 1.0, 0.5f));
	reticle->render(glm::translate(0.0f, 0.0f, -10.0f)* glm::scale(.1f, .1f, .1f), projection, false);
	gun->render(glm::translate(1.0f, -1.0f, -2.0f)* glm::scale(.05f, .05f, .05f)* glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f), projection, true);
	glEnable(GL_DEPTH_TEST);

	glutSwapBuffers(); // Swap the buffers.

	checkError ("display");
}

void idle()
{
	glutPostRedisplay();
}


void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	checkError ("reshape");
}

bool useMouseCamera = false;
void specialKeyboard(int Key, int x, int y)
{
	
		camera->OnKeyboard(Key);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'c':
		useMouseCamera = !useMouseCamera;
		break;
	case 'w':
		camera->OnKeyboardchar('w');
		break;
	case 's':
		camera->OnKeyboardchar('s');
		break;
	case 'a':
		camera->OnKeyboardchar('a');
		break;
	case 'd':
		camera->OnKeyboardchar('d');
		break;
	case 'f':
		on = !on;
		break;

   }
}

static void passiveMouse(int x, int y)
{
if(useMouseCamera)
  camera->OnMouse(x, y);
}

void shootSphere(int x, int y) {
	GLint viewport[] = { 0,0,0,0 };
	glGetIntegerv(GL_VIEWPORT, viewport);
	vec3 pointOnNear = glm::unProject(vec3(x, viewport[3] - y, 0.0), view, projection, vec4(viewport[0], viewport[1], viewport[2], viewport[3]));
	vec3 pointOnFar = glm::unProject(vec3(x, viewport[3] - y, 1.0), view, projection, vec4(viewport[0], viewport[1], viewport[2], viewport[3]));
	shootDir = normalize(pointOnFar - pointOnNear);
	shootModel = translate(pointOnNear);
	shoot = true;


}
void MouseButton(int button, int state, int x, int y)
{
	// Respond to mouse button presses.
	// If button1 pressed, mark this state so we know in motion function.

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		shootSphere(x, y);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	//glutInitContextVersion (1,5);
	//glutInitContextProfile ( GLUT_COMPATIBILITY_PROFILE );
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Lighting and Quaternion Camera Demo");
	
	//glutGameModeString("800x600:8");
	//glutEnterGameMode();
	glewInit();
	dumpInfo ();
	init ();

	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc(specialKeyboard);
    glutPassiveMotionFunc(passiveMouse);
	glutMouseFunc(MouseButton);
	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_DEPTH_TEST);

	torus = new Model(&shader,"models/torus.obj",  "models/");
	cube = new Model(&shader,"models/cube.obj",  "models/");
	sphere = new Model(&shader,"models/sphere.obj", "models/");
	plane = new Model(&shader,"models/texcube.obj",  "models/");
	cylinder = new Model( &shader, "models/lamp4.obj", "models/");
	cylinder2 = new Model(&shader, "models/barrelNew.obj", "models/");
	cylinder3 = new Model(&shader, "models/BarrelNewOBJ.obj", "models/");
	plane2 = new Model(&shader,"models/texcube.obj",  "models/");
	monkeysphere = new Model(&shader, "models/monkeysphere.obj", "models/");
	reticle = new Model(&shader, "models/torus.obj", "models/");
	coin = new Model(&shader, "models/Winner.obj", "models/");
	lamp = lamp2 = lamp3 = lamp4 = cylinder;
	Wall[0] = Wall[1] = Wall[2] = Wall[3] = plane2;
	mesh = mesh2 = mesh3 = mesh4 = mesh5 = bullet = sphere;
	meshspot = monkeysphere;
	t1 = new Model(&shader, "models/target.obj", "models/");
	t2 = new Model(&shader, "models/target.obj", "models/");
	t3 = new Model(&shader, "models/target.obj", "models/");
	t4 = new Model(&shader, "models/target.obj", "models/");
	t5 = new Model(&shader, "models/target.obj", "models/");
	t6 = new Model(&shader, "models/target.obj", "models/");
	t7 = new Model(&shader, "models/target.obj", "models/");
	gun = new Model( &shader,"models/m16_1.obj", "models/");

	glutMainLoop();

   return 0;
}