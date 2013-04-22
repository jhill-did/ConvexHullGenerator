#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <gl/glew.h>
#include <gl/glfw.h>
//#include <gl/gl.h> We don't necessarily need this
#include <gl/freeglut.h>
#include <stdio.h>
#include "ConvexHullGenerator.h"
#include "ColorOutput.h"

int main(int argc, char **argv);
void Init(int argc, char **argv);
void MainLoop();

ConvexHullGenerator hullgen = ConvexHullGenerator();

int main(int argc, char **argv)
{
    Init(argc, argv);
    MainLoop();
}

void Init(int argc, char **argv)
{

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL


    //Set up window
    if(glfwInit() != GL_TRUE)
        exit( EXIT_FAILURE );
    CO::printC("Successfully Initialized GLFW\n", CO::COLOR_GREEN);


    if(glfwOpenWindow(800,600,0,0,0,0,0,0,GLFW_WINDOW) != GL_TRUE)
        exit(1);
    glfwSetWindowTitle("LUL CONVERX HURRS");
    CO::printC("Successfully Initialized GLFW window.\n", CO::COLOR_GREEN);

    //Print off current GL version.
    int major, minor, revision;
    glfwGetGLVersion(&major,&minor,&revision);
    printf("Current GL version: %d.%d.%d\n", major,minor,revision);

    int width,height;
    glfwGetWindowSize(&width, &height);
    printf("Window size (%d,%d)\n", width, height);

    //Enable Stuff Here
    glfwEnable(GLFW_AUTO_POLL_EVENTS);
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_POINT_SMOOTH );
    glPointSize( 4.0f );

    //Set up ortho view
    glMatrixMode( GL_PROJECTION );
    gluOrtho2D(-1,1,-1,1);
    glMatrixMode( GL_MODELVIEW );

    //Init Glut too, just in case we need it or something.
    glutInit(&argc, argv);

    //INIT STUFF NOW
    hullgen.Init();

}

void MainLoop()
{
    while(true)
    {
        glClearColor(1, 1, 1, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glColor3f(0,0,0);
        glRasterPos2f(-0.95f,0.95f);
        glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)"Convex Hulls are polygons that encompass\nall points in a given set.");
        glRasterPos2f(0.25f,-0.80f);
        glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)"Click LMB to place points.\nPress Space to calculate Hull.");
        hullgen.Update();
        hullgen.Draw();

        glfwSwapBuffers();
    }
}


