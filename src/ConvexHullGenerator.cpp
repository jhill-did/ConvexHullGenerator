#include "ConvexHullGenerator.h"
#include <gl/gl.h>
#include <gl/glext.h>
#include <windows.h>
#include <gl/glfw.h>
#include <windows.h>
#include <deque>
#include <math.h>
#include <stdio.h>
#include <limits>
#include "ColorOutput.h"


ConvexHullGenerator::ConvexHullGenerator()
{
    //ctor
}

ConvexHullGenerator::~ConvexHullGenerator()
{
    //dtor
}

void ConvexHullGenerator::Init()
{
    currentMode = PLOTTING_MODE;
    mouseRecentlyClicked = false;
}

void ConvexHullGenerator::Update()
{
    switch(currentMode)
    {
        case PLOTTING_MODE:

                //Reset mouseRecentlyClicked if applicable.
                if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
                    mouseRecentlyClicked = false;

                //If we're holding space, CalculateHull(); and set mode to COMPLETE_MODE
                if(glfwGetKey(32) == GLFW_PRESS)
                {
                    currentMode = COMPLETE_MODE;
                    CalculateHull();
                }

                if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseRecentlyClicked == false)
                {
                        int xpos,ypos;
                        glfwGetMousePos(&xpos,&ypos);
                        mouseRecentlyClicked = true;
                        pt testpoint;

                        //Let's get our screen sizes to normalize our mouse position to the real world.
                        int windowWidth, windowHeight;
                        glfwGetWindowSize(&windowWidth,&windowHeight);

                        //Let's do that math now.
                        testpoint.x = xpos/(windowWidth/2.0f)-1.0f;
                        testpoint.y = 1.0-ypos/(windowHeight/2.0f);
                        printf("New point: (%f,%f)\n",testpoint.x,testpoint.y);
                        unusedPts.push_back(testpoint);
                }

            break;

        case COMPLETE_MODE:
                if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    ClearPts();
                    currentMode = PLOTTING_MODE;
                }

            break;
    }

}


void ConvexHullGenerator::CalculateHull()
{
    CO::printC("\nCalculating Points!\n", CO::COLOR_INFO);
    printf("We'll have to work with %d points.\n", unusedPts.size());
    if(unusedPts.size()<=2)
    {
        CO::printC("\nERROR: POINT COUNT TOO LOW! (SIZE<=2)\n", CO::COLOR_ERROR);
        ClearPts();
        return;
    }

    //Let's find top most point
    unsigned int highestIndex = 0;
    float highestY = std::numeric_limits<float>::min();
    for(unsigned int spot=0; spot<unusedPts.size(); spot++)
    {
        if(unusedPts.at(spot).y > highestY)
        {
            highestY = unusedPts.at(spot).y;
            highestIndex = spot;
        }
    }
    pt highestPoint = unusedPts.at(highestIndex);
    usedPts.push_back( highestPoint ); //Put the point into the Used deque so we don't waste time processing it again
    unusedPts.erase( unusedPts.begin() + highestIndex );    //Take the point out of the Unused deque

    pt first, second, target;
    //Let's find our second point now
    unsigned int secondPtIndex;
    float greatestAngle = std::numeric_limits<float>::min();
    for(unsigned int spot=0; spot<unusedPts.size(); spot++)
    {
        target = unusedPts.at(spot);
        pt testpoint; testpoint.x = highestPoint.x+1; testpoint.y = highestPoint.y;
        float angle = CalcAngle(testpoint,highestPoint,target);
        if(angle>greatestAngle)
        {
            greatestAngle = angle;
            secondPtIndex = spot;
        }
    }
    pt secondPoint = unusedPts.at(secondPtIndex);
    usedPts.push_back(secondPoint);     //Put our point into the Used deque
    unusedPts.erase( unusedPts.begin() + secondPtIndex );  //Take the point out of the Unused deque

    //Alright, let's find all the points after this. These wont be special cases and we can find them through algorithm
    first = highestPoint;
    second= secondPoint;
    int targetIndex;

    while(true)
    {
        greatestAngle = -100;
        targetIndex = -1;
        for(unsigned int spot=0; spot<unusedPts.size(); spot++)
        {
            target = unusedPts.at(spot);
            float currentAngle = CalcAngle(first, second, target);

            if(currentAngle > greatestAngle)
            {
                greatestAngle = currentAngle;
                targetIndex = spot;
            }
        }
        if(CalcAngle(first,second,highestPoint) > greatestAngle || targetIndex==-1)
        {
            printf("\nDidn't find an angle long enough, breaking loop.\n");
            usedPts.push_back(highestPoint);
            break;
        } //If at any point the angle back to our start is larger, we're done.
        pt goodTarget = unusedPts.at(targetIndex);
        first = second; //Let's set our first point to our old mid point
        second = goodTarget; //Let's set our mid point to the new point we just found, so we can go off and find a new third.
        usedPts.push_back(goodTarget); //Put our new point in used
        unusedPts.erase( unusedPts.begin() + targetIndex ); //remove it from unused
    }

    //Let's see what's left over
    if(unusedPts.size()>0)
        printf("We have %d points left unused, let me list them now:\n", unusedPts.size());
    for(unsigned int spot=0; spot < unusedPts.size(); spot++)
    {
        pt point = unusedPts.at(spot);
        printf("Pt index: %d at (%f,%f)\n", spot, point.x, point.y);
    }
    printf("\n");


}

float ConvexHullGenerator::CalcAngle(pt first, pt midpoint, pt target)
{
    //float distFromFirstToTarget = sqrtf( pow(first.x - target.x, 2) + pow(first.y - target.y, 2) );
    float a = sqrtf( pow(midpoint.x - first.x,  2) + pow(midpoint.y - first.y , 2) );
    float b = sqrtf( pow(midpoint.x - target.x, 2) + pow(midpoint.y - target.y, 2) );
    float c = sqrtf( pow(first.x    - target.x, 2) + pow(first.y    - target.y, 2) );
    return acosf ( (a*a + b*b - c*c ) / (2*a*b) ); //Use law of cosines to find our angle C.
}

void ConvexHullGenerator::ClearPts()
{
    unusedPts.clear();
    usedPts.clear();
    CO::printC("Convex Hull cleared!\n", CO::COLOR_INFO);
}

void ConvexHullGenerator::Draw()
{

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    //Draw Origin
    glColor3f(0,0,0);
    glLineStipple(4,0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin( GL_LINES );
        glVertex2f(-1,0);
        glVertex2f(1,0);
        glVertex2f(0,-1);
        glVertex2f(0,1);
    glEnd();
    glDisable( GL_LINE_STIPPLE );

    glColor3f(0.5f,0.5f,0.5f);
    glBegin( GL_POINTS );
        for(unsigned int x=0; x<unusedPts.size(); x++)
            {
                pt currentPoint = unusedPts.at(x);
                glVertex2f(currentPoint.x, currentPoint.y);
            }
    glColor3f(1,0,0);
        for(unsigned int x=0; x<usedPts.size(); x++)
            {
                pt currentPoint = usedPts.at(x);
                glVertex2f(currentPoint.x, currentPoint.y);
            }
    glEnd();

    glBegin( GL_LINE_STRIP );
        for(unsigned int x=0; x<usedPts.size(); x++)
            {
                pt currentPoint = usedPts.at(x);
                glVertex2f(currentPoint.x, currentPoint.y);
            }
    glEnd();

    glPopMatrix();

}

