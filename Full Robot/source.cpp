#include <stdlib.h>
#include <GL\glut.h>
#include <GL\freeglut.h>
#include <math.h>


#define PI 3.14

static int Rshoulderx = 0, Rshoulderz = 0, Relbowx = 0, RHIPx = 0, RKNEEx = 0, RHIPz = 0, Rfingerup=0,Rfingerbase=0, Lfingerup = 0, Lfingerbase = 0,
Lshoulderx = 0, Lshoulderz = 0, Lelbowx = 0, LHIPx = 0, LKNEEx = 0, LHIPz = 0;
//camera
double eye[] = { 0, 0, 2 };
double center[] = { 0, 0, 0 };
double up[] = { 0.0, 1.0, 0.0 };

int moving, startx, starty;


GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */


void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    // cam
    glMatrixMode(GL_PROJECTION); // cam
    gluPerspective(60.0, 1.0, 1.0, 20.0); // cam
}

void crossProduct(double a[], double b[], double c[])// cam
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[]) // cam
{
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}
void rotatePoint(double a[], double theta, double p[]) // cam
{

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta) * p[0];
    temp[1] += cos(theta) * p[1];
    temp[2] += cos(theta) * p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];
}

void reset() // cam
{
    double e[] = { 0.0, 0.0, 2 };
    double c[] = { 0.0, 0.0, 1.0 };
    double u[] = { 0.0, 1.0, 0.0 };
    for (int i = 0; i < 3; i++)
    {
        eye[i] = e[i];
        center[i] = c[i];
        up[i] = u[i];
    }
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    gluLookAt(eye[0], eye[1], eye[2],
        center[0], center[1], center[2],
        up[0], up[1], up[2]); // cam
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);

    //Draw body 
    glPushMatrix();
    glScalef(3, 5, 1.0);
    glutWireCube(1.0);
    glPopMatrix();

    //Draw head
    glPushMatrix();
    glTranslatef(0, 4, 0);
    glutWireSphere(1, 10, 10);
    glPopMatrix();

    // Draw  arm R
    glPushMatrix();
    glTranslatef(2, 2.25, 0);
    glRotatef((GLfloat)Rshoulderx, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)Rshoulderz, 0.0, 0.0, 1.0);
    glTranslatef(0, -0.75, 0);
    glPushMatrix();
    glScalef(0.5, 2, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -1.1, 0);
    glRotatef((GLfloat)Relbowx, 1.0, 0.0, 0.0);
    glTranslatef(0, -1, 0);
    glPushMatrix();
    glScalef(0.5, 2, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
        // fingers
        glPushMatrix();
        glTranslatef(0.15, -1, 0.4);
        glRotatef((GLfloat)Rfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();    
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Rfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.15, -1, 0.15);
        glRotatef((GLfloat)Rfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Rfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.15, -1, -0.15);
        glRotatef((GLfloat)Rfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Rfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.15, -1, -0.4);
        glRotatef((GLfloat)Rfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Rfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();


        glPushMatrix();
        glTranslatef(-0.15, -1, 0.4);
        glRotatef((GLfloat)Rfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Rfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();

    glPopMatrix();
    glPopMatrix();


    // Draw  arm l
    glPushMatrix();
    glTranslatef(-2, 2.25, 0);
    glRotatef((GLfloat)Lshoulderx, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)Lshoulderz, 0.0, 0.0, 1.0);
    glTranslatef(0, -0.75, 0);
    glPushMatrix();
    glScalef(0.5, 2, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -1.1, 0);
    glRotatef((GLfloat)Lelbowx, 1.0, 0.0, 0.0);
    glTranslatef(0, -1, 0);
    glPushMatrix();
    glScalef(0.5, 2, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
        // fingers
        glPushMatrix();
        glTranslatef(-0.15, -1, 0.4);
        glRotatef((GLfloat)Lfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Lfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.15, -1, 0.15);
        glRotatef((GLfloat)Lfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Lfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.15, -1, -0.15);
        glRotatef((GLfloat)Lfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Lfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.15, -1, -0.4);
        glRotatef((GLfloat)Lfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Lfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.15, -1, 0.4);
        glRotatef((GLfloat)Lfingerbase, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glTranslatef(0, -0.3, 0);
        glRotatef((GLfloat)Lfingerup, 0.0, 0.0, 1.0);
        glTranslatef(0, -0.3, 0);
        glPushMatrix();
        glScalef(0.2, 0.6, 0.2);
        glutWireCube(1.0);
        glPopMatrix();
        glPopMatrix();
    
    glPopMatrix();
    glPopMatrix();



    // Draw  LEG AND FOOT R
    glPushMatrix();
    glTranslatef(1, -2, 0);
    glRotatef((GLfloat)RHIPx, 1, 0, 0);
    glRotatef((GLfloat)RHIPz, 0, 0, 1);
    glTranslatef(0, -2, 0);
    glPushMatrix();
    glScalef(1, 3, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -1.6, 0);
    glRotatef((GLfloat)RKNEEx, 1, 0, 0);
    glTranslatef(0, -1.5, 0);
    glPushMatrix();
    glScalef(1, 3, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -1.6, 0);
    glScalef(1, 0.2, 2);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    // Draw  LEG AND FOOT L
    glPushMatrix();
    glTranslatef(-1, -2, 0);
    glRotatef((GLfloat)LHIPx, 1, 0, 0);
    glRotatef((GLfloat)LHIPz, 0, 0, 1);    glTranslatef(0, -2, 0);
    glPushMatrix();
    glScalef(1, 3, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -1.6, 0);
    glRotatef((GLfloat)LKNEEx, 1, 0, 0);
    glTranslatef(0, -1.5, 0);
    glPushMatrix();
    glScalef(1, 3, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -1.6, 0);
    glScalef(1, 0.2, 2);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();


        
    

    glPopMatrix();
    glutSwapBuffers();
}


void rotateCameraRight() // cam
{
    rotatePoint(up, PI / 20, eye);
}

void rotateCameraLeft() // cam
{
    rotatePoint(up, -PI / 20, eye);
}

// Rotation about horizontal direction

void rotateCameraUp() // cam
{
    double horizontal[3];
    double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, look, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, PI / 20, eye);
    rotatePoint(horizontal, PI / 20, up);
}

void rotateCameraDown() // cam
{
    double horizontal[3];
    double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, look, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, -PI / 20, eye);
    rotatePoint(horizontal, -PI / 20, up);
}

// Forward and Backward
void zoomIn() // cam
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    double speed = 0.1;
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}

void zoomOut() // cam
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    double speed = 0.1;
    eye[0] -= direction[0] * speed;
    eye[1] -= direction[1] * speed;
    eye[2] -= direction[2] * speed;

    center[0] -= direction[0] * speed;
    center[1] -= direction[1] * speed;
    center[2] -= direction[2] * speed;

}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -15.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '0': // cam
        reset();
        glutPostRedisplay();
        break;

    case '+': // cam
        zoomIn();
        glutPostRedisplay();
        break;
    case '-': // cam
        zoomOut();
        glutPostRedisplay();
    break;
    case 'R':
        if (Rshoulderx < 70 % 360)
            Rshoulderx = (Rshoulderx + 5) % 360;

        glutPostRedisplay();
        break;
    case 'r':
        if (Rshoulderx > -170 % 360)
            Rshoulderx = (Rshoulderx - 5) % 360;

        glutPostRedisplay();
        break;
    case 't':
        if (Rshoulderz < 160 % 360)
            Rshoulderz = (Rshoulderz + 5) % 360;

        glutPostRedisplay();
        break;
    case 'T':
        if (Rshoulderz >-2)
            Rshoulderz = (Rshoulderz - 5) % 360;

        glutPostRedisplay();
        break;

    case 'E':
        if (Lshoulderx < 70 % 360)
            Lshoulderx = (Lshoulderx + 5) % 360;

        glutPostRedisplay();
        break;
    case 'e':
        if (Lshoulderx > -170 % 360)
            Lshoulderx = (Lshoulderx - 5) % 360;

        glutPostRedisplay();
        break;
    case 'w':
        if (Lshoulderz > -160 % 360)
            Lshoulderz = (Lshoulderz - 5) % 360;

        glutPostRedisplay();
        break;
    case 'W':
        if (Lshoulderz < 2)
            Lshoulderz = (Lshoulderz + 5) % 360;
        glutPostRedisplay();
        break;



    case 'd':
        if (Relbowx > -160)
            Relbowx = (Relbowx - 5) % 360;

        glutPostRedisplay();
        break;
    case 'D':
        if (Relbowx < 0)
            Relbowx = (Relbowx + 5) % 360;

        glutPostRedisplay();
        break;
    case 's':
        if (Lelbowx > -160)
            Lelbowx = (Lelbowx - 5) % 360;

        glutPostRedisplay();
        break;
    case 'S':
        if (Lelbowx < 0)
            Lelbowx = (Lelbowx + 5) % 360;

        glutPostRedisplay();
        break;

    case 'O':
        if (RHIPx < 70 % 360)
            RHIPx = (RHIPx + 5) % 360;

        glutPostRedisplay();
        break;
    case 'o':
        if (RHIPx > -150 % 360)
            RHIPx = (RHIPx - 5) % 360;

        glutPostRedisplay();
        break;
    case 'p':
        if (RHIPz < 100 % 360)
            RHIPz = (RHIPz + 5) % 360;

        glutPostRedisplay();
        break;
    case 'P':
        if (RHIPz > -20)
            RHIPz = (RHIPz - 5) % 360;

        glutPostRedisplay();
        break;

    case 'I':
        if (LHIPx < 100 % 360)
            LHIPx = (LHIPx + 5) % 360;

        glutPostRedisplay();
        break;
    case 'i':
        if (LHIPx > -150 % 360)
            LHIPx = (LHIPx - 5) % 360;

        glutPostRedisplay();
        break;
    case 'u':
        if (LHIPz > -110 % 360)
            LHIPz = (LHIPz - 5) % 360;

        glutPostRedisplay();
        break;
    case 'U':
        if (LHIPz < 20)
            LHIPz = (LHIPz + 5) % 360;
        glutPostRedisplay();
        break;



    case 'l':
        if (RKNEEx < 130)
            RKNEEx = (RKNEEx + 5) % 360;
        glutPostRedisplay();
        break;
    case 'L':
        if (RKNEEx > -5)
            RKNEEx = (RKNEEx - 5) % 360;

        glutPostRedisplay();
        break;


    case 'k':
        if (LKNEEx < 130)
            LKNEEx = (LKNEEx + 5) % 360;
        glutPostRedisplay();
        break;
    case 'K':
        if (LKNEEx > -5)
            LKNEEx = (LKNEEx - 5) % 360;

        glutPostRedisplay();
        break;
    case 'f':
        if (Lfingerbase < 90) {

            Rfingerbase = (Rfingerbase - 5) % 360;
            Lfingerbase = (Lfingerbase + 5) % 360;
        }
        glutPostRedisplay();
        break;

    case 'g':
        if (Lfingerup < 90) {
            Rfingerup = (Rfingerup - 5) % 360;
            Lfingerup = (Lfingerup + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'F':
        if (Lfingerbase>0) {
            Rfingerbase = (Rfingerbase + 5) % 360;
            Lfingerbase = (Lfingerbase - 5) % 360;
        }
        glutPostRedisplay();
        break;

    case 'G':
        if (Lfingerup > 0) {
            Rfingerup = (Rfingerup + 5) % 360;
            Lfingerup = (Lfingerup - 5) % 360;
        }
        glutPostRedisplay();
        break;
    }

}

void cameraKeys(int key, int x, int y) // cam
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        rotateCameraLeft();
        break;
    case GLUT_KEY_RIGHT:
        rotateCameraRight();
        break;
    case GLUT_KEY_UP:
        rotateCameraUp();
        break;
    case GLUT_KEY_DOWN:
        rotateCameraDown();
        break;
    }
    glutPostRedisplay();
}


static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}


static void motion(int x, int y)
{
    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(cameraKeys); // cam
    glutMainLoop();
    return 0;
}