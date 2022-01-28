/* inclusion des fichiers d'en-tete freeglut */
#include <stdio.h>
#include <cstdlib>
#include "iostream"
#include <cmath>
#include <cstdio>

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif 
#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif

#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>

#include "sphere.h"
#include "cylinder.h"
#include "tore.h"
#include "Revolution.h"
#include "texture.h"
#include "bline.h"
#include "bsurface.h"
#ifndef BEZIER4_H
#include "bezier4.h"
#endif

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void SpecialInput(int key, int x, int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void loadJpegImage(char *fichier);

unsigned char img[256*256*3];
GLuint tex0, tex1;
char presse;
int anglex,angley,x,y,xold,yold;
unsigned char image[256*256*3];

float theta, vsize, camX, camY, camZ;
int alpha, beta, adir, bdir;

//we declare our objects
BLine main0_body, main1_body, tail_body, neck_body, seg0_tail, seg1_tail;
BSurface wing0;
Revolution skull0, skull1, horn0;
Sphere skull_sph0;
float resize = 0.5;

void idleAnim()
{

	theta = theta + M_PI/90;
	glutPostRedisplay();

	beta = (beta + bdir);

	if (beta >= 180)
	{
		bdir = -1;
	}
	if (beta <= 0)
	{
		bdir = 1;
	}

}

void wingAnim()
{

	alpha = (alpha + adir);

	if (alpha >= 30)
	{
		adir = -2;
	}
	if (alpha <= -10)
	{
		adir = 2;
	}

}


void setup()
{

	//GLOBAL SETTINGS

	theta = 0;
	alpha = 30;
	adir = -2;
	beta = 0;
	bdir = 1;
	vsize = 20; //distance de vu
	camX = -10;
	camY = -5;
	camZ = -10;

	//BODY SETUP
	Point a0 = Point(0,2,0);		Point b0 = Point(3,2,0);		Point c0 = Point(3,-3,0);	Point d0 = Point(6,0,0);
	Bezier4 bez0 = Bezier4(a0,b0,c0,d0);

	main0_body = BLine(3);
	main0_body.setEndingRadius(2);
	main0_body.setLine(bez0.Spline());
	main0_body.setTexture("./res/tex0.jpg");

	Point a1 = a0;	Point b1 = Point(-3,2,0);	Point c1 = Point(-3,3,0);	Point d1 = Point(-5,3,0);
	Bezier4 bez1 = Bezier4(a1,b1,c1,d1);

	main1_body = BLine(3);
	main1_body.setEndingRadius(2);
	main1_body.setLine(bez1.Spline());
	main1_body.setTexture("./res/tex0.jpg");

	Point a2 = Point(6,0,0);	Point b2 = Point(8,2,0);	Point c2 = Point(8,3,0);	Point d2 = Point(10,3,0);
	Bezier4 bez2 = Bezier4(a2,b2,c2,d2);

	tail_body = BLine(2);
	tail_body.setEndingRadius(1);
	tail_body.setLine(bez2.Spline());
	tail_body.setTexture("./res/tex0.jpg");

	neck_body = BLine(2);
	neck_body.setEndingRadius(1);
	neck_body.setTexture("./res/tex0.jpg");	

	//TAIL SETUP - part is moving given a cosine/sine patern so we dont declare any curve/points here !
	seg0_tail = BLine(1);
	seg0_tail.setEndingRadius(0.5);
	seg0_tail.setTexture("./res/tex0.jpg");

	seg1_tail = BLine(0.5);
	seg1_tail.setEndingRadius(0);
	seg1_tail.setTexture("./res/tex0.jpg");

	//HEAD SETUP 

	std::vector<Point> sk0_pts;

	Bezier4 s0Bez = Bezier4(Point(1,0,0), Point(1,0.5,0), Point(2,1,0), Point(2,2,0));
	skull0 = Revolution(s0Bez.Spline());
	skull0.setTexture("./res/tex0.jpg");

	Bezier4 s1Bez = Bezier4(Point(2,2,0), Point(2,3,0), Point(0.5,2.5,0), Point(0.5,4,0));
	skull1 = Revolution(s1Bez.Spline());
	skull1.setTexture("./res/tex0.jpg");

	Bezier4 s2Bez = Bezier4(Point(0.25,0,0), Point(0.25,0.5,0), Point(0,1,0), Point(0,1.5,0));
	horn0 = Revolution(s2Bez.Spline());
	horn0.setTexture("./res/tex0.jpg");

	skull_sph0 = Sphere(0.5);
	skull_sph0.setCenter(Point(0,0,4));
	skull_sph0.setTexture("./res/tex0.jpg");

	
}

void DrawBody()
{
	//body will remain static no movement
	main0_body.GLDraw();
	main1_body.GLDraw();
	tail_body.GLDraw();

	//NECK
	std::vector<Point> npts;

	Point a3 = Point(-5,3,0);	Point b3 = Point(-6,3,0);	Point c3 = Point(-7,1*cos(theta) + 4,0);	
	Point d3 = Point(-8,1*cos(theta) + 4,0);
	Bezier4 bez3 = Bezier4(a3,b3,c3,d3);

	unsigned it = 0;
	for (Point p : bez3.Spline())
	{
		p.z += cos( (M_PI*beta/180) ) * (it)/bez3.getDefinition();
		npts.push_back(p);
		it++;
	}

	neck_body.setLine(npts);
	neck_body.GLDraw();

}

void DrawTail()
{

	//this need to be recalcultated do to animation
	Point a4 = Point(0,0,0);	Point b4 = Point(2,0,0);	
	Point c4 = Point(3,cos(theta),0);	Point d4 = Point(4+sin(theta),cos(theta),0);
	Bezier4 bez4 = Bezier4(a4,b4,c4,d4);

	seg0_tail.setLine(bez4.Spline());

	Point a5 = Point(4+sin(theta),cos(theta),0);	Point b5 = Point(5.5+sin(theta),cos(theta),0);
	Point c5 = Point(7,cos(2*theta),0);	Point d5 = Point(8,0,0);
	Bezier4 bez5 = Bezier4(a5,b5,c5,d5);

	seg1_tail.setLine(bez5.Spline());

	//tail
	glPushMatrix();

	glTranslatef(10,3,0);

	seg0_tail.GLDraw();
	seg1_tail.GLDraw();

	glPopMatrix();

}

void DrawWing()
{
	//WING SETUP
	std::vector<std::vector<Point>> BSP0;

	std::vector<Point> vec0, vec1, vec2, vec3;

	vec0.push_back(Point(0,0,0)); vec0.push_back(Point(2,4*cos(theta),0)); 
	vec0.push_back(Point(4,-4*cos(theta),0)); vec0.push_back(Point(6,0,0));

	vec1.push_back(Point(0,0,2)); vec1.push_back(Point(2,2*cos(theta),2)); 
	vec1.push_back(Point(4,-2*cos(theta),2)); vec1.push_back(Point(6,0,2));

	vec2.push_back(Point(0,0,4)); vec2.push_back(Point(2,4*cos(theta),4)); 
	vec2.push_back(Point(4,-4*cos(theta),4)); vec2.push_back(Point(6,0,4));

	vec3.push_back(Point(0,0,6)); vec3.push_back(Point(2,4*cos(theta),6)); 
	vec3.push_back(Point(4,-4*cos(theta),6)); vec3.push_back(Point(6,0,6));

	BSP0.push_back(vec0); BSP0.push_back(vec1); BSP0.push_back(vec2); BSP0.push_back(vec3);

	wing0 = BSurface(BSP0);
	wing0.setTexture("./res/tex0.jpg");
	wing0.GLDraw();
}

void DrawHead()
{
	glPushMatrix();
	glTranslatef(-8,1*cos(theta) + 4, cos( (M_PI*beta/180) ));
	glRotatef(90,0,0,1);
	skull0.GLDraw();
	skull1.GLDraw();
	skull_sph0.GLDraw();
	glPopMatrix();

	for (int i = 0; i < 8; i++)
	{

		glPushMatrix();		
		glTranslatef(-10,2*cos(2*M_PI*i/8) + 4, 2*sin(2*M_PI*i/8));
		glTranslatef(0,1*cos(theta),cos((M_PI*beta/180)));
		glRotatef(360*i/8,1,0,0);
		
		horn0.GLDraw();

		glPopMatrix();

	}

}

int main(int argc,char **argv)
{

	/* Chargement de la texture */
  	loadJpegImage("./res/tex0.jpg");

	setup();

	/* initialisation de glut et creation
		de la fenetre */
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(500,500);
	glutCreateWindow("thingy");

	/* Initialisation d'OpenGL */
	glClearColor(0.0,0.0,0.0,0.0);
	glColor3f(1.0,1.0,1.0);
	glShadeModel(GL_FLAT);
	glPointSize(2.0);
	glEnable(GL_DEPTH_TEST);

	/* enregistrement des fonctions de rappel */
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutSpecialFunc(SpecialInput);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);
	glutIdleFunc(idleAnim);

	/* Parametrage du placage de textures */
	glGenTextures(1, &tex0);
	glBindTexture(GL_TEXTURE_2D, tex0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	loadJpegImage("./res/tex1.jpg");

	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Entree dans la boucle principale glut */
	glutMainLoop();
	return 0;
}


void affichage()
{

  	int i,j;
	int spc[4] = {1,1,1,1};

	glMaterialiv(GL_FRONT_AND_BACK,GL_SPECULAR, spc);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS, 100);
  	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	float x = camX; 
	float y = camY;
	float z = camZ;  
    glOrtho(x, x + vsize, y, y + vsize, z, z + vsize);
    glMatrixMode(GL_MODELVIEW);

	/* effacement de l'image avec la couleur de fond */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);


	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glLoadIdentity();
	glRotatef(angley,1.0,0.0,0.0);
	glRotatef(anglex,0.0,1.0,0.0);

	//coord homogènes : position
	GLfloat position_source0[] = {10.0, 10.0, 10.0, 1.0};
	//direction source à distance infinie
	GLfloat dir0[] = {-1.0, -1.0, -1.0, 0.0};
	GLfloat dif_0[] = {float(beta)/180, 0, 0, 1.0};//composante diffuse
	GLfloat spec_0[] = {float(beta)/180, 0, 0, 1.0}; //composante spéculaire
	//spécification des propriétés
	glLightfv(GL_LIGHT0, GL_POSITION, position_source0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif_0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec_0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir0);
	//activation de la source GL_LIGHT0
	glEnable(GL_LIGHT0);

	//coord homogènes : position
	GLfloat position_source1[] = {0.0, 0.0, 0.0, 1.0};
	//direction source à distance infinie
	GLfloat dif_1[] = {0.5, 0.5, 0.5, 1.0};//composante diffuse
	GLfloat spec_1[] = {0.5, 0.5, 0.5, 1.0}; //composante spéculaire
	GLfloat amb[] = {0.5, 0.5, 0.5, 1.0}; //composante spéculaire
	//spécification des propriétés
	glLightfv(GL_LIGHT1, GL_POSITION, position_source1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif_1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec_1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	//activation de la source GL_LIGHT0
	glEnable(GL_LIGHT1);

	//Drawings
	glBindTexture(GL_TEXTURE_2D, tex0);
	glPushMatrix();

	glScalef(resize,resize,resize);

	DrawBody();
	DrawTail();
	DrawHead();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, tex1);
	//WING
	glPushMatrix();
	glTranslatef(0,resize*2,resize*(3));
	glRotatef(alpha,0,0,1);
	glRotatef(30,-1,0,0);
	glRotatef(alpha + 15,0,1,0);
	glScalef(resize*1.5,resize*1.5,resize*2);
	DrawWing();
	glPopMatrix();

	//WING
	glPushMatrix();
	glTranslatef(0,resize*2,resize*(-3));
	glRotatef(alpha,0,0,1);
	glRotatef(30,1,0,0);
	glRotatef(alpha + 15,0,-1,0);
	glScalef(resize*1.5,resize*1.5,-resize*2);
	DrawWing();
	glPopMatrix();

/*
    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0,0.0);
    glVertex3f(5, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, 0,0.0);
    glVertex3f(0, 5,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0,0.0);
    glVertex3f(0, 0,5.0);
    glEnd();

*/

  	glFlush();
  
  	//On echange les buffers 
  	glutSwapBuffers();
}

void clavier(unsigned char touche,int x,int y)
{
  	switch (touche)
    {
		case '1': /*anim des ailes*/
			wingAnim();
		break;
		case 'z': /*anim des ailes*/
			resize = resize - 0.010;
			if (resize <= 0)
			{
				resize = 0;
			}
			glutPostRedisplay();
		break;
		case 'Z': /*anim des ailes*/
			resize = resize + 0.010;
			glutPostRedisplay();
		break;
    	case 'p': /* affichage du carre plein */
      		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      		glutPostRedisplay();
      	break;
    	case 'f': /* affichage en mode fil de fer */
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glutPostRedisplay();
      		break;
    	case 's' : /* Affichage en mode sommets seuls */
      		glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      		glutPostRedisplay();
      	break;
    	case 'd':
      		glEnable(GL_DEPTH_TEST);
      		glutPostRedisplay();
      		break;
    	case 'D':
      		glDisable(GL_DEPTH_TEST);
      		glutPostRedisplay();
      	break;
    	case 'q' : /*la touche 'q' permet de quitter le programme */
      		exit(0);
    }
}

void SpecialInput(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		angley = angley + 1;
	break;
	case GLUT_KEY_DOWN:
		angley = angley - 1;
	break;
	case GLUT_KEY_LEFT:
		anglex = anglex + 1;
	break;
	case GLUT_KEY_RIGHT:
		anglex = anglex - 1;
	break;
	}
	glutPostRedisplay();
}

void reshape(int x,int y)
{
  	if (x<y)
    	glViewport(0,(y-x)*2,x,x);
  	else 
    	glViewport((x-y)*2,0,y,y);
}

void mouse(int button, int state,int x,int y)
{
  	/* si on appuie sur le bouton gauche */
  	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
  	{
    	presse = 1; /* le booleen presse passe a 1 (vrai) */
    	xold = x; /* on sauvegarde la position de la souris */
    	yold=y;
  	}
  	/* si on relache le bouton gauche */
  	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
    	presse=0; /* le booleen presse passe a 0 (faux) */
	}

void mousemotion(int x,int y)
{
	if (presse) /* si le bouton gauche est presse */
    {
      	/* on modifie les angles de rotation de l'objet
	 	en fonction de la position actuelle de la souris et de la derniere
	 	position sauvegardee */
      	anglex=anglex+(x-xold); 
      	angley=angley+(y-yold);
      	glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

	//printf("x %i , y %i\n", x,y);
    
    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
}

void loadJpegImage(char* fichier)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *file;	
    unsigned char *ligne;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

#ifdef __WIN32
    errno_t err = fopen_s(&file , fichier, "rb");
    if (err != 0)
    {
        std::cout << strerror(err) << std::endl;
        printf("Erreur : impossible d'ouvrir le fichier %s", fichier);
        exit(1);
    }
#elif __GNUC__
    if ((file = fopen(fichier,"rb")) == 0)
    {
        fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
        exit(1);
    }	
#endif

    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);

    if ((cinfo.image_width!=256)||(cinfo.image_height!=256)) {
        fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
        exit(1);
    }
    if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
        fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
        exit(1);
    }

    jpeg_start_decompress(&cinfo);
    ligne=img;
    while (cinfo.output_scanline<cinfo.output_height)
    {
        ligne=img+3*256*cinfo.output_scanline;
        jpeg_read_scanlines(&cinfo,&ligne,1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}
