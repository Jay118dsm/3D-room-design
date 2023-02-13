#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include <windows.h>
#include "vector"

using namespace std;

#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texture[5];
static GLfloat xrot = 0;
static GLfloat yrot = 0;

float r = 0;

GLint imagewidth0, imagewidth1, imagewidth2, imagewidth3;
GLint imageheight0, imageheight1,imageheight2, imageheight3;
GLint pixellength0, pixellength1, pixellength2, pixellength3;
vector <GLubyte*>p;

float fltFOV = 70; //Field Of View
float fltZoom = 1; //Zoom amount
float fltX0 = 4; //Camera position
float fltY0 = 2;
float fltZ0 = 4;
float fltXRef = 0.0; //Look At reference point
float fltYRef = 0.2;
float fltZRef = 0.0;
float fltXUp = 0.0; //Up vector
float fltYUp = 1.0;
float fltZUp = 0.0;
GLfloat step = 1;
GLfloat xb = 0; 
GLfloat yb = 0;
GLfloat zb = 0;
GLfloat sb1 = 1;
GLfloat sb2 = 1;
GLfloat sb3 = 1;
GLfloat sh = 0;
GLfloat rt = 0;
int time_interval = 16; // declare refresh interval in ms

void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}

void OnTimer(int value) {
	r += step;
	if (r >= 360)
		r = 0;
	else if (r <= 0)
		r = 359;
	glutTimerFunc(time_interval, OnTimer, 1);
}



void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
	GLubyte* pixeldata;
	FILE* pfile;
	fopen_s(&pfile, path, "rb");
	if (pfile == 0) exit(0);
	fseek(pfile, 0x0012, SEEK_SET);
	fread(&imagewidth, sizeof(imagewidth), 1, pfile);
	fread(&imageheight, sizeof(imageheight), 1, pfile);
	pixellength = imagewidth * 3;
	while (pixellength % 4 != 0)pixellength++;
	pixellength *= imageheight;
	pixeldata = (GLubyte*)malloc(pixellength);
	if (pixeldata == 0) exit(0);
	fseek(pfile, 54, SEEK_SET);
	fread(pixeldata, pixellength, 1, pfile);
	p.push_back(pixeldata); 
	fclose(pfile);
}

void SetupRC()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);


	GLfloat mat_ambient[] = { 0.6f,0.6f,0.6f,1.0f };
	GLfloat mat_diffuse[] = { 0.5f,0.5f,0.5f,1.0f };
	GLfloat mat_specular[] = { 1.0f,1.0f,1.0f,1.0f, };
	GLfloat mat_shininess[] = { 25.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//纹理

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	ReadImage("1.bmp", imagewidth0, imageheight0, pixellength0);
	ReadImage("2.bmp", imagewidth1, imageheight1, pixellength1);
	ReadImage("3.bmp", imagewidth2, imageheight2, pixellength2);
	ReadImage("4.bmp", imagewidth3, imageheight3, pixellength3);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set pixel storage modes (in the memory)
	glGenTextures(3, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]); // target to which texture is bound and name of a texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth0, imageheight0, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth1, imageheight1, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth2, imageheight2, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth3, imageheight3, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[3]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


	
}

void DrawWall()
{
	glPushMatrix();
	glTranslatef(1.2, 0.01, 1.2);
	glScaled(2.4, 0.02, 2.4);
	glutSolidCube(1.0);
	glPopMatrix();
}


void DrawPillar()
{
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0.15, 0);
	glScaled(0.02, 0.3, 0.02);
	glColor3f(178.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}

void DrawTree() {

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.1);
	glPushMatrix();
	glTranslatef(0.3, 0.6, 1.8);
	glScalef(0.25, 0.252, 0.25);
	glColor3f(0.0, 0.7, 0.1);
	glutSolidIcosahedron();
	glColor3f(0.0, 0.0, 0.0);
	glutWireIcosahedron();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.3, 0.35, 1.8);
	glScalef(0.1, 0.7, 0.11);
	glColor3f(0.7, 0.3, 0.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
}

//绘制桌子
void DrawTable()
{

	glPushMatrix();
	glTranslatef(0.05, 0.3, 0.05);
	glScaled(0.6, 0.02, 0.6);
	glColor3f(210.0/255.0, 105.0/255.0, 30.0/255.0);
	glutSolidCube(1.0); //绘制桌面
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.275, 0, 0.275);
	DrawPillar();			//绘制桌腿

	glTranslatef(0, 0, -0.45);
	DrawPillar();			//绘制桌腿

	glTranslatef(-0.45, 0, 0.45);
	DrawPillar();			//绘制桌腿
	glTranslatef(0, 0, -0.45);
	DrawPillar();			//绘制桌腿
	glPopMatrix();
}


void DrawSofa() {
	glPushMatrix();
	glScaled(0.75,0.75,0.75);
	glTranslatef(0, 0, 0.2);
	glPushMatrix();
	glTranslatef(0.08, 0, 0.15);
	glPushMatrix();
	glTranslatef(0.25,0.15,1);
	glScaled(2.4, 1.5, 2.2);
	glColor3f(244.0 / 255.0, 164.0 / 255.0, 96.0 / 255.0);
	glutSolidCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.15, 0.552);
	glScaled(2.4, 1.5, 2.2);
	glColor3f(244.0 / 255.0, 164.0 / 255.0, 96.0 / 255.0);
	glutSolidCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.26, 1.25);
	glScaled(2.4, 2.5, 0.6);
	glColor3f(244.0 / 255.0, 164.0 / 255.0, 96.0 / 255.0);
	glutSolidCube(0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.26, 0.29);
	glScaled(2.4, 2.5, 0.6);
	glColor3f(244.0 / 255.0, 164.0 / 255.0, 96.0 / 255.0);
	glutSolidCube(0.2);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.05, 0.34+sh, 0.9);
	glScaled(0.5, 3.8, 4.3);
	glColor3f(244.0 / 255.0, 164.0 / 255.0, 96.0 / 255.0);
	glutSolidCube(0.2);
	glPopMatrix();
	glPopMatrix();
}

void DrawRoller() {
	glPushMatrix();
	glTranslatef(1.2, 0.9, 0.05);
	glScaled(0.02, 0.01, 0.13);
	glColor3f(0.0, 0.7, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();
}

void DrawBlade() {
	glEnable(GL_TEXTURE_2D);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texture[2]); 
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4, 0, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4, 0.2, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0, 0.0);
	glEnd();
}

void DrawFan() {
	glPushMatrix();
	DrawRoller();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, 0.9, 0.12);
	glPushMatrix();
	glRotatef(r, 0, 0, 1);

	glPushMatrix();
	DrawBlade();
	glPopMatrix();

	glPushMatrix();
	glRotatef(120, 0, 0, 1);

	DrawBlade();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(240, 0, 0, 1);

	DrawBlade();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}


void DrawWindow() {
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.01, 0.6, 0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.01, 1.6, 0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.01, 1.6, 1.5);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.01, 0.6, 1.5);
	glEnd();
}




//绘图
void RenderScene()
{
	GLfloat light_position[] = { 2.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);  //0号光源的位置

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat h = 1.0;
	glOrtho(-h * 64 / 48.0, h * 64 / 48.0, -h, h, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(fltX0 * fltZoom, fltY0 * fltZoom, fltZ0 * fltZoom, fltXRef, fltYRef, fltZRef, fltXUp, fltYUp, fltZUp);

	//开始绘制
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotated(90.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	DrawWall();				//绘制一面墙壁
	glPopMatrix();

	glPushMatrix();
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	DrawWall();		//绘制另一面墙壁
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.8, 0, 0.35);
	glScaled(1.2, 1.2, 1.2);

	glPushMatrix();
	glTranslatef(0.4, 0, 0.4);
	DrawTable();			//绘制桌子
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.45, 0.1, 0.85);
	glRotatef(180.0, 180.0, 1, 180.0);
	glScaled(0.8, 0.8, 1.0);
	glColor3f(255.0 / 255.0, 192.0 / 255.0, 203.0 / 255.0);
	glutSolidCube(0.2);		//绘制箱子
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.85, 0.1, 0.45);
	glScaled(0.8, 0.8, 1.0);
	glColor3f(138.0 / 255.0, 43.0 / 255.0, 226.0 / 255.0);
	glutSolidCube(0.2);		//绘制箱子
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, 0.38, 0.6);
	glRotated(-100+rt, 0, 1, 0);
	glColor3f(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);
	glutSolidTeapot(0.1);	//绘制茶壶
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0.33, 0.5);
	glRotatef(180, 0, 180, 180);
	glutSolidTorus(0.02f, 0.05, 25, 50);//绘制烟灰缸
	glPopMatrix();

	glPushMatrix();
	glScaled(sb1, sb2, sb3);
	glTranslatef(0.45+xb, 0.42+yb, 0.3+zb);	
	glColor3f(25.0 / 255.0, 2.0 / 255.0, 203.0 / 255.0);
	glutSolidSphere(0.1, 15, 50);		//绘制球体
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, 0.0, -0.1);

	glPushMatrix();
	glTranslatef(1.0, 0.35, 0.3);
	glScaled(0.03, 0.7, 0.03);
	glColor3f(128.0 / 255.0, 42.0 / 255.0, 42.0 / 255.0);
	glutSolidCube(1.0);					//绘制灯柱
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0, 0.67, 0.3);
	glRotatef(190, 160, 200, 180);
	glColor3f(138.0 / 255.0, 54.0 / 255.0, 15.0 / 255.0);
	glutSolidCone(0.15, 0.25f, 30, 25);	//绘制圆锥型灯罩
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	DrawTree();
	glPopMatrix();

	glPushMatrix();
	DrawSofa();
	glPopMatrix();

	glPushMatrix();
	DrawFan();
	glPopMatrix();

	glPushMatrix();
	DrawWindow();
	glPopMatrix();


	//绘制纹理
	glEnable(GL_TEXTURE_2D);		//开启纹理
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//映射方式
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);				//绘制地面
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 2.4);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.4, 0.0, 2.4);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.4, 0.0, 0.0);
	glEnd();
	

	//开启纹理
	//映射方式
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);				
	glTexCoord2f(0.0, 0.0); glVertex3f(0.005, 0.0, 2.4);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.005, 2.4, 2.4 );
	glTexCoord2f(1.0, 1.0); glVertex3f(0.005, 2.4, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.005, 0.0, 0.0 );
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);				
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.005);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 2.4, 0.005);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.4, 2.4, 0.005);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.4, 0.0, 0.005);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glFlush();


}


void SpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:		//上键
		xrot += 1.0;
		break;
	case GLUT_KEY_DOWN:		//下键
		xrot -= 1.0;
		break;
	case GLUT_KEY_LEFT:		//左键
		yrot += 1.0;
		break;
	case GLUT_KEY_RIGHT:	//右键
		yrot -= 1.0;
		break;
	default:
		break;
	}
	glutPostRedisplay();
	glFlush();
}

void adjustDisplay(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q')
		exit(0);

	if (key == 'd' || key == 'D')
			fltY0 -= 0.1; //Camera down
	if (key == 'u' || key == 'U')
			fltY0 += 0.1; //Camera up
	if (key == 'i' || key == 'I')
			fltZoom -= 0.1; //Zoom in
	if (key == 'o' || key == 'O')
			fltZoom += 0.1; //Zoom out

	if (key == 'j' || key == 'J')
		xb += 0.01; 
	if (key == 'k' || key == 'K')
		xb -= 0.01; 
	if (key == 'n' || key == 'N')
		yb += 0.01;
	if (key == 'm' || key == 'M')
		yb -= 0.01;
	if (key == 'g' || key == 'G')
		zb += 0.01;
	if (key == 'h' || key == 'H')
		zb -= 0.01;
	if (key == 'y' || key == 'Y')
		zb -= 0.01;
	if (key == 'w' || key == 'W')
		sh += 0.01;
	if (key == 's' || key == 'S')
		sh -= 0.01;
	if (key == 'z' || key == 'Z')
		rt +=2;


	if (key == 't' || key == 'T') {
		sb1 += 0.015;
		sb2 += 0.015;
		sb3 += 0.015;
	}

	if (key == 'r' || key == 'R') {
		sb1 -= 0.01;
		sb2 -= 0.01;
		sb3 -= 0.01;
	}






	glutPostRedisplay();
}

void mouse_input(int button, int state, int x, int y) { // mouse interaction
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && step >= -15)
		step -= 1; // decrement step
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON && step <= 15)
		step += 1; // increment step
}

//改变窗口大小
void ChangeSize(int w, int h)
{
	GLfloat lightPos[] = { -50.f, 50.0f, 100.0f, 1.0f };
	GLfloat nRange = 1.9f;

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange * h / w, -nRange, nRange);
	else
		glOrtho(-nRange * w / h, nRange * w / h, -nRange, nRange, -nRange, nRange);

	/*glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 400);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("Room");

	SetupRC();
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	glutIdleFunc(when_in_mainloop);
	glutTimerFunc(time_interval, OnTimer, 1);

	glutKeyboardFunc(adjustDisplay);
	glutMouseFunc(mouse_input);
	glutSpecialFunc(SpecialKeys);
	glutMainLoop();
}



