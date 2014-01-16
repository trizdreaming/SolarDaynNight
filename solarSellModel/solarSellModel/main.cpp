#include "include.h"

#define GL_PI 3.14159f

static bool flag = true;
static GLfloat zDistance = 0.0f;

void DrawLandArea();
void DrawSun();
void Drawpillar();
void texTureParametri();
void DrawSolarCell();


GLfloat amb [] = { 0.3f, 0.3f, 0.3f, 1.0f};
GLfloat dif [] = { 0.6f, 0.6f, 0.6f, 1.0f}; 
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPos[4]={0.f, 15.f, -200.f, 1.f}; 

AUX_RGBImageRec * texRec1;
AUX_RGBImageRec * texRec2;
GLuint texID1;
GLuint texID2;

GLUquadricObj * quadratic;


void SetupRC()
{
	glEnable(GL_DEPTH_TEST);  
	glEnable(GL_CULL_FACE);
	glEnable(GL_CCW);

	glEnable(GL_LIGHTING);	
	//light
	glLightfv(GL_LIGHT0,GL_AMBIENT, amb);   
	glLightfv(GL_LIGHT0,GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR, specular);
	

	//materials
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

	quadratic = gluNewQuadric();
	
	glGenTextures(1, &texID1);
	glGenTextures(1, &texID2);
	
	texRec1 = auxDIBImageLoad("land.bmp");
	glBindTexture(GL_TEXTURE_2D, texID1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texRec1->sizeX, texRec1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec1->data);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);

	texRec2 = auxDIBImageLoad("rawPanel_256.bmp");
	glBindTexture(GL_TEXTURE_2D, texID2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texRec2->sizeX, texRec2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec2->data);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
	glTranslatef(0.0f,0.0f, zDistance);
	glPushMatrix();
	//glLoadIdentity();
	//조명 설치
	if (flag) //낮 시간엔 빛이 있고
	{
		glEnable(GL_LIGHT0);
	}
	else  //밤 시간에 빛이 없고
	{
		glDisable(GL_LIGHT0);
	}
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	
	/*
	//조명 위치 확인용 코드
	glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
	glColor3ub(255,255,0);
	glutSolidSphere(2.f, 15, 15);
	*/
	glPopMatrix();
	//하늘 부분 그리기
	DrawSun();
	//땅 부분 그리기
	DrawLandArea();
	
	glPushMatrix();
	glTranslatef(0,0,-100);
	//기둥 그리기
	Drawpillar();
	//회복 있는 전지판 그리기
	DrawSolarCell();
	glPopMatrix();
	

	glutSwapBuffers();
}

void DrawSolarCell()
{
	static float fCellPosition = 90;
	

	texTureParametri();
	glBindTexture(GL_TEXTURE_2D, texID2);
	glEnable(GL_TEXTURE_2D);

	GLTVector3 vNormal;
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(0,0,-10);
	glRotatef(fCellPosition, 0.f, 1.f, 0.f);

	glPushMatrix();
	//glColor3ub(255, 255, 255);
	glRotatef(-40.f, 1.0f, 0.0f, 0.0f); 

	//glColor3ub(128, 128, 128);
	glBegin(GL_QUADS);
	{
		GLTVector3 vPoints[3] = {{-5.f, -5.f, 0.f},{5.f, -5.f, 0.f},{5.f, 5.f, 0.f}};
		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-5,-5);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(5,-5);

		glTexCoord2f(1.f, 1.f);
		glVertex2f(5,5);

		glTexCoord2f(0.f, 1.f);
		glVertex2f(-5,5);
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
	
	if ( fCellPosition >= 90 && !flag)
	{
		flag = true;
	}
	else if ( fCellPosition <= -90 && flag)
	{
		flag = false;
	}


	//이동 명령
	if (flag)
	{
		fCellPosition -= 5;
	}
	else
	{
		fCellPosition += 5;
	}

	glDisable(GL_TEXTURE_2D);
}

void Drawpillar()
{
	glPushMatrix();

	//glLoadIdentity();
	glTranslatef(0,0,-150);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(90.f, 0.f, 0.f, 1.f);

	glColor3ub(255,255,255); 
	glBegin(GL_POLYGON);
	gluCylinder(quadratic, 1.f, 1.f, 30.f, 40, 40);
	glEnd();

	glPopMatrix();
}

void DrawSun()
{
	static float fSunRot = 0.0f;
	//해 경로
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(0.0f,-50.0f, -250.0f);	
	glRotatef(fSunRot, 0.0f, 0.0f, 1.0f);
	glTranslatef(130.f,0.0f,0.0f);

	glColor3ub(255,70,0); 
	glutSolidSphere(5.0f, 30, 17); 
	fSunRot += 5.0f;
	if(fSunRot >= 360.0f)
		fSunRot = 0.0f;
	glPopMatrix();
}

void DrawLandArea()
{
	texTureParametri();
	glBindTexture(GL_TEXTURE_2D, texID1);
	glEnable(GL_TEXTURE_2D);
	
	GLTVector3 vNormal;

	glPushMatrix();
	//glLoadIdentity();
	glColor3ub(255, 255, 255);
	glTranslatef(0.f, -25.f, -250.f);
	glRotatef(-80.f, 1.0f, 0.0f, 0.0f); 

	//glColor3ub(128, 64, 64);
	

	glBegin(GL_QUADS);
	{
		GLTVector3 vPoints[3] = {{-70.f, -70.f, 0.f},{70.f, -70.f, 0.f},{70.f, 70.f, 0.f}};
		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-70,-70);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(70,-70);

		glTexCoord2f(1.f, 1.f);
		glVertex2f(70,70);

		glTexCoord2f(0.f, 1.f);
		glVertex2f(-70,70);
	}
	glEnd();
	glPopMatrix();
	

	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(0.f, -37.f, -158.f);	
	glBegin(GL_QUADS);
	{
		GLTVector3 vPoints[3] = {{-61.f, -5.f, 0.f},{61.f, -5.f, 0.f},{61.f, 5.f, 0.f}};
		gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
		glNormal3fv(vNormal);
		glTexCoord2f(0.0f, 1.f);
		glVertex2f(-61,-5);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(61,-5);
		glTexCoord2f(1.f, 0.8f);
		glVertex2f(61,5);
		glTexCoord2f(0.f, 0.8f);
		glVertex2f(-61,5);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void texTureParametri()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


void ChangeSize(GLint w, GLint h)
{
	GLfloat fAspect;
	glViewport(0,0,w,h);
	fAspect = (GLfloat)w/(GLfloat)h; 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,fAspect,1.0f,600.0f); 
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,0.f);
}

void ContorolKey(int key, int x, int y)
{
	if(key == GLUT_KEY_UP)
		zDistance += 0.5f;

	if(key == GLUT_KEY_DOWN)
		zDistance -= 0.5f;

	glutPostRedisplay();
}

void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunc,1);
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Solar Cell Modeling");

	SetupRC();

	glutSpecialFunc(ContorolKey);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33,TimerFunc,1);
	glutDisplayFunc(RenderScene);

	glutMainLoop();
}