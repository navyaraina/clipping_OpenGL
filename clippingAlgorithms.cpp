#include <cstdlib>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define NUMBERPRIMITIVES 3

static int width, height;
static float pointSize = 3.0; 
static int primitive = INACTIVE;
static int pointCount = 0;
static int rectCount = 0;
static int tempX, tempY;
static bool Clip = false;
float xMax, xMin, yMax, yMin;

class Point
{
public:
	float x, y;
	bool isClippedLiangBarsky;
	bool isClippedCohenSutherland;
	float size;
	Point(float xVal, float yVal, float sizeVal)
	{
		x = xVal; y = yVal; size = sizeVal;
		isClippedLiangBarsky = false;
		isClippedCohenSutherland = false;
	}
	Point() {};
	void drawPoint(void);
};

void Point::drawPoint()
{
	if (isClippedLiangBarsky == true)
		glColor3f(0.0, 0.7, 0.7);
	else if (isClippedCohenSutherland == true)
		glColor3f(0.7, 0.0, 0.7);
	else
		glColor3f(0.0, 0.0, 0.0);
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0.0);
	glEnd();
}

std::vector<Point> points;

void drawPoints(void)
{
	for (auto point : points) { point.drawPoint(); }
}

class Line
{
public:
	float x1, y1, x2, y2;
	bool isClippedLiangBarsky;
	bool isClippedCohenSutherland;
	Line(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
		isClippedLiangBarsky = false;
		isClippedCohenSutherland = false;
	}
	Line() {};
	void drawLine();
};


void Line::drawLine()
{
	if (isClippedLiangBarsky == true)
		glColor3f(0.0, 0.7, 0.7);
	else if (isClippedCohenSutherland == true)
		glColor3f(0.7, 0.0, 0.7);
	else
		glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

std::vector<Line> lines;

void drawLines(void)
{
	for (auto line : lines) { line.drawLine(); }
}

class Rect
{
public:
	float x1, y1, x2, y2;
	Rect(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	}
	Rect() {};
	void drawRectangle();

};

void Rect::drawRectangle()
{
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(x1, y1, x2, y2);
}

std::vector<Rect> rectangles;

void drawRectangles(void)
{
	for (auto rectangle : rectangles) { rectangle.drawRectangle(); }
}

void drawPointSelectionBox(void)
{
	if (primitive == POINT) glColor3f(1.0, 1.0, 1.0);
	else glColor3f(0.8, 0.8, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.9 * height, 0.1 * width, height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.9 * height, 0.1 * width, height);

	glPointSize(pointSize);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(0.05 * width, 0.95 * height, 0.0);
	glEnd();
}

void drawLineSelectionBox(void)
{
	if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); 
	else glColor3f(0.8, 0.8, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.025 * width, 0.825 * height, 0.0);
	glVertex3f(0.075 * width, 0.875 * height, 0.0);
	glEnd();
}

void drawRectangleSelectionBox(void)
{
	if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0);
	else glColor3f(0.8, 0.8, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.025 * width, 0.735 * height, 0.075 * width, 0.765 * height);
	glEnd();
}

void drawInactiveArea(void)
{
	glColor3f(0.6, 0.6, 0.6);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBERPRIMITIVES * 0.1) * height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBERPRIMITIVES * 0.1) * height);
}

void drawTempPoint(void)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glVertex3f(tempX, tempY, 0.0);
	glEnd();
}


void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	drawPointSelectionBox();
	drawLineSelectionBox();
	drawRectangleSelectionBox();
	drawInactiveArea();

	drawPoints();
	drawLines();
	drawRectangles();

	if (((primitive == LINE) || (primitive == RECTANGLE)) &&
		(pointCount == 1)) drawTempPoint();

	glutSwapBuffers();
}

void pickPrimitive(int y)
{
	if (y < (1 - NUMBERPRIMITIVES * 0.1) * height) primitive = INACTIVE;
	else if (y < (1 - 2 * 0.1) * height) primitive = RECTANGLE;
	else if (y < (1 - 1 * 0.1) * height) primitive = LINE;
	else primitive = POINT;
}

void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y = height - y; 

		if (x < 0 || x > width || y < 0 || y > height);

		else if (x < 0.1 * width)
		{
			pickPrimitive(y);
			pointCount = 0;
		}

		else
		{
			if (primitive == POINT) points.push_back(Point(x, y, pointSize)); 
			else if (primitive == LINE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					lines.push_back(Line(tempX, tempY, x, y)); 
					pointCount = 0;
				}
			}
			else if (primitive == RECTANGLE)
			{
				if (rectCount == 0) 
				{
					if (pointCount == 0)
					{
						tempX = x; tempY = y;
						pointCount++;
					}
					else
					{
						rectangles.push_back(Rect(tempX, tempY, x, y));
						pointCount = 0;
						rectCount++;
					}
				}
			}
		}
	}
	glutPostRedisplay();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void clearAll(void)
{
	points.clear();
	lines.clear();
	rectangles.clear();
	primitive = INACTIVE;
	pointCount = 0;
	rectCount = 0;
	xMax = 0;xMin = 0;yMax = 0;yMin = 0;
}


const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

int computeCode(float x, float y)
{
	int code = INSIDE; 
	if (x < xMin)
	{
		code = code | LEFT;
	}
	else if (x > xMax) 
	{
		code = code | RIGHT;
	}
	if (y < yMin) 
	{
		code = code | BOTTOM;
	}
	else if (y > yMax) 
	{
		code = code | TOP;
	}
	return code;
}

void clipLineCohenSutherland(void)
{
	//Calculate the extremes of the clipping window
	for (auto rectangle : rectangles)
	{
		xMax = (rectangle.x1 > rectangle.x2) ? rectangle.x1 : rectangle.x2;
		xMin = (rectangle.x1 < rectangle.x2) ? rectangle.x1 : rectangle.x2;
		yMax = (rectangle.y1 > rectangle.y2) ? rectangle.y1 : rectangle.y2;
		yMin = (rectangle.y1 < rectangle.y2) ? rectangle.y1 : rectangle.y2;
	}
	for (auto& line : lines)
	{
		int startCode = computeCode(line.x1, line.y1);
		int endCode = computeCode(line.x2, line.y2);
		bool accept = false;
		while (true)
		{
			if (!(startCode | endCode))
			{ // Both endpoints inside window.
				accept = true;
				break;
			}
			else if (startCode & endCode)
			{ // Both endpoints outside same edge.
				break;
			}
			else
			{
				float x, y;
				int outsideCode = startCode ? startCode : endCode;

				// Calculate intersection point
				if (outsideCode & TOP) {
					x = line.x1 + (line.x2 - line.x1) * (yMax - line.y1) / (line.y2 - line.y1);
					y = yMax;
				}
				else if (outsideCode & BOTTOM) {
					x = line.x1 + (line.x2 - line.x1) * (yMin - line.y1) / (line.y2 - line.y1);
					y = yMin;
				}
				else if (outsideCode & RIGHT) {
					y = line.y1 + (line.y2 - line.y1) * (xMax - line.x1) / (line.x2 - line.x1);
					x = xMax;
				}
				else if (outsideCode & LEFT) {
					y = line.y1 + (line.y2 - line.y1) * (xMin - line.x1) / (line.x2 - line.x1);
					x = xMin;
				}

				// Update the point and region code
				if (outsideCode == startCode)
				{
					line.x1 = x;
					line.y1 = y;
					startCode = computeCode(line.x1, line.y1);
				}
				else
				{
					line.x2 = x;
					line.y2 = y;
					endCode = computeCode(line.x2, line.y2);
				}
			}
		}
		if (!accept)
			line.isClippedCohenSutherland = false;
		else
			line.isClippedCohenSutherland = true;
	}

	for (auto& point : points)
	{
		int Pointcode = computeCode(point.x, point.y);

		//If code is non-zero, its outside the clipping window
		if (Pointcode != 0)
			point.isClippedCohenSutherland = false;
		else
			point.isClippedCohenSutherland = true;
	}

	auto itL = lines.begin();
	while (itL != lines.end()) {
		if (!itL->isClippedCohenSutherland) {
			itL = lines.erase(itL); 
		}
		else {
			++itL;
		}
	}

	auto itP = points.begin();
	while (itP != points.end()) {
		if (!itP->isClippedCohenSutherland) {
			itP = points.erase(itP); 
		}
		else {
			++itP;
		}
	}
}

void clipLineLiangBarsky() {
	for (auto rectangle : rectangles)
	{
		xMax = (rectangle.x1 > rectangle.x2) ? rectangle.x1 : rectangle.x2;
		xMin = (rectangle.x1 < rectangle.x2) ? rectangle.x1 : rectangle.x2;
		yMax = (rectangle.y1 > rectangle.y2) ? rectangle.y1 : rectangle.y2;
		yMin = (rectangle.y1 < rectangle.y2) ? rectangle.y1 : rectangle.y2;
	}
	for (auto& line : lines) {
		float dx = line.x2 - line.x1;
		float dy = line.y2 - line.y1;

		float p[4] = { -dx, dx, -dy, dy }; // p1 = -(x2 - x1), p2 = x2 - x1, p3 = -(y2 - y1), p4 = y2 - y1
		float q[4] = { line.x1 - xMin, xMax - line.x1, line.y1 - yMin, yMax - line.y1 }; // q1 = x1 - xMin, q2 = xMax - x1, q3 = y1 - yMin, q4 = yMax - y1

		float u1 = 0.0, u2 = 1.0;

		bool accept = true;

		for (int i = 0; i < 4; i++) {
			if (p[i] == 0) {
				// Line is parallel to the corresponding clipping boundary
				if (q[i] < 0) {
					accept = false; // Line is outside
					break;
				}
			}
			else {
				float r = q[i] / p[i];
				if (p[i] < 0) { // Line entering the boundary
					u1 = std::max(u1, r);
				}
				else { // Line leaving the boundary
					u2 = std::min(u2, r);
				}
			}
		}

		if (accept && u1 <= u2) {
			// Calculate the clipped line coordinates
			float newX1 = line.x1 + u1 * dx;
			float newY1 = line.y1 + u1 * dy;
			float newX2 = line.x1 + u2 * dx;
			float newY2 = line.y1 + u2 * dy;

			// Update the line with clipped coordinates
			line.x1 = newX1;
			line.y1 = newY1;
			line.x2 = newX2;
			line.y2 = newY2;

			line.isClippedLiangBarsky = true;
		}
		else {
			line.isClippedLiangBarsky = false; // Line is outside the clipping window
		}
	}

	auto itL = lines.begin();
	while (itL != lines.end()) {
		if (!itL->isClippedLiangBarsky) {
			itL = lines.erase(itL); 
		}
		else {
			++itL; 
		}
	}
	for (auto& point : points)
	{
		int Pointcode = computeCode(point.x, point.y);

		if (Pointcode != 0)
			point.isClippedLiangBarsky = false;
		else
			point.isClippedLiangBarsky = true;
	}
	auto itP = points.begin();
	while (itP != points.end()) {
		if (!itP->isClippedLiangBarsky) {
			itP = points.erase(itP);
		}
		else {
			++itP;
		}
	}
}

void rightMenu(int id)
{
	if (id == 1)
	{
		Clip = true;
		clipLineCohenSutherland();
		glutPostRedisplay();
	}
	if (id == 2) {
		Clip = true;
		clipLineLiangBarsky();
		glutPostRedisplay();
	}
	if (id == 3)
	{
		clearAll();
		glutPostRedisplay();
	}
}

void makeMenu(void)
{
	glutCreateMenu(rightMenu);
	glutAddMenuEntry("Clip-CohenSutherland", 1);
	glutAddMenuEntry("Clip-LiangBarsky", 2);
	glutAddMenuEntry("Clear", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	makeMenu(); 
}

void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Left click on a box on the left to select a primitive." << std::endl
		<< "Then left click on the drawing area: once for point, twice for line or rectangle." << std::endl
		<< "Draw a rectangle to define the clipping window." << std::endl
		<< "Right click for menu options." << std::endl
		<< "Click on Clip in the menu to Clip the image" << std::endl;
}

int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("cohen sutherland");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}