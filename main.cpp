#include <stdio.h> //create a file, this is a default library

//printf("Hello World\n"); //output
//scanf()  //input


#include <iostream> //system library

//cout<< "Hello World"<<endl;
//cin>> ...

#include <opencv2/core/core.hpp> //include all the basic data types from opencv
#include <opencv2/highgui/highgui.hpp>//allow you to load image from computer or display image onto the screen
#include <opencv2/imgproc/imgproc.hpp> //provide functions to process an image

using namespace cv; //tell the compiler most of the data types involved are from OpenCV
using namespace std; //C++ library


Mat canvas;
Point p1, p2, p3, p4;
vector<Point> pts;
int counter = 0; //record user's click times
int select = -1;
int red = 255, blue = 0, green = 0;
int lines = 0;
void deletePoints()
{
    for (int i = 0; i < pts.size(); i++)
    {
        circle(canvas, pts[i], 1, Scalar(255, 255, 255), 2);
    }
    
}
void drawSplines(Point p1, Point p2, Point p3, Point p4, int i)
{
    
    
    double d[4][4] = { { -1, 3, -3, 1 }, { 3, -6, 3, 0 }, { -3, 3, 0, 0 }, { 1, 0, 0, 0 } };
    Mat M = Mat(4, 4, CV_64FC1, d);
    double curve[4][2] = {
        {(double)p1.x,(double)p1.y},
        {(double)p2.x,(double)p2.y},
        {(double)p3.x,(double)p3.y},
        {(double)p4.x,(double)p4.y} };
    Mat C = Mat(4, 2, CV_64FC1, curve);
    Mat A = M * C;
    for (double t = 0.0; t <= 1.0; t += 0.001)
    {
        double x = (A.ptr<double>(0)[0] * (t*t*t)) + (A.ptr<double>(1)[0] * (t*t)) + (A.ptr<double>(2)[0] * t) + (A.ptr<double>(3)[0]);
        double y = (A.ptr<double>(0)[1] * (t*t*t)) + (A.ptr<double>(1)[1] * (t*t)) + (A.ptr<double>(2)[1] * t) + (A.ptr<double>(3)[1]);
        circle(canvas, Point(x, y), 1, Scalar(0, 0, 0), 1);
    }
    if (i == 0) {
        line(canvas, p1, p2, Scalar(0, 0, 255), 1);
        line(canvas, p3, p4, Scalar(0, 0, 255), 1);
    }
    else if (i == 1)
    {
        line(canvas, p1, p2, Scalar(255, 255, 255), 1);
        line(canvas, p3, p4, Scalar(255, 255, 255), 1);
    }
}
void drawControlPoints(int red, int green, int blue)
{
    for (int i = 0; i < pts.size(); i++)
    {
        circle(canvas, pts[i], 1, Scalar(blue, green, red), 2);
    }
}
void drawSpline()
{
    int spline = pts.size()/4;
    for (int i = 0; i < spline; i++)
    {
        int x = i * 4;
        drawSplines(pts[x + 0], pts[x + 1], pts[x + 2], pts[x + 3], lines);
    }
}

void myMouse(int event, int x, int y, int, void*)
{
    
    if (event == EVENT_LBUTTONDOWN)
    {
        
        for (int i = 0; i < pts.size(); i++)
        {
            
            if (abs(pts[i].x - x) + abs(pts[i].y - y) <= 10)
            {
                
                select = i;
            }
            
        }
        
        if (select < 0) {
            
            pts.push_back(Point(x, y));
        }
    }
    else if (event == EVENT_LBUTTONUP)
    {
        select = -1;
        
    }
    else if (event == EVENT_RBUTTONDOWN)
    {
        pts.pop_back();
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        if (select > -1)
        {
            pts[select].x = x;
            pts[select].y = y;
        }
    }
}

int main()
{
    canvas = Mat(500, 500, CV_8UC3);
    
    namedWindow("Bezier Curve"); 
    setMouseCallback("Bezier Curve", myMouse);
    while (1)
    {
        canvas = Scalar(255, 255, 255);
        drawControlPoints(red, green, blue);
        drawSpline();
        imshow("Bezier Curve", canvas);
        char c = waitKey(1);
        if (c == 32)
            break;
        else if (c == 's' || c == 'S')
        {
            imwrite("SplineImg.jpg", canvas);
        }
        else if (c == 'r' || c == 'R')
        {
            blue = 255;
            green = 255;
            lines = 1;
        }
       
    }
    
    return 0;
}
