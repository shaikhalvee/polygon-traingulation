#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>

using namespace std;

#define MAX 10000000
#define pi (2*acos(0.0))

// for graphical
struct Point
{
	double x,y,z;
};

// for quick hull simulation
struct point
{
    int x, y;
    char type;
};
typedef vector<point>vp;
vp points;
vp output;

void print(vp P);
vp rightPointsOf(point, point, vp);
int crossProduct(point, point, point);

void drawAxes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0, 0, 0);
        glVertex3f( 150,0,0);
        glVertex3f(-150,0,0);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0,-150,0);
        glVertex3f(0, 150,0);

        glVertex3f(0,0, 150);
        glVertex3f(0,0,-150);
    } glEnd();

}


void drawCircle(double radius,int segments)
{
    int i;
    struct Point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

// draw points in P as circle.
void drawPoints(vp P)
{
    for(int i=0;i<P.size();i++){
        glPushMatrix();
        {
            glTranslated(P[i].x, P[i].y, 0);
            drawCircle(0.5,30);
        }
        glPopMatrix();
    }
}

// draw polygon using vector<points> P
void drawPolygon(vp P)
{
    for(int i=1;i<P.size();i++){
        glBegin(GL_LINES);
        {
			//glColor3d((double)i/(double)P.size(), 0,0);
			glVertex3d(P[i-1].x,P[i-1].y,0);
			glVertex3d(P[i].x,P[i].y,0);
        }
        glEnd();
    }
}

// anticlockwise rotationed polygon is typed by their property
void setTypes(vp &P)
{
    point now,next,prev;
    for(int i=0;i<P.size();++i){
        now = P[i];
        next = P[i+1];
        prev =  P[ ( i-1 + P.size() ) % P.size() ];
        if(now.y > next.y && now.y > prev.y){       // start or split
            if( crossProduct(now, prev, next)>0 ){  // start
                P[i].type = 's';
            }
            else{                                   // split
                P[i].type = 'p';
            }
        }
        else if(now.y < next.y && now.y < prev.y){  // end or merge
            if(crossProduct(now, prev, next)>0){    // end
                P[i].type = 'e';
            }
            else{
                P[i].type = 'm';                    // merge
            }
        }
        else{
            P[i].type = 'r';                        // regular
        }
    }
}

void polygonTriangulationSimulation()
{
    FILE *fp = freopen("input.txt","r",stdin);
    point input;
    int size;
    cin >> size;
    for(int i = 0; i<size; i++){
        cin >> input.x >> input.y;
        points.push_back(input);
    }
    //points.push_back(points[0]);
    setTypes(points);
    sort(points.begin(), points.end(), [](const point& upPoint, const point& downPoint)
    {
        return upPoint.y == downPoint.y ? upPoint.x < downPoint.x : upPoint.y > downPoint.y ;
    });
    print(points);
}

void drawPicture()
{
    drawPoints(points);
    drawPolygon(points);
}

void clearAll()
{
    points.clear();
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,20,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
    drawPicture();
    clearAll();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    //clearAll();
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	polygonTriangulationSimulation();
	glutInit(&argc,argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Polygon Traingulation simulation");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)


	//polygonTriangulationSimulation();
	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

// quickHull  main codes
void print(vp P)
{
    cout<<endl;
    for(int i = 0; i<P.size(); i++){
        cout << P[i].x << " " << P[i].y << endl;
        cout << "type : " << P[i].type << endl;
    }
}

// for points a,b,c return (ac x ab)
int crossProduct(point a, point b, point c)
{
    int ac_x = c.x-a.x;
    int ac_y = c.y-a.y;
    int ab_x = b.x-a.x;
    int ab_y = b.y-a.y;
    return ac_x*ab_y - ac_y*ab_x;
}

vp rightPointsOf(point leftP, point rightP, vp P)
{
    vp retPoints;
    for(int i=0;i<P.size();i++){
        if(crossProduct(leftP,rightP,P[i])>0){
            retPoints.push_back(P[i]);
        }
    }
    return retPoints;
}

