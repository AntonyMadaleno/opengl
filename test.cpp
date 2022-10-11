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
#include <iostream>
#include <fstream>
#include <string>
#include "blob.h"

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void SpecialInput(int key, int x, int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void loadJpegImage(char *fichier);

unsigned char img[1024*1024*3];
GLuint tex0, tex1;
char presse;
int anglex,angley,x,y,xold,yold;
unsigned char image[1024*1024*3];

float theta, vsize, camX, camY, camZ;
int alpha, beta, adir, bdir;

//we declare our objects
Blob blob;
Sphere s0, s1, s2, s3, s4, s5, s6, s7, c;
float resize = 0.5;

void setup()
{

	//GLOBALS
	theta = 0;
	alpha = 30;
	adir = -2;
	beta = 0;
	bdir = 1;
	vsize = 40; //distance de vu
	camX = -20;
	camY = -20;
	camZ = -20;

	//TEST
	Point p0, p1, p2, p3, p4, p5, p6, p7, pc;
	p0 = Point(5,5,5);
	p1 = Point(5,5,-5);
	p2 = Point(5,-5,5);
	p3 = Point(5,-5,-5);
	p4 = Point(-5,5,5);
	p5 = Point(-5,5,-5);
	p6 = Point(-5,-5,5);
	p7 = Point(-5,-5,-5);
	pc = Point(0,0,0);

	s0 = Sphere(1.2, p0);
	s1 = Sphere(1.2, p1);
	s2 = Sphere(1.2, p2);
	s3 = Sphere(1.2, p3);
	s4 = Sphere(1.2, p4);
	s5 = Sphere(1.2, p5);
	s6 = Sphere(1.2, p6);
	s7 = Sphere(1.2, p7);
	c = Sphere(0.5, pc);
	c.setColor(Point(1,0,0));

	blob = Blob();
	blob.addMetaball(Metaball(p0, 1.2) );
	blob.addMetaball(Metaball(p1, 1.2) );
	blob.addMetaball(Metaball(p2, 1.2) );
	blob.addMetaball(Metaball(p3, 1.2) );
	blob.addMetaball(Metaball(p4, 1.2) );
	blob.addMetaball(Metaball(p5, 1.2) );
	blob.addMetaball(Metaball(p6, 1.2) );
	blob.addMetaball(Metaball(p7, 1.2) );
	blob.addMetaball(Metaball(pc, -0.5) );
	blob.setColor(Point(1,0,1));
	blob.setPrecision(32);
	blob.draw(Point(-8,-8,-8), Point(8,8,8));

}

void draw()
{

	//Drawings
	glBindTexture(GL_TEXTURE_2D, tex0);
	blob.GLDraw();
	glPushMatrix();
	//glScalef(0.75,0.75,0.75);
		s0.GLDraw();
		s1.GLDraw();
		s2.GLDraw();
		s3.GLDraw();
		s4.GLDraw();
		s5.GLDraw();
		s6.GLDraw();
		s7.GLDraw();
		c.GLDraw();
	glPopMatrix();
}

double cy = 0;
void anim(int dir)
{
	cy += (double) dir * 0.1;
	c.setCenter(Point(0,cy,0));
	blob.removeMetaball(8);
	blob.addMetaball(Metaball( Point(0,cy,0), -0.5 ) );
	blob.draw(Point(-8,-8,-8), Point(8,8,8));
}

int main(int argc,char **argv)
{

	setup();
	loadJpegImage("./res/earth.jpg");

	/* initialisation de glut et creation
		de la fenetre */
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1280,720);
	glutCreateWindow("thingy");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	/* Parametrage du placage de textures */
	glGenTextures(1, &tex0);
	glBindTexture(GL_TEXTURE_2D, tex0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
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
	glColor3f(1.0,1.0,1.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	//coord homogènes : position
	GLfloat position_source0[] = {10.0, 10.0, 0.0, 1.0};
	//direction source à distance infinie
	GLfloat dir0[] = {-1.0, -1.0, 0.0, 0.0};
	GLfloat dif_0[] = {1, 1, 1, 1.0};//composante diffuse
	GLfloat spec_0[] = {1, 1, 1, 1.0}; //composante spéculaire
	//spécification des propriétés
	glLightfv(GL_LIGHT0, GL_POSITION, position_source0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif_0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec_0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir0);
	//activation de la source GL_LIGHT0
	glEnable(GL_LIGHT0);

	//coord homogènes : position
	GLfloat position_source1[] = {-10.0, 10.0, 0.0, 1.0};
	//direction source à distance infinie
	GLfloat dir1[] = {1.0, -1.0, 0.0, 0.0};
	GLfloat dif_1[] = {1, 1, 1, 1.0};//composante diffuse
	GLfloat spec_1[] = {1, 1, 1, 1.0}; //composante spéculaire
	//spécification des propriétés
	glLightfv(GL_LIGHT1, GL_POSITION, position_source1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif_1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec_1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir1);
	//activation de la source GL_LIGHT0
	glEnable(GL_LIGHT1);

	//coord homogènes : position
	GLfloat position_source2[] = {0.0, 10.0, 10.0, 1.0};
	//direction source à distance infinie
	GLfloat dir2[] = {0.0, -1.0, -1.0, 0.0};
	GLfloat dif_2[] = {1, 1, 1, 1.0};//composante diffuse
	GLfloat spec_2[] = {1, 1, 1, 1.0}; //composante spéculaire
	//spécification des propriétés
	glLightfv(GL_LIGHT2, GL_POSITION, position_source2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, dif_2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spec_2);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir2);
	//activation de la source GL_LIGHT0
	glEnable(GL_LIGHT2);


	glLoadIdentity();
	glScalef(1.0,16.0/9.0,1.0);
	glRotatef(angley,1.0,0.0,0.0);
	glRotatef(anglex,0.0,1.0,0.0);

	glPushMatrix();
		draw();
	glPopMatrix();

    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-100, 0,0.0);
    glVertex3f(100, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -100,0.0);
    glVertex3f(0, 100,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0,-100);
    glVertex3f(0, 0,100);
    glEnd();

  	glFlush();
  
  	//On echange les buffers 
  	glutSwapBuffers();
}

void clavier(unsigned char touche,int x,int y)
{
  	switch (touche)
    {
		case 'z':
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
		anim(1);
	break;

	case GLUT_KEY_DOWN:
		anim(-1);
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
	glViewport(0,0,x,y);
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

    if ((cinfo.image_width!=1024)||(cinfo.image_height!=1024)) {
        fprintf(stdout,"Erreur : l'image doit etre de taille 1024x1024\n");
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
        ligne=img+3*1024*cinfo.output_scanline;
        jpeg_read_scanlines(&cinfo,&ligne,1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}
