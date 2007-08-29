#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

GLint w = 800;
GLint h = 800;

void display(void)
{
   int seed = 10000; /* Choose any integer value for seed to initialize the random number generator with. The same value of seed will always produce the same sequence of random numbers. To get a different sequence of random numbers next time, choose a different value of seed. */
   double r;		/* random value in range [0,1) */
	long int M = 4;		/* user supplied upper boundary */
	double x;		/* random value in range [0,M) */
	int y;		/* random integer in range [0,M) if M is an integer then range = [0,M-1] */
	int z;		/* random integer in range [1,M+1) if M is an integer then range = [1,M] */
   int i;
   
   GLint currentXPos;
   GLint currentYPos;
   GLint cornerXPos = 0;
   GLint cornerYPos = 0;
   
   srand(seed); /*initialize random number generator*/
   currentXPos = w/2;
   currentYPos = h/2;
   glClear (GL_COLOR_BUFFER_BIT);
   glBegin(GL_POINTS);
      glColor3f (1.0, 1.0, 1.0);
   
   for (i=0; i < 100000; ++i)
   {
      r = ( (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
      /* r is a random floating point value in the range [0,1) {including 0, not including 1}. Note we must convert rand() and/or RAND_MAX+1 to floating point values to avoid integer division. In addition, Sean Scanlon pointed out the possibility that RAND_MAX may be the largest positive integer the architecture can represent, so (RAND_MAX+1) may result in an overflow, or more likely the value will end up being the largest negative integer the architecture can represent, so to avoid this we convert RAND_MAX and 1 to doubles before adding. */
      x = (r * M);
      /* x is a random floating point value in the range [0,M) {including 0, not including M}. */
      y = (int) x;
      /* y is a random integer in the range [0,M) {including 0, not including M}. If M is an integer then the range is [0,M-1] {inclusive} */
      z = y + 1;
      /* z is a random integer in the range [1,M+1) {including 1, not including M+1}. If M is an integer then the range is [1,M] {inclusive} */
      z = z - 1;
      switch (z)
      {
         case 0:
            cornerXPos = w/2;
            cornerYPos = h;
            break;
         case 1:
            cornerXPos = 1;
            cornerYPos = 1;
            break;
         case 2:
            cornerXPos = w;
            cornerYPos = 1;
            break;
      }
      currentXPos = (cornerXPos + currentXPos)/2;
      currentYPos = (cornerYPos + currentYPos)/2;
      glVertex2d (currentXPos, currentYPos);
   }
   glEnd();
   glFinish();
}

void init (void) 
{
/* select clearing color 	*/
   glClearColor (0.0, 0.0, 0.0, 0.0);

/* initialize viewing values  */
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (800, 800); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("sierpenski");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
