/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

//disables serial communication attempts
//#define NO_ROBOT

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>

#include <SerialStream.h>
using namespace LibSerial ;
#ifndef NO_ROBOT
SerialPort Serial("/dev/ttyUSB0");
#endif
#include <stdlib.h>
#include <math.h>
#include <vector>
static int slices = 16;
static int stacks = 16;
#define WW 640
#define HH 480
int w=WW,h=HH;
int drag=0;
int rotm=0;


class Point
{
public:
    Point(double _x,double _y,double _z);

    double x;
    double y;
    double z;

};
Point::Point(double _x,double _y,double _z)
{
    x=_x;
    y=_y;
    z=_z;
}

using namespace std;
vector<Point> checkp;
/* GLUT callback Handlers */

double zoomFactor=1.0;
static void resize(int width, int height)
{
    w=width;
    h=height;
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glFrustum(-ar, ar, -1.0, 1.0, 1, 10.0);
    gluPerspective (50.0*zoomFactor, (float)width/(float)height, .1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

#define RESOL 1
#define L 90 * RESOL
#define W 90 * RESOL
#define H 3 * RESOL


#define STRONG glColor4d(1,1,1,.5);
#define WEAK continue;
//#define WEAK glColor3d(.05,.05,.05);

#define pA glVertex3d(-.2,.2,0);
#define pB glVertex3d(-.2,-.2,0);
#define pC glVertex3d(.2,-.2,0);
#define pD glVertex3d(.2,.2,0);

#define pE glVertex3d(-.2,.2,.2);
#define pF glVertex3d(-.2,-.2,.2);
#define pG glVertex3d(.2,-.2,.2);
#define pH glVertex3d(.2,.2,.2);
float cursor_x=45,cursor_y=45,cursor_z=0;
void draw_checkpoint();

void lineW()
{
    glLineWidth(2.0);
}

void draw_robot()
{
    glColor3d(0.0,0.0,1.0);

    glBegin(GL_LINES);
    pA pB
    pB pC
    pC pD
    pD pA

    pE pF
    pF pG
    pG pH
    pH pE

    pA pE
    pB pF
    pC pG
    pD pH

    glVertex3d(-.15,0,.1);
    glVertex3d(.15,0,.1);

    glVertex3d(.15,0,.1);
    glVertex3d(.1,-.05,.1);

    glVertex3d(.15,0,.1);
    glVertex3d(.1,.05,.1);

    glEnd();
}

void draw_grid()
{
    glBegin(GL_LINES);
    glColor3d(.5,.5,.5);
    int i,j;
    for(i=0; i<=L/RESOL; ++i)
    {
        for(j=0; j<=H/RESOL; ++j)
        {
            if(j==0)
                STRONG
                else
                    WEAK
                    glVertex3d(i*RESOL,0.0,j*RESOL);
            glVertex3d(i*RESOL,W,j*RESOL);
        }
    }

    for(i=0; i<=W/RESOL; ++i)
    {
        for(j=0; j<=H/RESOL; ++j)
        {
            if(j==0)
                STRONG
                else
                    WEAK
                    glVertex3d(0.0,i*RESOL,j*RESOL);
            glVertex3d(L,i*RESOL,j*RESOL);
        }
    }


    for(i=0; i<=L/RESOL; ++i)
    {
        WEAK
        for(j=0; j<=W/RESOL; ++j)
        {
            glVertex3d(i*RESOL,j*RESOL,.0);
            glVertex3d(i*RESOL,j*RESOL,H);
        }
    }
    glEnd();
}
#define RBUF_SIZE 1000
char rbuf[RBUF_SIZE];
int rb_cur=0;
#define PI 3.14159265359
double dist_look=5.0;
double theta=PI/4.0;//-pi->pi
double phi=PI/4.0; //0->pi/2

double transx=L/2;
double transy=W/2;


double r_x=L/2, r_y=W/2, r_a=0.0;
int iter=0;
double lastpoll=0;


int mprevx=0, mprevy=0;
int specialKey;
void processMouse(int button, int state, int x, int y)
{
    //printf("*************************************\n");

    specialKey = glutGetModifiers();
    // if both a mouse button, and the ALT key, are pressed  then
    if ((state == GLUT_DOWN) &&
            (specialKey == GLUT_ACTIVE_SHIFT))
    {

        // set the color to pure red for the left button
        if (button == GLUT_LEFT_BUTTON)
        {
//			red = 1.0; green = 0.0; blue = 0.0;
            checkp.push_back(Point(cursor_x,cursor_y,cursor_z));
            //      printf("add\n");

        }
    }
    if ((state == GLUT_DOWN) &&
            (specialKey != GLUT_ACTIVE_SHIFT))
    {

        // set the color to pure red for the left button
        if (button == GLUT_LEFT_BUTTON)
        {
//			red = 1.0; green = 0.0; blue = 0.0;
            checkp.clear();
            checkp.push_back(Point(cursor_x,cursor_y,cursor_z));

        }
    }
    // set the color to pure green for the middle button
    if (button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            rotm=1;
        }
        else if(state == GLUT_UP)
        {
            rotm = 0;
        }
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            drag=1;
        }
        else if(state == GLUT_UP)
        {
            drag = 0;
        }
    }
    if (button == 3)
    {
        dist_look/=1.1;
    }
    if (button == 4)
    {
        dist_look*=1.1;
    }
}


void processMousePassiveMotion(int x, int y)
{

    // User must press the SHIFT key to change the
    // rotation in the X axis
    if(!drag && !rotm)
    {
        double dpx = tan(50.0*PI/180.0);
        double dpy = tan(50*h/w*PI/180.0);

        float xx=(float)x / w * 2.0 - 1.0;
        float yy=(float)y / h * 2.0 - 1.0;

        //float angx = atan(xx/dpy)/1.7;//xx*50.0*PI/2.0/180.0*zoomFactor;
        //float angy = atan(yy/dpx)/1.7;//-yy*50.0*PI/2.0/180.0/zoomFactor * h/w;
        float angx = xx*50.0*PI/2.0/180.0*zoomFactor*1.27;
        float angy = yy*50.0*PI/2.0/180.0/zoomFactor * h/w*1.3;

        double eye_x=dist_look*cos(theta)*cos(phi);
        double eye_y=dist_look*sin(theta)*cos(phi);
        double eye_z=dist_look*sin(phi);


        double y_pretran = eye_z/tan(-phi-angy);


        double d2 = eye_z/sin(-phi-angy);

        double x_pretran = d2*tan(-angx);

        double z2 = 0.0;
        double x2 = x_pretran*sin(-theta) + y_pretran * cos(-theta)+eye_x+transx;//eye_x- d2 * cos(theta-angx) * cos(phi - angy) + transx;
        double y2 = x_pretran*cos(-theta) - y_pretran * sin(-theta)+eye_y+transy;//eye_y- d2 * sin(theta-angx) * cos(phi - angy) + transy;

        cursor_x = x2;
        cursor_y = y2;
        cursor_z = z2;
    }
//    printf("passive ax=%f  ay=%f  x2=%f  y2=%f\n",angx*180/PI,angy*180/PI,x2,y2);
    if(drag==1 || rotm==1)
    {
        mprevx=x;
        mprevy=y;
        if(drag)
            drag++;
        if(rotm)
            rotm++;
        return;
    }
    if(drag==2)
    {
        int dx=x-mprevx;
        int dy=y-mprevy;
        dx*=dist_look;
        dy*=dist_look;

        mprevx=x;
        mprevy=y;
        double dx1 = dx * sin(-theta) + dy * cos(-theta);
        double dy1 = dx * cos(-theta) - dy * sin(-theta);
        //printf("lol2%f %f\n",dx1,dy1);

        transx-=dx1/50.0;
        transy-=dy1/50.0;
    }
    if(rotm==2)
    {
        int dx=x-mprevx;
        int dy=y-mprevy;
        mprevx=x;
        mprevy=y;
        theta-=(double)dx/(double)w*5.0;
        phi+=(double)dy/(double)h*5.0;
        if(phi>PI/2-.001)
            phi=PI/2-.001;
        if(phi<0.01)
            phi=0.01;

        if(theta>PI)
            theta-=2*PI;
        if(theta<-PI)
            theta+=2*PI;


    }
    if (specialKey != GLUT_ACTIVE_SHIFT)
    {

        // setting the angle to be relative to the mouse
        // position inside the window
    }
}

void draw_checkpoints()
{
    glLineWidth(2.0);
    int i;
    Point pprev(0,0,0);
    for(i=0; i<checkp.size(); ++i)
    {
        Point p = checkp[i];
        if(i!=0)
        {
            glColor4d(.5,1,1,.75);

            glBegin(GL_LINE);
            glVertex3d(pprev.x,pprev.y,pprev.z);
            glVertex3d(p.x,p.y,p.z);
            glEnd();
        }

        glColor4d(0,1,1,1);
        glPushMatrix();
        glTranslated(p.x,p.y,p.z);
        glRotated(0,0,0,1);
        draw_checkpoint();
        glPopMatrix();
        pprev=p;

    }
    lineW;

}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    double timz=glutGet(GLUT_ELAPSED_TIME);
    if(timz-lastpoll > 1000)
    {
        printf("poll\n");
#ifndef NO_ROBOT
        Serial.Write("R#");
#endif
        lastpoll=timz;
    }
#ifndef NO_ROBOT
    while(Serial.IsDataAvailable ())
    {
        char c=Serial.ReadByte();
        if(c==0xA)
        	continue;
        rbuf[rb_cur++]=c;
        rb_cur%=RBUF_SIZE-2;
        if(c=='#')
        {
            rbuf[rb_cur++]='\0';
            rb_cur=0;
            printf("recv %s\n",rbuf);

            char c0=rbuf[0];
        //    printf("*** c0 = %x , c1 = %x ***\n",c0,rbuf[1]);
        //    printf("2*** c0 = %c , c1 = %c ***\n",c0,rbuf[1]);


            if(c0=='X')
            {
                long nx;
                sscanf(rbuf,"X%ld#",&nx);
                r_x=(double)nx/1000.0+L/2;
            }
            if(c0=='Y')
            {
                long ny;
                sscanf(rbuf,"Y%ld#",&ny);
                r_y=(double)ny/1000.0+W/2;
            }
            if(c0=='A')
            {
                long na;
                sscanf(rbuf,"A%ld#",&na);
                r_a=PI+(double)na/1000.0;
            }
        }
    }
#endif

    if(iter++%1000==0)
        printf("X%f Y%f A%f\n",r_x,r_y,r_a);

    double eye_x=transx+dist_look*cos(theta)*cos(phi);
    double eye_y=transy+dist_look*sin(theta)*cos(phi);
    double eye_z=dist_look*sin(phi);

    double lookAt_x=transx;
    double lookAt_y=transy;
    double lookAt_z=0.0;

    double up_x=0.0;
    double up_y=0.0;
    double up_z=1.0;
    glLoadIdentity() ;

    gluLookAt(eye_x, eye_y, eye_z, lookAt_x, lookAt_y, lookAt_z, up_x, up_y, up_z);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    draw_grid();

    glColor3d(1,0,0);
    glPushMatrix();
    glTranslated(r_x,r_y,0.0);
    // glRotated(60,1,0,0);
    glRotated(r_a/PI*180.0,0,0,1);
    //  glutWireTorus(0.2,0.8,slices,stacks);

    draw_robot();
    glPopMatrix();


    glColor4d(0,1,1,.333);
    glPushMatrix();
    glTranslated(cursor_x,cursor_y,cursor_z);
    glRotated(0,0,0,1);
    draw_checkpoint();
    glPopMatrix();
    draw_checkpoints();


    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'o':
        exit(0);
        break;

    case 'z':
        transx-=.5*cos(theta);
        transy-=.5*sin(theta);
        break;
    case 's':
        transx+=.5*cos(theta);
        transy+=.5*sin(theta);
        break;
    case 'd':
        transx+=.5*cos(theta+PI/2);
        transy+=.5*sin(theta+PI/2);
        break;
    case 'q':
        transx-=.5*cos(theta+PI/2);
        transy-=.5*sin(theta+PI/2);
        break;

    case '8':
        phi+=.1;
        break;

    case '2':
        phi-=.1;
        break;

    case '4':
        theta-=.1;
        break;

    case '6':
        theta+=.1;
        break;


    case '+':
        dist_look/=1.1;
        break;

    case '-':
        dist_look*=1.1;
        break;

    }
    if(phi>PI/2-.001)
        phi=PI/2-.001;
    if(phi<0.01)
        phi=0.01;

    if(theta>PI)
        theta-=2*PI;
    if(theta<-PI)
        theta+=2*PI;

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

void draw_checkpoint()
{
    glutSolidSphere(.075, 32,32);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WW,HH);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("RoboMORT v0.00001");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    lineW();
#ifndef NO_ROBOT
    Serial.Open() ;
    Serial.SetBaudRate( SerialPort::BAUD_115200 ) ;
#endif

    glClearColor(0,0,0,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMouseFunc(processMouse);
    glutPassiveMotionFunc(processMousePassiveMotion);
    glutMotionFunc(processMousePassiveMotion);

    glutMainLoop();

    return EXIT_SUCCESS;
}


