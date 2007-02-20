#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static GLfloat spin = 180.0;
static GLfloat r = 1.0;
static GLfloat l = 10.0;
static GLuint direction = 0;

void drawSolidCircle(void)
{
   GLUquadric *qobj;
   /* Note that no positional information is contained in this function */
   qobj = gluNewQuadric();

   gluQuadricDrawStyle(qobj, GLU_FILL); /* smooth shaded */
   gluDisk(qobj, 0.0, r, 60, 1);
}

void spinDisplay(void)
{
+_+++   if (direction == 0)
   {
      spin = spin - 0.1;
      if (spin < 0.0)
         spin = spin + 360.0;
   }
   else if (direction == 1)
   {
      spin = spin + 0.1;
      if (spin > 360.0)
         spin = spin - 360.0;
   }
   
   glutPostRedisplay();
}

void reshape(int w, int h)
{
   GLfloat a = 1.25 * l;
   
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
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
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(-a, a, -a, a);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void display(void)
{
/* clear all pixels  */
   glClear (GL_COLOR_BUFFER_BIT);
  
   glPushMatrix();    
      glRotatef(-spin, 0.0, 0.0, 1.0);
      glColor3f (1.0, 1.0, 1.0);
      glBegin(GL_LINES);
         glVertex2f (0.0, 0.0);
         glVertex2f (0.0, l);
      glEnd();
      glTranslatef(0.0, l, 0.0);
      glColor3f (1.0, 0.0, 0.0);
      drawSolidCircle();
   glPopMatrix();
   glutSwapBuffers();   
}

void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(spinDisplay);
         break;
         
      case GLUT_MIDDLE_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;
      default:
         break;
   }
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 27:
         exit(0);
         break;
      case 32:
         glutIdleFunc(NULL);
         break;
      case 43:
         direction = 0; /* Plus - counterclockwise */
         glutIdleFunc(spinDisplay);
         break;
      case 45:
         direction = 1; /* Minus - clockwise */
         glutIdleFunc(spinDisplay);
         break;
   }
}

void init (void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("pendulum");
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
