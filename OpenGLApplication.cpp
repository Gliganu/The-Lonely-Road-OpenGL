// OpenGLApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "glut.h"
#include "glm.h"
#include <gl/gl.h>

#include "tga.h"

using namespace std;

GLfloat fvLightAmbient[4] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat fvLightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat fvLightSpecular[4] = { 1.0, 1.0, 1.0, 1.0 };


GLfloat fvTeapotMatAmbient[4] = { 0.7, 0.0, 0.0, 1.0 };
GLfloat fvTeapotMatDiffuse[4] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat fvTeapotMatSpecular[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat fvTeapotMatShininess[1] = { 32.0 };

GLfloat fvGenericMatAmbient[4] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat fvGenericMatDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat fvGenericMatSpecular[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat fvGenericMatShininess[1] = { 64.0 };

//equations of the planes on which the shadow is cast
GLfloat fvFloorPlaneEq[4];

//projected shadows matrices
GLfloat fvFloorShadowMat[16];

//GLfloat fvLightPos[4] = { 1, 1, -4 ,1 };

GLfloat fvLightPos[4] = { -1, -1, -1 ,1 };

int ROAD_LIMIT = 2;

int nrStars = 400;

int screen_width = 640;
int screen_height = 480;

double sceneRotateX = 0;
double sceneRotateY = 0;
double sceneRotateZ = 0;

double sceneTranslateX = 0;
double sceneTranslateY = 0;
double sceneTranslateZ = 0;

int carLife = 50;

static int wireframeOption;

GLfloat g_light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat g_light_diffuse[] = { 0, 0, 0, 1.0 };
GLfloat g_light_specular[] = {0.8, 0.8, 1, 1.0 };


struct Point {
	int x;
	int y;
	int z;
};

vector<Point> lightingPosition;
vector<Point> startPositionVector;

GLfloat angle = 0;

GLMmodel *carModel;
GLMmodel *treeModel;
GLMmodel *castleModel;
GLMmodel *planeModel;
GLMmodel *poleModel;
GLMmodel *waterModel;
GLMmodel *fishModel;
GLMmodel *fenceModel;
GLMmodel *moonModel;

double lightX = 0;
double lightY = 0;
double lightZ = 10;

int viewingMode = 1;

double wheelRotationAngle = 0;

double carFrontDirection = 0;
double carSideDirection = 0;

double carSideAngle = 0;

double wheelAngle = 0;

double carFront = 0;

double diffuseLightIntensity = 0.3;

double fishAngle = 0;
double fishDepth = 0;

double planeFrontDirection = 0;
double planeSideAngle = 0;

double fTextureAnimation = 5;

GLuint treeTexture; //variable used for the ID of the tree texture
GLuint waterTexture; //variable used for the ID of the tree texture
GLuint fireTexture; //variable used for the ID of the tree texture


GLuint skyboxTexture[6]; //variable used for the IDs of the six textures that form the skybox 
GLfloat fSkyboxSizeX, fSkyboxSizeY, fSkyboxSizeZ; //skybox size on X, Y and Z axes
GLfloat fTreeSize;

GLfloat starPosition = 0;

GLfloat sunMaterial[] = { 1.0, 1.0, 0.0, 1.0 };

bool isDay = false;


vector<int> lightSources;

int NUMBER_LIGHTS = 6;

bool fogEnabled = false;

GLfloat fogColor[3] = { 0.5,0.5,0.5 };

//////////////

void initLight();
void initStarPositionVector();
void renderBitmapString(float x, float y, float z, char* string);
void drawModel(GLMmodel **pmodel, char*filename, GLuint mode);
void ComputePlaneEquations();
void ComputeShadowMatrices();
void playMusic();

//////////
void drawSkybox(GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ)
{
	glEnable(GL_TEXTURE_2D); //enable 2D texturing

	//negative x plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]); //select the current texture
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);glVertex3f(-sizeX, sizeY, -sizeZ); //assign each corner of the texture to a 3D vertex in the OpenGL scene
	glTexCoord2f(0, 1);glVertex3f(-sizeX, sizeY, sizeZ); //(0,0) is the left lower corner, while (1,1) is the right upper corner of the texture
	glTexCoord2f(0, 0);glVertex3f(-sizeX, -sizeY, sizeZ);
	glTexCoord2f(1, 0);glVertex3f(-sizeX, -sizeY, -sizeZ);
	glEnd();

	//negative y plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);glVertex3f(sizeX, -sizeY, -sizeZ);
	glTexCoord2f(0, 1);glVertex3f(-sizeX, -sizeY, -sizeZ);
	glTexCoord2f(0, 0);glVertex3f(-sizeX, -sizeY, sizeZ);
	glTexCoord2f(1, 0);glVertex3f(sizeX, -sizeY, sizeZ);
	glEnd();

	//negative z plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);glVertex3f(-sizeX, sizeY, sizeZ);
	glTexCoord2f(0, 1);glVertex3f(sizeX, sizeY, sizeZ);
	glTexCoord2f(0, 0);glVertex3f(sizeX, -sizeY, sizeZ);
	glTexCoord2f(1, 0);glVertex3f(-sizeX, -sizeY, sizeZ);
	glEnd();

	//positive x plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);glVertex3f(sizeX, sizeY, sizeZ);
	glTexCoord2f(0, 1);glVertex3f(sizeX, sizeY, -sizeZ);
	glTexCoord2f(0, 0);glVertex3f(sizeX, -sizeY, -sizeZ);
	glTexCoord2f(1, 0);glVertex3f(sizeX, -sizeY, sizeZ);
	glEnd();

	//positive y plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);glVertex3f(sizeX, sizeY, sizeZ);
	glTexCoord2f(0, 1);glVertex3f(-sizeX, sizeY, sizeZ);
	glTexCoord2f(0, 0);glVertex3f(-sizeX, sizeY, -sizeZ);
	glTexCoord2f(1, 0);glVertex3f(sizeX, sizeY, -sizeZ);
	glEnd();

	//positive z plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);glVertex3f(sizeX, sizeY, -sizeZ);
	glTexCoord2f(0, 1);glVertex3f(-sizeX, sizeY, -sizeZ);
	glTexCoord2f(0, 0);glVertex3f(-sizeX, -sizeY, -sizeZ);
	glTexCoord2f(1, 0);glVertex3f(sizeX, -sizeY, -sizeZ);
	glEnd();

	glDisable(GL_TEXTURE_2D); //disable 2D texuring
}


void drawSingleTreeTexture(GLfloat size)
{
	glEnable(GL_TEXTURE_2D); //enable 2D texturing		
	glBindTexture(GL_TEXTURE_2D, treeTexture); //select the current texture


	glEnable(GL_ALPHA_TEST); //enable alpha testing
	glAlphaFunc(GL_GREATER, 0.1f); //select the alpha testing function
								   //GL_GREATER function passes if the incoming alpha value is greater than the reference value

	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(size, size, 0.0); //assign each corner of the texture to a 3D vertex in the OpenGL scene
		glTexCoord2f(0, 1);glVertex3f(-size, size, 0.0); //(0,0) is the left lower corner, while (1,1) is the right upper corner of the texture
		glTexCoord2f(0, 0);glVertex3f(-size, -size, 0.0);
		glTexCoord2f(1, 0);glVertex3f(size, -size, 0.0);
	glEnd();

	glDisable(GL_ALPHA_TEST); //disable alpha testing
	glDisable(GL_TEXTURE_2D); //disable 2D texturing
}

void drawWaterTexture() {
	fTextureAnimation += 0.05;

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
		glRotatef(fTextureAnimation, 1, 0, 0);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D); //enable 2D texturing		
	glBindTexture(GL_TEXTURE_2D, waterTexture); //select the current texture
	

	glEnable(GL_ALPHA_TEST); //enable alpha testing
	glAlphaFunc(GL_GREATER, 0.1f); //select the alpha testing function
								   //GL_GREATER function passes if the incoming alpha value is greater than the reference value
		
	
	drawModel(&waterModel, "objects/water.obj", GLM_SMOOTH | GLM_TEXTURE);

	glDisable(GL_ALPHA_TEST); //disable alpha testing
	glDisable(GL_TEXTURE_2D); //disable 2D texturing

	glMatrixMode(GL_TEXTURE);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

void drawFireTexture() {
	fTextureAnimation += 0.95;

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glRotatef(fTextureAnimation, 1, 1, 1);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D); //enable 2D texturing		
	glBindTexture(GL_TEXTURE_2D, fireTexture); //select the current texture


	glEnable(GL_ALPHA_TEST); //enable alpha testing
	glAlphaFunc(GL_GREATER, 0.1f); //select the alpha testing function
								   //GL_GREATER function passes if the incoming alpha value is greater than the reference value


	drawModel(&carModel, "objects/porsche.obj", GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);

	glDisable(GL_ALPHA_TEST); //disable alpha testing
	glDisable(GL_TEXTURE_2D); //disable 2D texturing

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}




void generateTextures() {
	//generate new texture IDs and load each texture that will be used
	glGenTextures(1, &treeTexture);
	loadTGA("Textures\\tree.tga", treeTexture);

	glGenTextures(1, &waterTexture);
	loadTGA("Textures\\river_flat_01.tga", waterTexture);

	glGenTextures(1, &fireTexture);
	loadTGA("Textures\\fx_fireball.tga", fireTexture);
	

	glGenTextures(6, skyboxTexture);
	loadTGA("Textures\\greenhill_negative_x.tga", skyboxTexture[0]);
	loadTGA("Textures\\greenhill_negative_y.tga", skyboxTexture[1]);
	loadTGA("Textures\\greenhill_negative_z.tga", skyboxTexture[2]);
	loadTGA("Textures\\greenhill_positive_x.tga", skyboxTexture[3]);
	loadTGA("Textures\\greenhill_positive_y.tga", skyboxTexture[4]);
	loadTGA("Textures\\greenhill_positive_z.tga", skyboxTexture[5]);

	//set skybox size
	fSkyboxSizeX = 100.0;
	fSkyboxSizeY = 10.0;
	fSkyboxSizeZ = 200.0;

	//set tree size
	fTreeSize = 1;
}

void initOpenGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);

	lightSources.push_back(GL_LIGHT0);
	lightSources.push_back(GL_LIGHT1);
	lightSources.push_back(GL_LIGHT2);
	lightSources.push_back(GL_LIGHT3);
	lightSources.push_back(GL_LIGHT4);
	lightSources.push_back(GL_LIGHT5);
	
	glEnable(GL_LIGHT6);

	glEnable(GL_LIGHTING);

	initLight();

	initStarPositionVector();

	glEnable(GL_NORMALIZE);

	generateTextures();

	ComputePlaneEquations();
	ComputeShadowMatrices();


}


void PlaneEq(GLfloat plane[4], GLfloat p0[4], GLfloat p1[4], GLfloat p2[4])
{
	GLfloat vec0[3], vec1[3];

	vec0[0] = p1[0] - p0[0];
	vec0[1] = p1[1] - p0[1];
	vec0[2] = p1[2] - p0[2];

	vec1[0] = p2[0] - p0[0];
	vec1[1] = p2[1] - p0[1];
	vec1[2] = p2[2] - p0[2];

	plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);
	plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];

	plane[3] = -(plane[0] * p0[0] + plane[1] * p0[1] + plane[2] * p0[2]);
}

void ComputeShadowMatrix(GLfloat shadowMat[16], GLfloat plane[4], GLfloat lightPos[4])
{
	GLfloat dotProduct;
	
	dotProduct = plane[0] * lightPos[0] +
		plane[1] * lightPos[1] +
		plane[2] * lightPos[2] +
		plane[3] * lightPos[3];

	shadowMat[0] = dotProduct - lightPos[0] * plane[0];
	shadowMat[1] = 0.0f - lightPos[1] * plane[0];
	shadowMat[2] = 0.0f - lightPos[2] * plane[0];
	shadowMat[3] = 0.0f - lightPos[3] * plane[0];

	shadowMat[4] = 0.0f - lightPos[0] * plane[1];
	shadowMat[5] = dotProduct - lightPos[1] * plane[1];
	shadowMat[6] = 0.0f - lightPos[2] * plane[1];
	shadowMat[7] = 0.0f - lightPos[3] * plane[1];

	shadowMat[8] = 0.0f - lightPos[0] * plane[2];
	shadowMat[9] = 0.0f - lightPos[1] * plane[2];
	shadowMat[10] = dotProduct - lightPos[2] * plane[2];
	shadowMat[11] = 0.0f - lightPos[3] * plane[2];

	shadowMat[12] = 0.0f - lightPos[0] * plane[3];
	shadowMat[13] = 0.0f - lightPos[1] * plane[3];
	shadowMat[14] = 0.0f - lightPos[2] * plane[3];
	shadowMat[15] = dotProduct - lightPos[3] * plane[3];
}


void ComputePlaneEquations()
{
	//floor points
	GLfloat fvFloorP0[4] = { fSkyboxSizeX / 2, -fSkyboxSizeY / 2, fSkyboxSizeZ / 2, 1.0 };
	GLfloat fvFloorP1[4] = { fSkyboxSizeX / 2, -fSkyboxSizeY / 2, -fSkyboxSizeZ / 2, 1.0 };
	GLfloat fvFloorP2[4] = { -fSkyboxSizeX / 2, -fSkyboxSizeY / 2, -fSkyboxSizeZ / 2, 1.0 };

	PlaneEq(fvFloorPlaneEq, fvFloorP0, fvFloorP1, fvFloorP2);
}

void ComputeShadowMatrices()
{


	if (fvLightPos[0] == -1) {
		fvLightPos[0] = lightX;
		fvLightPos[1] = lightY;
		fvLightPos[2] = lightZ;
	}
	else {
		Point pos = lightingPosition[1];
		fvLightPos[0] = pos.x - carFrontDirection;
		fvLightPos[1] = pos.y;
		fvLightPos[2] = pos.z - carSideDirection;

	}
	ComputeShadowMatrix(fvFloorShadowMat, fvFloorPlaneEq, fvLightPos);

}


void initLight() {

	for (int i = 0;i < NUMBER_LIGHTS;i++) {
		glEnable(lightSources[i]);
	}

	

}

void initStarPositionVector() {

	srand(1);

	int starX, starZ, boundary = 400;

	for (int i = 0; i < nrStars; i++) {

		starX = (rand() % boundary) - boundary / 2;
		starZ = (rand() % boundary) - boundary / 2;
		
		Point newPoint = { starX, 18, starZ };
		startPositionVector.push_back(newPoint);

	}

}


void drawModel(GLMmodel **pmodel, char*filename, GLuint mode){
	if (!*pmodel){
		*pmodel = glmReadOBJ(filename);
		if (!*pmodel)exit(0);
		glmUnitize(*pmodel);
		//generate facet normal vectors for model
		glmFacetNormals(*pmodel);
		//generate vertex normal vectors (called after generating facet normals)
		glmVertexNormals(*pmodel, 90.0);

		glmLinearTexture(*pmodel);
	}

	glmDraw(*pmodel, mode);
}

void EnableFog()
{
	glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.1f);
	glEnable(GL_FOG);
}


void constructCar(){

	glPushMatrix();
		glTranslatef(0, -0.7, 0); 
		drawModel(&carModel, "objects/porsche.obj", GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);

		if (carLife == 0) {
			glScalef(1.1, 1.1, 1.1);
			drawFireTexture();
		}

	glPopMatrix();
	
}

void constructTrees(){

	int numberOfTrees = 50;

	int distanceBetweenTrees = 5;

	int streedWideness = 7;

	int treeHeight = 5;
	int treeWidth = 2;
	for (int i = 1; i < numberOfTrees; i++){
	
			glPushMatrix();
				glTranslatef(streedWideness, 4, i*distanceBetweenTrees);
				glScalef(3, 5, 3 );
				drawSingleTreeTexture(fTreeSize);
				glRotatef(45, 0.0, 1.0, 0.0);
				drawSingleTreeTexture(fTreeSize);
				glRotatef(45, 0.0, 1.0, 0.0);
				drawSingleTreeTexture(fTreeSize);

			glPopMatrix();

	}

}


void constructForest() {

	int numberOfTrees = 50;
	int numberOfRows = 3;
	int distanceBetweenTrees = 5;
	int forestX = -30;
	int treeHeight = 5;
	int treeWidth = 2;
	
	for (int j = 1; j < numberOfRows; j++) {

		forestX -= j * 10;
	
		for (int i = 1; i < numberOfTrees; i++) {

			glPushMatrix();
			
				glTranslatef(forestX, 4, i*distanceBetweenTrees);
				glScalef(3, 5, 3);
				drawSingleTreeTexture(fTreeSize);
				glRotatef(45, 0.0, 1.0, 0.0);
				drawSingleTreeTexture(fTreeSize);
				glRotatef(45, 0.0, 1.0, 0.0);
				drawSingleTreeTexture(fTreeSize);

			glPopMatrix();

		}

	}

	

}

void constructStars() {

	if (starPosition < 400) {
		starPosition += 1;
	}
	else {
		starPosition = 0;
		isDay = !isDay;
	}


	GLfloat pos[] = { 0, 15, starPosition ,1 };

	glLightfv(GL_LIGHT6, GL_AMBIENT, g_light_ambient);
	glLightfv(GL_LIGHT6, GL_SPECULAR, g_light_specular);
	glLightfv(GL_LIGHT6, GL_POSITION, pos);

	//draw sun/moon
	glPushMatrix();
		glTranslatef(-10, 15, starPosition);
		glScalef(3, 5, 3);
		
		if (isDay) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sunMaterial);
		}
		
		glutSolidSphere(0.5, 20.0, 10);

	glPopMatrix();


	//draw stars
	if (!isDay) {

		int starX, starZ, boundary = 400;
		for (int i = 0; i < nrStars; i++) {

			Point position = startPositionVector[i];

			glPushMatrix();
				glTranslatef(position.x, position.y, position.z);
				glutSolidSphere(0.2, 20.0, 10);
			glPopMatrix();

		}

	}

}

void constructLightingPoles() {

	int numberOfPoles = 6;

	int distanceBetweenPoles = 50;

	int streedWideness = 4;

	int poleHeight = 5;
	int poleWidth = 2;

	Point lightPoint;

	for (int i = 0; i < numberOfPoles; i++) {


		glPushMatrix();
			glTranslatef(streedWideness, 4, i*distanceBetweenPoles);
			glScalef(3, 5, 3);
			drawModel(&poleModel, "objects/streetlamp.obj", GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
			lightPoint = { streedWideness, 10, i*distanceBetweenPoles };
			lightingPosition.push_back(lightPoint);
		glPopMatrix();
	}

}

void constructCastle(){

	glPushMatrix();
		glTranslatef(-49,14,115);
		glScalef(5, 10, 5);
	drawModel(&castleModel, "objects/glm-data/castle.obj", GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glPopMatrix();

}

void constructPlane(){

	glPushMatrix();

		glTranslatef(0, 0, 60); // just move it a little further
		glRotatef(planeSideAngle, 0, 1, 0); //for circular movement
		glTranslatef(15, 15, 05); // for circular movement radius
		glRotatef(30, 0, 0, 1); // tilt it a little bit
		glScalef(5, 5, 5);

		drawModel(&planeModel, "objects/glm-data/f-16.obj", GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);

	glPopMatrix();

}

void constructFences() {

	int numberOfFences = 50;

	int streetWideness = 3;

	int distanceBetweenFences = 5;

	for (int i = 0; i < numberOfFences; i++) {

		glPushMatrix();
			glTranslatef(streetWideness, -0.7, i*distanceBetweenFences);
			glScalef(1, 0.5, 5);
			glRotatef(90, 0, 1, 0); // tilt it a little bit
			drawModel(&fenceModel, "objects/oldFence.obj", GLM_SMOOTH | GLM_TEXTURE);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-streetWideness, -0.7, i*distanceBetweenFences);
			glScalef(1, 0.5, 5);
			glRotatef(270, 0, 1, 0); // tilt it a little bit
			drawModel(&fenceModel, "objects/oldFence.obj", GLM_SMOOTH | GLM_TEXTURE);
		glPopMatrix();
	}

}


void constructRoads() {
	
	////main road
	glPushMatrix();
		glTranslatef(0, -0.99, 100);
		glScalef(10, 0.1, 700);
		glutSolidCube(0.5);
	glPopMatrix();
}


void constructSkyBox() {

	glPushMatrix();
	
		glTranslatef(0, fSkyboxSizeY*0.9, fSkyboxSizeZ*0.95);

		drawSkybox(fSkyboxSizeX, fSkyboxSizeY, fSkyboxSizeZ);
	glPopMatrix();

}

void initializeLighting() {

	if(isDay) {
		diffuseLightIntensity = 0.7;
	}
	else {
		diffuseLightIntensity = 0.1;
	}
		

	g_light_diffuse[0] = diffuseLightIntensity;
	g_light_diffuse[1] = diffuseLightIntensity;
	g_light_diffuse[2] = diffuseLightIntensity;

	
	for (int i = 0; i < NUMBER_LIGHTS;i++) {
		
		Point position = lightingPosition[i];

		GLfloat pos[] = { position.x, position.y, position.z ,1 };
		glLightfv(lightSources[i], GL_AMBIENT, g_light_ambient);
		glLightfv(lightSources[i], GL_DIFFUSE, g_light_diffuse);

		glLightfv(lightSources[i], GL_SPECULAR, g_light_specular);
		glLightfv(lightSources[i], GL_POSITION, pos);
	}	

}

void constructFish() {

	fishAngle += 4;
	
	if (fishDepth > 300) {
		fishDepth = 0;
	}
	else {
		fishDepth += 0.1;
	}
	

	glPushMatrix();

		glTranslatef(-15, -1, fishDepth);
		glRotatef(90, 0, 1, 0);
		glRotatef(fishAngle, 0, 0, 1);
		glTranslatef(-2, 2, 5);

		
	drawModel(&fishModel, "objects/dolphins.obj", GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glPopMatrix();
}

void constructWater() {

	glPushMatrix();
		glTranslatef(-15, -1, 100);
		glScalef(10, 1, 1500);
		drawWaterTexture();
	glPopMatrix();

	constructFish();

}

void constructOnScreenWriting() {
	char carLifeStr[15];
	sprintf_s(carLifeStr, "Car life: %d", carLife);
	renderBitmapString(-1, 2, 0, carLifeStr);

	char wheelAngleStr[20];
	sprintf_s(wheelAngleStr, "Wheel Angle: %.f", wheelAngle);
	renderBitmapString(-1, 1.8, 0, wheelAngleStr);
}

void constructObjects(){


	glPushMatrix(); 

		glRotatef(-carSideAngle, 0, 1, 0);

		glTranslatef(-carFrontDirection, 0, -carSideDirection);

		constructStars();

		constructLightingPoles();

		initializeLighting();

		constructSkyBox();

		constructCastle();

		constructTrees();

		constructForest();
	
		constructWater();

		constructFences();

	
	glPopMatrix();

	constructOnScreenWriting();

	


}



void normalizeAngles(){

	carSideAngle = (int)carSideAngle % 360;

	if (carSideAngle < 0){
		carSideAngle = 360 - abs(carSideAngle);
	}

}



void detectCollision(double carFrontDirection) {


	if ((carFrontDirection > ROAD_LIMIT || carFrontDirection < -ROAD_LIMIT) && carLife > 0) {
		carLife--;
	}
	
}

void moveCar(int direction){
	ComputeShadowMatrices();
	
	//back barrier
	if (carSideDirection < -4 && direction == -1) {
		return;
	}

	//front barrier
	if (carSideDirection > 387 && direction == 1) {
		return;
	}

	carSideAngle += wheelAngle * direction; // increase the rotation angle of the car with the angle of the wheels


	carFrontDirection += sin(carSideAngle * 3.14 / 180) * direction; // translate the car using sin/cos
	carSideDirection += cos(carSideAngle * 3.14 / 180) * direction;

	//allow it to go off road
	if (carSideDirection > 250) {
		return;
	}

	detectCollision(carFrontDirection);

	carFrontDirection = fmin(carFrontDirection, ROAD_LIMIT);
	carFrontDirection = fmax(carFrontDirection, -ROAD_LIMIT);



}


void playMusic() {
	
	PlaySound((LPCSTR) "setFire.WAV", NULL, SND_FILENAME | SND_ASYNC);
	
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	switch (viewingMode) {
		case 1:
			gluLookAt(0, 0.5, 5, 0, 0.5, -10.0, 0.0, 1.0, 0.0);
			break;
		case 2:
			gluLookAt(0, -0.3, 3, 0, 0.5, -10.0, 0.0, 1.0, 0.0);
			break;
		case 3:
			gluLookAt(0, -0.3, -1, 0, 0.5, -10.0, 0.0, 1.0, 0.0);
			break;
	}
		
		
	glRotatef(sceneRotateX, 1, 0, 0);
	glRotatef(sceneRotateY, 0, 1, 0);
	glRotatef(sceneRotateZ, 0, 0, 1);
	
	glTranslatef( sceneTranslateX, 0, 0);
	glTranslatef( 0, sceneTranslateY, 0);
	glTranslatef( 0, 0, sceneTranslateZ);
	
	printf("%.f -- %.f \n", carSideDirection, carFrontDirection);

	//// see whole scene from behind
	glRotatef(180, 0, 1, 0);



	//enable stencil testing
	glEnable(GL_STENCIL_TEST);
	//set the stencil function to keep all data except when depth test fails
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//set stencil function to allways pass
	//this way the stencil buffer will be 1 in the area where the floor is drawn
	glStencilFunc(GL_ALWAYS, 1, 0);
	//clear the stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT);

	//draw floor
	glPushMatrix();
		glRotatef(-carSideAngle, 0, 1, 0);
		glTranslatef(-carFrontDirection, 0, -carSideDirection);
		constructRoads();
	glPopMatrix();

	//set the stencil function to pass only when the stencil buffer is 1
	//this way the shadow will appear only on the surface previously drawn
	glStencilFunc(GL_EQUAL, 1, 1);
	//disable depth testing for the shadow
	glDisable(GL_DEPTH_TEST);
	//disable lighting, in order to have a black shadow
	glDisable(GL_LIGHTING);

	//floor shadow	
	glColor3f(0.0f, 0.0f, 0.0f);//set the shadow color 
	glPushMatrix();
	//project the teapot onto the floor
	glMultMatrixf(fvFloorShadowMat);
	//apply the rotation of the teapot to the shadow
	//glRotatef(fRotAngleY, 0.0, 1.0, 0.0);
	
	
	//draw the projected car (the shadow)
	constructCar();

	glPopMatrix();


	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	//enable lighting
	glEnable(GL_LIGHTING);

	//set stencil function to allways pass
	//this way the stencil buffer will be 1 in the area where the left wall is drawn
	glStencilFunc(GL_ALWAYS, 1, 0);
	//clear the stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT);

	glEnable(GL_LIGHTING);

	if (fogEnabled) {
		EnableFog();
	}

	constructCar();

	constructObjects();


	glDisable(GL_FOG);

	glutSwapBuffers();
}


void renderBitmapString(float x, float y, float z, char* string) {


	glRasterPos3f(x, y, z);
	int size = strlen(string);
	for (int count = 0; count < size; count++) {
		
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[count]);
	}

}

void changeSize(int w, int h)
{
	screen_width = w;
	screen_height = h;

	if (h == 0)
		h = 1;

	float ratio = 1.0*w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 50.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
}

void processMouseClick(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		isDay = !isDay;
	}
}


void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'w'://drive forward

		moveCar(1);
		break;

	case 's'://drive backwards
		moveCar(-1);

		break;

	case 'a':
		wheelAngle = fmin(50, wheelAngle + 1);

		break;


	case 'd':
		wheelAngle = fmax(-50, wheelAngle - 1);
		break;

	case 'o':
		sceneRotateX += 2;
		break;

	case 'l':
		sceneRotateX -= 2;
		break;

	case 'i':
		sceneRotateY += 2;
		break;

	case 'k':
		sceneRotateY -= 6;

		break;
	case 'u':
		sceneRotateZ += 6;
		break;

	case 'j':
		sceneRotateZ -= 2;
		break;

	case 'y':
		sceneTranslateY -= 1;
		break;


	case 'h':
		sceneTranslateY += 1;
		break;


	case 't':
		sceneTranslateX += 1;
		break;


	case 'g':
		sceneTranslateX -= 1;
		break;

	case 'r':
		sceneTranslateZ += 1;
		break;


	case 'f':
		sceneTranslateZ -= 1;
		break;

	case 'v':
		lightX -= 1;
		ComputeShadowMatrices();
		break;

	case 'b':
		lightX += 1;
		ComputeShadowMatrices();
		break;

	case 'n':
		lightY -= 1;
		ComputeShadowMatrices();
		break;

	case 'm':
		lightY += 1;
		ComputeShadowMatrices();
		break;

	case 'z':
		lightZ -= 1;
		ComputeShadowMatrices();
		break;

	case 'x':
		lightZ += 1;
		ComputeShadowMatrices();
		break;


	case '1':
		viewingMode = 1;
		break;

	case '2':
		viewingMode = 2;
		break;

	case '3':
		viewingMode = 3;
		break;

	case '4':
		carLife = 50;
		break;

	case '5':
		isDay = !isDay;
		break;

	case '7':
		carFrontDirection = 0;
		carSideDirection = 0;
		carSideAngle = 0;
		wheelAngle = 0;
		wheelRotationAngle = 0;
		break;

	case '8':
		playMusic();
		break;

	case 'p':

		fogEnabled = !fogEnabled;
		break;

	case '6':
		wireframeOption = wireframeOption == 0 ? 1 : 0;
		if (wireframeOption)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;

	}

	

}

int main(int argc, char* argv[])
{
	//Initialize the GLUT library
	glutInit(&argc, argv);
	//Set the display mode
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//Set the initial position and dimensions of the window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen_width, screen_height);
	//creates the window
	glutCreateWindow("First OpenGL Application");
	//Specifies the function to call when the window needs to be redisplayed
	glutDisplayFunc(renderScene);
	//Sets the idle callback function
	glutIdleFunc(renderScene);
	//Sets the reshape callback function
	glutReshapeFunc(changeSize);
	//Keyboard callback function
	glutKeyboardFunc(processNormalKeys);
	glutMouseFunc(processMouseClick);
	//Initialize some OpenGL parameters
	initOpenGL();
	//Starts the GLUT infinite loop
	glutMainLoop();
	return 0;
}

