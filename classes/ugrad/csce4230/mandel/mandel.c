#include <GL/glut.h>
#include <stdio.h>

const int w = 1024;
const int h = 768;

void display(void)
{
   int maxIterations = 2048;
   int maxSize = 4;
   float Xmin = -2.0;
   float Xmax = 1.2;
   float Ymin = -1.2;
   float Ymax = 1.2;
 
   double X, Y;
   double Xsquare, Ysquare;
   double currentP;
   double currentQ[767];
   double deltaP, deltaQ;
   int color;
   int currentRow, currentCol;
 
   /* clear all pixels  */
   glClear (GL_COLOR_BUFFER_BIT);
   
   deltaP = (Xmax - Xmin)/(double)w;
 
   deltaQ = (Ymax - Ymin)/(double)h;
 
   currentQ[0] = Ymax;
   for (currentRow = 1; currentRow < h; currentRow++) {
      currentQ[currentRow] = currentQ[currentRow-1] - deltaQ;
   }
   currentP = Xmin;
 
   glBegin(GL_POINTS);
   for (currentCol = 0; currentCol < w; currentCol++) {
      for (currentRow = 0; currentRow < h; currentRow++) {
         X = 0.0;
         Y = 0.0;
         Xsquare = 0.0;
         Ysquare = 0.0;
         color = 0;
         while ((color <= maxIterations) && (Xsquare + Ysquare <= maxSize)) {
            Xsquare = X * X;
            Ysquare = Y * Y;
            Y = (2*X*Y) + currentQ[currentRow];
            X = (Xsquare - Ysquare) + currentP;
            color++;
         }
             /*gl_setpixel(currentCol, currentRow, color % 256);*/
            /* draw white polygon (rectangle) with corners at
            * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)  
            */
            glColor3f ((float)(0.2/(color % 256)), (float)(0.2/(color % 256)), (float)(7.0/(color % 256)));
            glVertex2i (currentCol, currentRow);
            glRasterPos2i(currentCol, currentRow);
      }
      currentP = currentP + deltaP;
      /* don't wait!  
      * start processing buffered OpenGL routines 
      */
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
   glOrtho(0.0, w, h, 0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/* 
 * Declare initial window size, position, and display mode

 * (single buffer and RGBA).  Open window with "hello"
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (1024, 768); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("mandelbrot");
   init ();
   glutDisplayFunc(display); 
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
