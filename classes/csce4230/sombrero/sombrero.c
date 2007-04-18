/*
 * robot.c
 * This program shows how to composite modeling transformations
 * to draw translated and rotated hierarchical models.
 * Interaction:  pressing the s and e keys (shoulder and elbow)
 * alters the rotation of the robot arm.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define k 60
#define nv 3721
#define nt 7200

static GLdouble vertex_array[nv][3];
static GLdouble vertex_normals[nv][3];
static GLuint tri[nt][3];
static int xrot = 0, yrot = 0, zooom = 0;

void init(void) 
{
   GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 0.9 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 0.5, 0.5, 1.0 };
   GLfloat light_position[] = { 0.0, -2.0, 0.0, 0.0 };
   
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glVertexPointer(3, GL_DOUBLE, 0, vertex_array);
   glNormalPointer(GL_DOUBLE, 0, vertex_normals);
  
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

double z(GLdouble x, GLdouble y)
{
	return ( .5 * exp( -.04 * sqrt( pow( 80 * x - 40, 2 ) + pow( 90 * y - 45, 2 ) ) ) * cos( 0.15 * sqrt( pow( 80 * x - 40, 2 ) + pow( 90 * y - 45, 2 ) ) ) );
}

void normalize(GLdouble v[3])
{
	GLdouble d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (d == 0.0)
	{
		perror("zero length vector");
		return;
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}

void calculateCrossProduct(GLdouble v1[3], GLdouble v2[3], GLdouble out[3])
{
	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];
	normalize(out);
}

void calculateTriangles(void)
{
	GLuint i, j, iv, index;
	
	iv = 0;
	index = 0;
	for (i = 1; i < k; ++i) /* y */
	{
		for (j = 1; j < k; ++j) /* x */
		{
			iv = i * (k+1) + j;
			tri[index][0] = iv - k - 2;
			tri[index][1] = iv - k - 1;
			tri[index][2] = iv;
			
			tri[index + 1][0] = iv - k - 2;
			tri[index + 1][1] = iv;
			tri[index + 1][2] = iv - 1;
			index += 2;
		}
	}
}

void calculateShape(void)
{
	GLuint i, j, vertex_index;
	GLdouble x, y, side;
	
	vertex_index = 0;
	side = 1 / (GLdouble) k;
	for (i = 0; i < k + 1; ++i)
	{
		y = side * i;
	   	for (j = 0; j < k + 1; ++j)
	   	{
	   		x = side * j;
	   		vertex_array[vertex_index][0] = x;
	   		vertex_array[vertex_index][1] = y;
	   		vertex_array[vertex_index][2] = z(x, y);
	   		/* printf("Vertex values %f %f %f\n", x, y, z(x, y)); */
	   		++vertex_index;
	   	}
	}
}

void calculateNormals(void)
{
	GLuint i;
	for (i = 0; i < nv; ++i)
	{
		calculateCrossProduct(vertex_array[i], vertex_array[i+1], vertex_normals[i]);
	}
	for (i = 0; i < nv; ++i)
	{
		printf("Normals=> %f %f %f\n", vertex_normals[i][0],vertex_normals[i][1],vertex_normals[i][2]);
	}
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   
   glColor3d(1.0,1.0,1.0);
   glTranslatef (0.0, 0.0, zooom);
   glRotatef (xrot, 1.0, 0.0, 0.0);
   glRotatef (yrot, 0.0, 1.0, 0.0);
   
   /*glDrawElements(GL_TRIANGLES, 3 * nt, GL_UNSIGNED_INT, tri);*/
   glDrawElements(GL_LINES, nv, GL_DOUBLE, vertex_normals);

   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 24.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

void menu(int value)
{
   switch (value)
   {
	  case 7:
         xrot = (xrot + 5) % 360;
         glutPostRedisplay();
         break;
      case 8:
         xrot = (xrot - 5) % 360;
         glutPostRedisplay();
         break;
	  case 9:
         yrot = (yrot + 5) % 360;
         glutPostRedisplay();
         break;
      case 10:
         yrot = (yrot - 5) % 360;
         glutPostRedisplay();
         break;
      case 11:
         zooom -= 1.0;
         glutPostRedisplay();
       	 break;
      case 12:
      	 zooom += 1.0;
      	 glutPostRedisplay();
      	 break;
      case 13:
         exit(0);
         break;
      default:
         break;
   }
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
	  case 'x':
         menu(7);
         break;
      case 'X':
         menu(8);
         break;
	  case 'y':
         menu(9);
         break;
      case 'Y':
         menu(10);
         break;
      case 'z':
         menu(11);
         break;
      case 'Z':
         menu(12);
         break;
      case 27:
         menu(13);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   int m;
   calculateShape();
   calculateTriangles();
   calculateNormals();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (800, 800); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   m = glutCreateMenu(menu);
   glutAddMenuEntry("x Rotate CCW", 7);
   glutAddMenuEntry("X Rotate CW", 8);
   glutAddMenuEntry("y Rotate CCW", 9);
   glutAddMenuEntry("Y Rotate CW", 10);
   glutAddMenuEntry("z Zoom Out", 11);
   glutAddMenuEntry("Z Zoom In", 12);
   glutAddMenuEntry("Exit", 13);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   glutMainLoop();
   return 0;
}
