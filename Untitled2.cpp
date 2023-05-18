#include <glut.h>
#include <math.h>/*
#include<iostream>*/
#define M_PI 3.14159265358979323846

using namespace std;

// Các thông số của bánh răng và bộ dẫn động
const int gearTeethCount = 20;
const float gearRadius = 15;
const float gear2Radius = 30;
const float gearThickness = 0.5;
const int driveGearTeethCount = 30;
const float driveGearRadius = 2.0;
const float driveGearThickness = 0.5;
const float driveGearRotationSpeed = 0.5;
const float driveGearInitialRotation = 0.0;
float driveGearRotation = driveGearInitialRotation;

// Biến lưu góc xoay của bánh răng 1
GLfloat gear1_angle = 0.0f;

// Biến lưu góc xoay của bánh răng 2
GLfloat gear2_angle = 0.0f;

// Biến lưu góc xoay ngóc nhìn ngang
GLfloat horizontal_angle = 0.0f;
// Biến lưu góc xoay ngóc nhìn ndoc
GLfloat vertical_angle = 0.0f;
// Biến tỷ lệ
float scale = 1.0;
// Biến lưu trữ hướng xoay của bánh răng
bool clockwise = true;
// Biến đếm số lần bấm phím V, W
bool v_key = false;
bool w_key = false;
// Biến màu
bool color = false;
// Hàm callback để xử lý sự kiện bấm phím
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'm':
	case 'M':
		// Thu nhỏ cảnh
		scale /= 1.1;
		break;
	case 'n':
	case 'N':
		// Phóng to cảnh
		scale *= 1.1;
		break;
	case 'i':
	case 'I':
	{
		clockwise = true;
		break;
	}
	case 'y':
	case 'Y':
	{
		clockwise = false;
		break;
	}
	case 'v':
	case 'V':
		// Tăng biến đếm số lần bấm phím V
		v_key = !v_key;
		break;
	case 'w':
	case 'W':
		w_key = !w_key;
	case 'a':
	case 'A':
		// Xoay ngóc nhìn ngang qua trái
		horizontal_angle += 5.0f;
		break;
	case 'd':
	case 'D':
		// Xoay ngóc nhìn ngang qua phải
		horizontal_angle -= 5.0f;
		break;
	case 'R':
	case 'r':
		color = !color;
		break;
	}
	glutPostRedisplay();
}
void drawGear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth, int num)
{
	glShadeModel(GL_FLAT);
	if (w_key)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (num == 1)
	{
		// Tính toán tọa độ của các điểm trên trục
		GLfloat axle_radius = inner_radius; // Bán kính của trục là 20% bán kính bên trong của bánh răng
		GLfloat axle_z = width * 2;			// Tọa độ z của trục nằm ở giữa bánh răng
		glColor3f(1.0f, 1.0f, 1.0f);		// Thiết lập màu trắng cho trục
		/*if (w_key)
			glBegin(GL_FILL);
		else
			glBegin(GL_LINES);*/
		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i <= 360; i += 10)
		{
			GLfloat angle = i * M_PI / 180.0f;
			GLfloat x = axle_radius * cos(angle);
			GLfloat y = axle_radius * sin(angle);
			glVertex3f(x, y, axle_z);
			glVertex3f(x, y, -axle_z);
		}
		glEnd();
	}
	if (color)
		if (num == 1)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 0.0f);
		}
	else
		glColor3f(1.0f, 1.0f, 1.0f);

	// Vẽ các răng của bánh răng
	GLfloat angle, da;
	GLfloat r1, r2, r3;
	GLint i;

	r1 = inner_radius;
	r2 = outer_radius - tooth_depth / 2.0f;
	r3 = outer_radius + tooth_depth / 2.0f;
	da = 2.0f * M_PI / teeth / 4.0f;
	angle = 2.0f * M_PI / teeth;

	glBegin(GL_QUAD_STRIP);

	for (i = 0; i <= teeth; i++)
	{
		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5f);
		glVertex3f(r2 * cos(angle), r2 * sin(angle), width * 0.5f);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5f);
		glVertex3f(r2 * cos(angle + 3 * da), r2 * sin(angle + 3 * da), width * 0.5f);
	}
	glEnd();

	glBegin(GL_QUADS);
	da = 2.0f * M_PI / teeth / 2.0f;
	for (i = 0; i < teeth; i++)
	{
		glVertex3f(r2 * cos(angle), r2 * sin(angle), width * 0.5f);
		glVertex3f(r3 * cos(angle + da), r3 * sin(angle + da), width * 0.5f);
		glVertex3f(r3 * cos(angle + 2 * da), r3 * sin(angle + 2 * da), width * 0.5f);
		glVertex3f(r2 * cos(angle + 3 * da), r2 * sin(angle + 3 * da), width * 0.5f);
	}
	glEnd();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, width);

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++)
	{
		glVertex3f(r2 * cos(angle), r2 * sin(angle), -width * 0.5f);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5f);
		glVertex3f(r2 * cos(angle + 3 * da), r2 * sin(angle + 3 * da), -width * 0.5f);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5f);
	}
	glEnd();

	glBegin(GL_QUADS);
	for (i = 0; i < teeth; i++)
	{
		glVertex3f(r2 * cos(angle + 3 * da), r2 * sin(angle + 3 * da), -width * 0.5f);
		glVertex3f(r3 * cos(angle + 2 * da), r3 * sin(angle + 2 * da), -width * 0.5f);
		glVertex3f(r3 * cos(angle + da), r3 * sin(angle + da), -width * 0.5f);
		glVertex3f(r2 * cos(angle), r2 * sin(angle), -width * 0.5f);
	}
	glEnd();

	glPopMatrix();

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < teeth; i++)
	{
		angle = i * 2.0f * M_PI / teeth;
		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5f);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5f);
		GLfloat u = r2 * cos(angle + 3 * da) - r1 * cos(angle);
		GLfloat v = r2 * sin(angle + 3 * da) - r1 * sin(angle);
		GLfloat len = sqrt(u * u + v * v);
		u /= len;
		v /= len;
		glNormal3f(v, -u, 0.0f);
		glVertex3f(r2 * cos(angle + 3 * da), r2 * sin(angle + 3 * da), width * 0.5f);
		glVertex3f(r2 * cos(angle + 3 * da), r2 * sin(angle + 3 * da), -width * 0.5f);
		glNormal3f(cos(angle), sin(angle), 0.0f);
		glVertex3f(r2 * cos(angle), r2 * sin(angle), width * 0.5f);
		glVertex3f(r2 * cos(angle), r2 * sin(angle), -width * 0.5f);
		GLfloat u1 = r3 * cos(angle + 2 * da) - r2 * cos(angle + 3 * da);
		GLfloat v1 = r3 * sin(angle + 2 * da) - r2 * sin(angle + 3 * da);
		glNormal3f(v1, -u1, 0.0f);
		glVertex3f(r3 * cos(angle + 2 * da), r3 * sin(angle + 2 * da), width * 0.5f);
		glVertex3f(r3 * cos(angle + 2 * da), r3 * sin(angle + 2 * da), -width * 0.5f);
		glNormal3f(cos(angle), sin(angle), 0.0f);
		glVertex3f(r3 * cos(angle), r3 * sin(angle), width * 0.5f);
		glVertex3f(r3 * cos(angle), r3 * sin(angle), -width * 0.5f);
	}

	glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5f);
	glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5f);

	glEnd();

	glShadeModel(GL_SMOOTH);
}
// Hàm cập nhật vị trí và góc xoay của bộ dẫn động
void updateDriveGear()
{
	const float pi = 3.14159265358979323846;
	const float toothAngle = 2.0 * pi / driveGearTeethCount;
	const float driveGearAngularVelocity = driveGearRotationSpeed / driveGearRadius;
	const float driveGearDeltaRotation = driveGearAngularVelocity * toothAngle;

	driveGearRotation += driveGearDeltaRotation;
	if (driveGearRotation >= 360.0)
	{
		driveGearRotation -= 360.0;
	}
}
// Hàm vẽ cảnh
void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (v_key)
	{
		gluPerspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
		glTranslatef(0.0f, -10.0f, 0.0f);
	}
	else
	{
		// Chế độ 3D
		gluPerspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
		glTranslatef(0.0f, 0.0f, -10.0f);
	}
	glRotatef(horizontal_angle, 0.0f, 1.0f, 0.0f); // Xoay ngóc nhìn ngang
	// Di chuyển và xoay camera
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);

	// Thay đổi tỷ lệ phóng to / thu nhỏ
	glScalef(scale, scale, scale);

	// Tạo hiệu ứng xoay bánh răng
	if (clockwise)
	{
		gear1_angle += 1.0f;
		gear2_angle -= 2.0f;
	}
	else
	{
		gear1_angle -= 1.0f;
		gear2_angle += 2.0f;
	}

	glPushMatrix();
	glTranslatef(-2.0f, 0.0f, 0.0f);
	glRotatef(gear1_angle, 0, 0.0f, 1.0f);
	drawGear(0.5f, 2.0f, 1.0f, gearRadius, 0.3f, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0f, 0.0f, 0.0f);
	glRotatef(gear2_angle, 0, 0, 1.0f);
	// drawGear(1.0f, 3.0f, 0.3f, 20, 0.3f);
	//  size r, size w, speed,
	drawGear(0.7f, 2.0f, 2.0f, gear2Radius, 0.5f, 0);
	glPopMatrix();

	glutSwapBuffers();
}
// Hàm cập nhật cảnh
void updateScene(int value)
{
	updateDriveGear();
	gear2_angle += gear1_angle * (gearRadius / gear2Radius); // Cập nhật góc quay của bánh răng 2
	glutPostRedisplay();
	glutTimerFunc(16, updateScene, 0);
}

int main(int argc, char **argv)
{
	/*cout << "M/N de tien gan va ra xa" << endl;
	cout << "I/Y de xoay theo/nguoc chieu kim dong ho" << endl;
	cout << "W de ve 3d" << endl;
	cout << "R de to mau" << endl;
	cout << "M/N de tien gan va ra xa" << endl;
	cout << "A/D de xoay cam sang trai/phai" << endl;*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Gear and Drive Train");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutDisplayFunc(drawScene);
	glutTimerFunc(16, updateScene, 10);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}