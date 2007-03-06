/*
 *  cube.c
 *  This program demonstrates a single modeling transformation,
 *  glScalef() and a single viewing transformation, gluLookAt().
 *  A wireframe cube is rendered.
 */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

static GLuint direction = 0;
static GLint xrot = 0;
static GLint yrot = 0;
static GLint zrot = 0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void spinDisplay(void)
{
   if (direction == 0)
   {
      xrot -= 5;
      if (xrot < 0)
         xrot += 360;
   }
   else if (direction == 1)
   {
      xrot += 5;
      if (xrot > 360)
         xrot -= 360;
   }
   else if (direction == 2)
   {
      yrot -= 5;
      if (yrot < 0)
         yrot += 360;
   }
   else if (direction == 3)
   {
      yrot += 5;
      if (yrot > 360)
         yrot -= 360;
   }
   else if (direction == 4)
   {
      zrot -= 5;
      if (zrot < 0)
         zrot += 360;
   }
   else if (direction == 5)
   {
      zrot += 5;
      if (zrot > 360)
         zrot -= 360;
   }
   glutPostRedisplay();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   glColor3f (1.0, 1.0, 1.0);
   glLoadIdentity (); /* clear the matrix */
   /* viewing transformation */
   glTranslated(0.0, 0.0, -5.0);
   glScalef (1.0, 2.0, 3.0); /* modeling transformation */ 
   glPushMatrix();    
      glRotatef(-xrot, 1.0, 0.0, 0.0);
      glRotatef(-yrot, 0.0, 1.0, 0.0);
      glRotatef(-zrot, 0.0, 0.0, 1.0);
      glutWireCube (1.0);
   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   if (w < h)
   {
      w = h;
      glutReshapeWindow((GLsizei) w, (GLsizei) h);
   }
   else if (h < w)
   {
      h = w;
      glutReshapeWindow((GLsizei) w, (GLsizei) h);
   }
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

void menu(int value)
{
   switch (value)
   {
      case 0:
         direction = 0;
         spinDisplay();
         break;
      case 1:
         direction = 1;
         spinDisplay();
         break;
      case 2:
         direction = 2;
         spinDisplay();
         break;
      case 3:
         direction = 3;
         spinDisplay();
         break;
      case 4:
         direction = 4;
         spinDisplay();
         break;
      case 5:
         direction = 5;
         spinDisplay();
         break;
      case 6:
         break;            
      case 7:
         exit(0);
         break;
   }
   printf ("X = %d, Y = %d, Z = %d\n", xrot, yrot, zrot);
}

void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         break;
      case GLUT_MIDDLE_BUTTON:
         break;
      default:
         break;
   }
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         menu(7);
         break;
      case 88: /* X */
         menu(0);
         break;
      case 89: /* Y */
         menu(2);
         break;
      case 90: /* Z */
         menu(4);
         break;
      case 120: /* x */
         menu(1);
         break;
      case 121: /* y */
         menu(3);
         break;
      case 122: /* z */
         menu(5);
         break;
   }
}

int main(int argc, char** argv)
{
   int m;
   
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   m = glutCreateMenu(menu);
   glutAddMenuEntry("X-clockwise", 1);
   glutAddMenuEntry("X-counter-clockwise", 2);
   glutAddMenuEntry("Y-clockwise", 3);
   glutAddMenuEntry("Y-counter-clockwise", 4);
   glutAddMenuEntry("Z-clockwise", 5);
   glutAddMenuEntry("Z-counter-clockwise", 6);
   glutAddMenuEntry("Exit", 7);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   glutMainLoop();
   return 0;
}
