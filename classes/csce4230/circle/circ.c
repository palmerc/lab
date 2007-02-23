#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

const double PI = 3.141592653589;
const int circle_points = 60;
static GLfloat spin = 0.0;


void init (void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);
}

void pointsCircle (GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
{
   GLint i;
   GLdouble theta;
   
   glBegin(GL_POINTS);
   for (i = 0; i < circle_points; i++)
   {
      theta = 2 * PI * i / circle_points;
      glVertex3f(x + radius * cos(theta), y, z + radius * sin(theta));
   }
   glEnd();
}

void linesCircle (GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
{
   GLint i;
   GLdouble theta;
   
   glBegin(GL_LINE_LOOP);
   for (i = 0; i < circle_points; i++)
   {
      theta = 2 * PI * i / circle_points;
      glVertex3f(x + radius * cos(theta), y + radius * sin(theta), z);
   }
   glEnd();
}

void displayAxis(void)
{
   glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(-20.0, 0.0, 0.0);
      glVertex3f(20.0, 0.0, 0.0);
   glEnd();
   
   glBegin(GL_LINES);
      glColor3f(0.0, 1.0, 0.0);
      glVertex3f(0.0, -20.0, 0.0);
      glVertex3f(0.0, 20.0, 0.0);
   glEnd();
   
   glBegin(GL_LINES);
      glColor3f(0.0, 0.0, 1.0);
      glVertex3f(0.0, 0.0, -20.0);
      glVertex3f(0.0, 0.0, 20.0);
   glEnd();
   
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   displayAxis();
   glColor3f(1.0, 1.0, 1.0);
   glPushMatrix();
   glRotatef(spin, 1.0, 1.0, 0.0);
   linesCircle(0, 0, 0, 20);
   glPopMatrix();
   glutSwapBuffers();
}

void spinDisplay(void)
{
   spin = spin + 0.01;
   if (spin > 360.0)
      spin = spin - 360.0;
   glutPostRedisplay();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 30.0);
   glRotatef(45, 1.0, 1.0, 0.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
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
      case 43:
         glutIdleFunc(spinDisplay);
         break;
      case 32:
         glutIdleFunc(NULL);
         break;
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (450, 450); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
