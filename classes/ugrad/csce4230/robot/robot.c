/*
 * robot.c
 * This program shows how to composite modeling transformations
 * to draw translated and rotated hierarchical models.
 * Interaction:  pressing the s and e keys (shoulder and elbow)
 * alters the rotation of the robot arm.
 */
#include <GL/glut.h>
#include <stdlib.h>

static int shoulder = 0, elbow = 0, hand = 0, xrot = 0, yrot = 0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   
   glTranslatef (0.0, 0.0, -8.0);
   glRotatef (xrot, 1.0, 0.0, 0.0);
   glRotatef (yrot, 0.0, 1.0, 0.0);
   
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
   glTranslatef (2.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (4.0, 1.0, 1.0);
   glColor3d(1.0, 0.0, 0.0);
   glutWireCube (1.0);
   glPopMatrix();

   glTranslatef (2.0, 0.0, 0.0);
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
   glTranslatef (2.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (4.0, 1.0, 1.0);
   glColor3d(0.0, 1.0, 0.0);
   glutWireCube (1.0);
   glPopMatrix();

   glTranslatef (2.0, 0.0, 0.0);
   glRotatef ((GLfloat) hand, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 1.0, 1.0);
   glColor3d(0.0, 0.0, 1.0);
   glutWireCube (1.0);
   glPopMatrix();

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
      case 1:
         shoulder = (shoulder + 5) % 360;
         glutPostRedisplay();
         break;
      case 2:
         shoulder = (shoulder - 5) % 360;
         glutPostRedisplay();
         break;
      case 3:
         elbow = (elbow + 5) % 360;
         glutPostRedisplay();
         break;
      case 4:
         elbow = (elbow - 5) % 360;
         glutPostRedisplay();
         break;
	  case 5:
         hand = (hand + 5) % 360;
         glutPostRedisplay();
         break;
      case 6:
         hand = (hand - 5) % 360;
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
         exit(0);
         break;
      default:
         break;
   }
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 's':
         menu(1);
         break;
      case 'S':
         menu(2);
         break;
      case 'e':
		 menu(3);
		 break;
      case 'E':
         menu(4);
         break;
	  case 'w':
         menu(5);
         break;
      case 'W':
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
      case 27:
         exit(11);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   int m;

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
   glutAddMenuEntry("Shoulder CCW", 1);
   glutAddMenuEntry("Shoulder CW", 2);
   glutAddMenuEntry("Elbow CCW", 3);
   glutAddMenuEntry("Elbow CW", 4);
   glutAddMenuEntry("Hand CCW", 5);
   glutAddMenuEntry("Hand CW", 6);
   glutAddMenuEntry("X Rotate CCW", 7);
   glutAddMenuEntry("X Rotate CW", 8);
   glutAddMenuEntry("Y Rotate CCW", 9);
   glutAddMenuEntry("Y Rotate CW", 10);
   glutAddMenuEntry("Exit", 11);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   glutMainLoop();
   return 0;
}
