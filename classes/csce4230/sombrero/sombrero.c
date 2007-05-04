/*
 * Cameron Palmer
 * CSCE 4230
 * Program 6
 * 
 * 
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* number of grid lines */
#define k 60
/* number of vertices is k+1 squared */
#define nv 3721
/* number of triangles is 2 time k-1 squared */ 
#define nt 6962 

static GLdouble vertices[nv][3];
static GLdouble normals[nv][3];
static GLuint triangles[nt][3];
static GLdouble tri_normals[nv][3];
static int xrot = 0, yrot = 0, zooom = 0, showNormals = 0, flatSmooth = 1, solidOrWireframe = 0;

double z(GLdouble x, GLdouble y)
{
	return ( .5 * exp( -.04 * sqrt( pow( 80 * x - 40, 2 ) + pow( 90 * y - 45, 2 ) ) ) * cos( 0.15 * sqrt( pow( 80 * x - 40, 2 ) + pow( 90 * y - 45, 2 ) ) ) );
}

void calculateVertices(void)
{
	GLuint vi;
	GLdouble x, y, side;
	
	vi = 0;
	side = 1 / (GLdouble) k;
	for (GLuint i = 0; i <= k; ++i)
	{
		y = side * i;
	   	for (GLuint j = 0; j <= k; ++j)
	   	{
	   		x = side * j;
	   		vertices[vi][0] = x;
	   		vertices[vi][1] = y;
	   		vertices[vi][2] = z(x, y);
	   		/* printf("Vertex values %f %f %f\n", x, y, z(x, y)); */
	   		++vi;
	   	}
	}
}

void calculateTriangles(void)
{
	GLuint ai, ti;
	
	ai = 0; /* Array Index */
	ti = 0; /* Triangle Index */
	for (GLuint row = 1; row < k; ++row) /* y */
	{
		for (GLuint col = 1; col < k; ++col) /* x */
		{
			/* We need to create a grid of triangles that indexes back to the vertices */
			ai = col * (k+1) + row;
			triangles[ti][0] = ai - k - 2;
			triangles[ti][1] = ai - k - 1;
			triangles[ti][2] = ai;
#ifdef debug
			printf("%d - %d - %d %d %d\n", ti, ai, triangles[ti][0], triangles[ti][1], triangles[ti][2]);
#endif	
			triangles[ti + 1][0] = ai - k - 2;
			triangles[ti + 1][1] = ai;
			triangles[ti + 1][2] = ai - 1;
#ifdef debug
			printf("%d - %d - %d %d %d\n", ti+1, ai, triangles[ti+1][0], triangles[ti+1][1], triangles[ti+1][2]);
#endif
			ti += 2;
		}
	}
}

void normalize(GLdouble v[3])
{
	GLdouble d = sqrt(pow(v[0],2)+pow(v[1],2)+pow(v[2],2));
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
#ifdef debug
	printf("Cross product %f, %f, %f\n", out[0], out[1], out[2]);
#endif
	normalize(out);
#ifdef debug
	printf("Normalized to %f, %f, %f\n", out[0], out[1], out[2]);
#endif
}

void calculateNormals(void)
{
	/* Once again we have the mapping from a triangle to vertices, and we and normals */
	GLuint i, lptr, mptr, nptr;
	
	for (i = 0; i < nt - 1; ++i)
	{
#ifdef debug
		printf("Round %d\n", i);
#endif
 		lptr = triangles[i][0];
 		mptr = triangles[i][1];
 		nptr = triangles[i][2];
 		
 		GLdouble a[] =
 		{
 			vertices[mptr][0] - vertices[lptr][0],
 			vertices[mptr][1] - vertices[lptr][1],
 			vertices[mptr][2] - vertices[lptr][2]
 		};
 		GLdouble b[] =
 		{
 			vertices[nptr][0] - vertices[lptr][0],
 			vertices[nptr][1] - vertices[lptr][1],
 			vertices[nptr][2] - vertices[lptr][2]
 		};	
 		calculateCrossProduct(a, b, normals[lptr]);
 		calculateCrossProduct(a, b, normals[mptr]);
 		calculateCrossProduct(a, b, normals[nptr]);
 		(normals[lptr][0] + normals[lptr][1] + normals[lptr][2]) / 3;
 	}
#ifdef debug
	printf("Exiting calculateNormals()\n");
#endif
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   
   glColor3d(1.0,0.0,0.0);
   glTranslatef (0.0, 0.0, zooom);
   glRotatef (xrot, 1.0, 0.0, 0.0);
   glRotatef (yrot, 0.0, 1.0, 0.0);
   
   if (solidOrWireframe)
   {   
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   }
   else
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   }     
      
   if (flatSmooth)
   {
   	glShadeModel(GL_SMOOTH);
      glDrawElements(GL_TRIANGLES, 3*nt, GL_UNSIGNED_INT, triangles);
   }
   else
   {
   	glShadeModel(GL_FLAT);
   	glBegin(GL_TRIANGLES);
		for (int i = 0;  i < nt; i++) {
			glNormal3dv(&tri_normals[i][0]);
			glVertex3dv(&vertices[triangles[i][0]][0]);
			glVertex3dv(&vertices[triangles[i][1]][0]);
			glVertex3dv(&vertices[triangles[i][2]][0]);
		}
		glEnd();
   }
   if (showNormals)
   {
      glDisable(GL_LIGHTING);
   	  glColor3d(0.0,0.0,1.0);
   	  glBegin(GL_LINES);
      for (GLint i = 0; i < nv; ++i)
      {
          glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
          glVertex3f(vertices[i][0] + normals[i][0] *.1, vertices[i][1] + normals[i][1]*.1, vertices[i][2] + normals[i][2]*.1);
      }
      glEnd();
      glEnable(GL_LIGHTING);
   }
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
   glTranslatef (0.0, 0.0, -4.0);
}

void menu(int value)
{
   switch (value)
   {
   	case 3:
   		xrot = 0;
   		yrot = 0;
   		zooom = 0;
   		showNormals = 0;
   		flatSmooth = 1;
   		solidOrWireframe = 0;
   		glutPostRedisplay();
   		break;
   	  case 4:
   	     if (solidOrWireframe == 1)
   	     {
   	        solidOrWireframe = 0;
   	     }
   	     else
   	     {
   	        solidOrWireframe = 1;
   	     }
   	     glutPostRedisplay();
   	     break;
      case 5:
   	     if (flatSmooth == 1)
   	     {
   	        flatSmooth = 0;
   	     }
   	     else
   	     {
   	        flatSmooth = 1;
   	     }
   	     glutPostRedisplay();
         break;
   	  case 6:
   	     if (showNormals == 1)
   	     {
   	        showNormals = 0;
   	     }
   	     else
   	     {
   	        showNormals = 1;
   	     }
   	     glutPostRedisplay();
         break;
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
   	case 'r':
   		menu(3);
   		break;
   	  case 'f':
   	     menu(4);
   	     break;
      case 's': /* Toggle function display on/off */
   	     menu(5);
   	     break;
   	  case 'n': /* Toggle normal display on/off */
   	     menu(6);
   	     break;
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
   glVertexPointer(3, GL_DOUBLE, 0, vertices);
   glNormalPointer(GL_DOUBLE, 0, normals);
  
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

int main(int argc, char** argv)
{
   int m;
   calculateVertices();
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
   glutAddMenuEntry("Toggle solid/wireframe", 4);
   glutAddMenuEntry("Toggle hide/show shape", 5);
   glutAddMenuEntry("Toggle normals", 6);
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
