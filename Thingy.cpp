/* inclusion des fichiers d'en-tete freeglut */
#include <stdio.h>
#include <cstdlib>
#include "iostream"
#include <cmath>
#include <cstdio>
#include <ctime>

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
int alpha, beta;
float resize = 0.5;
int grid[100][100][100];

void setup()
{
	srand(time(NULL));
	for(int x = 0; x<100; x++)
	{
			for(int y = 0; y<100; y++)
		{
				for(int z = 0; z<100; z++)
			{

				if(rand()%100 < 30)
				{
					grid[x][y][z] = 1;
				}
				else
				{
					grid[x][y][z] = 0;
				}

			}
		}
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
