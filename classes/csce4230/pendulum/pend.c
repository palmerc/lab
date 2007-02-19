#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

GLfloat angle = 180.0;
GLint direction = 0;

/* x and y represent the center of the circle */
void drawSolidCircle(float radius, float x, float y)
{
   int i;
   double radians, vectorX, vectorY, lastX, lastY;
   
   lastX = x;
   lastY = y;
   glBegin(GL_TRIANGLES);
   for (i = 0; i < 360; i++)
   {
      /* Multiply an degree angle times pi/180 to get radians */
      radians = (3.14159 * i) / 180;
      vectorX = x + (radius * sin(radians));
      vectorY = y + (radius * cos(radians));
      glVertex2d(x, y);
      glVertex2d(lastX, lastY);
      glVertex2d(vectorX, vectorY);
      lastX = vectorX;
      lastY = vectorY;
   }
   glEnd();
}

void display(void)
{
/* clear all pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   glColor3f (1.0, 1.0, 1.0);
   
   glPushMatrix();
      glRotatef(angle, 0.0, 0.0, 1.0);
      glBegin(GL_LINES);
         glVertex3f (0.0, 0.0, 0.0);
         glVertex3f (0.0, 0.50, 0.0);
      glEnd();
      glColor3f (1.0, 0.0, 0.0);
      drawSolidCircle(0.10, 0.0, 0.60);
   glPopMatrix();
   
   glFinish ();
   glutSwapBuffers();
}

void swing(void)
{
   if (angle < 135.0)
      direction = 0;
   else if (angle > 225.0)
      direction = 1;
   
   if (direction == 0)
      angle += 0.025;
   else if (direction == 1)
      angle -= 0.025;
   
   glutPostRedisplay();
}

void init (void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glEnable(GL_LINE_SMOOTH);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("pendulum");
   init ();
   glutDisplayFunc(display);
   glutIdleFunc(swing);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
