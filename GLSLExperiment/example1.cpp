//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/


// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);


typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program, model_loc, view_loc, projection_loc;
mat4 model_nam, model_robot_nam, model_view;
mat4 model_robot_nam_1, model_robot_nam_2, model_robot_nam_head, model_robot_nam_leg_1, model_robot_nam_leg_2;

mat4 model_sang, model_robot_sang;
mat4 model_robot_sang_1, model_robot_sang_2, model_robot_head_sang, model_robot_leg_sang_1, model_robot_leg_sang_2, model_tatca;

GLfloat xx = 0, yy = 0, zz = 0,
a = 0, b = 0, c = 0, A = 0, B = 0, C = 0,
q = 0, w = 0, e = 0;

GLfloat l = 1.0, r = -1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 1.5, zFar = 5.0;

GLfloat xxx = 0, yyy = 0, zzz = 0,
aa = 180, bb = 0, cc = 0, AA = 180, BB = 0, CC = 0,
qq = 0, ww = 0, ee = 0, p = 0;


void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry(void)
{
	initCube();
	makeColorCube();
}


void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

}


void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));


	model_loc = glGetUniformLocation(program, "model");
	view_loc = glGetUniformLocation(program, "view");
	projection_loc = glGetUniformLocation(program, "projection");

	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}

mat4 instace;

GLfloat BASE_x = 0.1, BASE_y = 0.1, BASE_z = 0.05,
UPPER_ARM_x = 0.05, UPPER_ARM_y = 0.2, UPPER_ARM_z = 0.05,
LOWER_ARM_x = 0.03, LOWER_ARM_y = 0.15, LOWER_ARM_z = 0.05,
Tran_x = 0.15 + 0.05;

void body_nam() {
	//body
	instace = Scale(0.3, 0.3, -0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}


void base_nam_1()
{
	instace = Translate(Tran_x, 0.5 * BASE_y, 0.0)
		* Scale(BASE_x, BASE_y, BASE_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_1 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void upper_arm_nam_1()
{
	instace = Translate(Tran_x, 0.5 * UPPER_ARM_y, 0.0)
		* Scale(UPPER_ARM_x, UPPER_ARM_y, UPPER_ARM_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_1 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lower_arm_nam_1()
{
	instace = Translate(Tran_x, 0.5 * LOWER_ARM_y, 0.0)
		* Scale(LOWER_ARM_x, LOWER_ARM_y, LOWER_ARM_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_1 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void arm_nam_1() {

	//right_arm

	model_robot_nam_1 = model_robot_nam * Translate(0.0, 0.0, 0.0) * RotateX(a);
	base_nam_1();

	model_robot_nam_1 = model_robot_nam_1 * Translate(0.0, BASE_y, 0.0) * RotateX(b);
	upper_arm_nam_1();

	model_robot_nam_1 = model_robot_nam_1 * Translate(0.0, UPPER_ARM_y, 0.0) * RotateX(c);
	lower_arm_nam_1();

}


void base_nam_2()
{
	instace = Translate(Tran_x, 0.5 * BASE_y, 0.0)
		* Scale(BASE_x, BASE_y, BASE_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_2 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void upper_arm_nam_2()
{
	instace = Translate(Tran_x, 0.5 * UPPER_ARM_y, 0.0)
		* Scale(UPPER_ARM_x, UPPER_ARM_y, UPPER_ARM_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_2 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lower_arm_nam_2()
{
	instace = Translate(Tran_x, 0.5 * LOWER_ARM_y, 0.0)
		* Scale(LOWER_ARM_x, LOWER_ARM_y, LOWER_ARM_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_2 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void arm_nam_2() {

	//model_robot_2 = model_robot * Translate(0, -BASE_y, 0) * Translate(0.0, UPPER_ARM_y, 0.0);

	//left_arm

	model_robot_nam_2 = model_robot_nam * Translate(0.0, 0.0, 0.0) * RotateX(A) * Translate(-Tran_x, 0, 0) * Translate(-Tran_x, 0, 0);
	base_nam_2();

	model_robot_nam_2 = model_robot_nam_2 * Translate(0.0, BASE_y, 0.0) * RotateX(B);
	upper_arm_nam_2();

	model_robot_nam_2 = model_robot_nam_2 * Translate(0.0, UPPER_ARM_y, 0.0) * RotateX(C);
	lower_arm_nam_2();
}

void head_nam() {
	//head
	model_robot_nam_head = model_robot_nam * Translate(0, 0, 0) * RotateY(q);

	instace = Translate(0, 0.2, 0) * Scale(0.15, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_head * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//righteye
	instace = Translate(-0.05, 0.2, -0.05) * Scale(0.01, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_head * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//lefteye
	instace = Translate(0.05, 0.2, -0.05) * Scale(0.01, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_head * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void leg_nam_1() {
	model_robot_nam_leg_1 = model_robot_nam * Translate(0, 0, 0) * RotateX(w);
	//rightleg
	instace = Translate(-0.1, -0.25, 0) * Scale(0.1, 0.25, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_leg_1 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void leg_nam_2() {

	model_robot_nam_leg_2 = model_robot_nam * Translate(0, 0, 0) * RotateX(e);

	//leftleg

	instace = Translate(0.1, -0.25, 0) * Scale(0.1, 0.25, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_nam * model_robot_nam_leg_2 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void arm_nam() {
	arm_nam_1();
	arm_nam_2();
}

void leg_nam() {
	leg_nam_1();
	leg_nam_2();
}

GLfloat ya = 0;
void robot_nam() {
	model_robot_nam = Translate(0.5, 0, 0) * Translate(xx, yy, 0) * RotateY(ya);

	body_nam();

	head_nam();

	arm_nam();

	leg_nam();
}



void body_sang() {
	//body
	instace = Translate(0, 0.1, -0.1) * Scale(0.3, 0.04, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_sang * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instace = Scale(0.3, 0.3, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_sang * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

//
//GLfloat BASE_x = 0.1, BASE_y = 0.1, BASE_z = 0.05,
//UPPER_ARM_x = 0.05, UPPER_ARM_y = 0.2, UPPER_ARM_z = 0.05,
//LOWER_ARM_x = 0.03, LOWER_ARM_y = 0.15, LOWER_ARM_z = 0.05,
//Tran_x = 0.15 + 0.05;

void base_sang_1()
{
	instace = Translate(Tran_x, 0.5 * BASE_y, 0.0)
		* Scale(BASE_x, BASE_y, BASE_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_sang_1 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void upper_arm_sang_1()
{
	instace = Translate(Tran_x, 0.5 * UPPER_ARM_y, 0.0)
		* Scale(UPPER_ARM_x, UPPER_ARM_y, UPPER_ARM_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_sang_1 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lower_arm_sang_1()
{
	instace = Translate(Tran_x, 0.5 * LOWER_ARM_y, 0.0)
		* Scale(LOWER_ARM_x, LOWER_ARM_y, LOWER_ARM_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_sang_1 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void arm_sang_1() {

	//right_arm

	model_robot_sang_1 = model_robot_sang * Translate(0.0, 0.1, 0.0) * RotateX(aa);
	base_sang_1();

	model_robot_sang_1 = model_robot_sang_1 * Translate(0.0, BASE_y, 0.0) * RotateX(bb);
	upper_arm_sang_1();

	model_robot_sang_1 = model_robot_sang_1 * Translate(0.0, UPPER_ARM_y, 0.0) * RotateX(cc);
	lower_arm_sang_1();

}


void base_sang_2()
{
	instace = Translate(Tran_x, 0.5 * BASE_y, 0.0)
		* Scale(BASE_x, BASE_y, BASE_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_sang_2 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void upper_arm_sang_2()
{
	instace = Translate(Tran_x, 0.5 * UPPER_ARM_y, 0.0)
		* Scale(UPPER_ARM_x, UPPER_ARM_y, UPPER_ARM_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_sang_2 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lower_arm_sang_2()
{
	instace = Translate(Tran_x, 0.5 * LOWER_ARM_y, 0.0)
		* Scale(LOWER_ARM_x, LOWER_ARM_y, LOWER_ARM_z);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_sang_2 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void arm_sang_2() {

	model_robot_sang_2 = model_robot_sang * Translate(0.0, 0.1, 0.0) * RotateX(AA) * Translate(-Tran_x, 0, 0) * Translate(-Tran_x, 0, 0);
	base_sang_2();

	model_robot_sang_2 = model_robot_sang_2 * Translate(0.0, BASE_y, 0.0) * RotateX(BB);
	upper_arm_sang_2();

	model_robot_sang_2 = model_robot_sang_2 * Translate(0.0, UPPER_ARM_y, 0.0) * RotateX(CC);
	lower_arm_sang_2();
}

void head_sang() {
	//head
	model_robot_head_sang = model_robot_sang * Translate(0, 0, 0) * RotateY(qq);


	instace = Translate(0, 0.3, 0) * Scale(0.01, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_head_sang * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instace = Translate(0, 0.2, 0) * Scale(0.15, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_head_sang * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//righteye
	instace = Translate(-0.05, 0.2, -0.05) * Scale(0.01, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_head_sang * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//lefteye
	instace = Translate(0.05, 0.2, -0.05) * Scale(0.01, 0.01, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_head_sang * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void leg_sang_1() {
	model_robot_leg_sang_1 = model_robot_sang * Translate(0, 0, 0) * RotateX(ww);
	//rightleg
	instace = Translate(-0.1, -0.25, 0) * Scale(0.1, 0.25, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_leg_sang_1 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void leg_sang_2() {

	model_robot_leg_sang_2 = model_robot_sang * Translate(0, 0, 0) * RotateX(ee);

	//leftleg

	instace = Translate(0.1, -0.25, 0) * Scale(0.1, 0.25, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * model_robot_leg_sang_2 * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void arm_sang() {
	arm_sang_1();
	arm_sang_2();
}

void leg_sang() {
	leg_sang_1();
	leg_sang_2();
}

void robot_sang() {
	model_robot_sang = Translate(-0.5, 0, 0) * Translate(0, yyy, p) * RotateY(ya);

	body_sang();

	head_sang();

	arm_sang();

	leg_sang();
}
mat4 modelchieu;
void chieu() {
	instace = Translate(0, -0.4, 0) * Scale(3, 0.01, 3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_sang * modelchieu * instace);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tatca() {
	chieu();
	robot_nam();
	robot_sang();
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	vec4 eye = vec4(0, 0.5, -2, 0);
	vec4 at = vec4(0, 0, 0, 0);
	vec4 up = vec4(0, 2, 0, 0);


	mat4 v = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, v);

	mat4 p = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);

	tatca();
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	case 's':	//body
		ya += 30;
		glutPostRedisplay();
		break;
	case 'q':	//right_base_nam
		a -= 5;
		if (a > 360) a -= 360;
		glutPostRedisplay();
		break;
	case 'w':	//right_upper_arm_nam
		b -= 5;
		if (b > 360) b -= 360;
		glutPostRedisplay();
		break;
	case 'e':	//right_lower_arm_nam
		c -= 5;
		if (c > 360) c -= 360;
		glutPostRedisplay();
		break;
	case 'Q':
		A -= 5;
		if (A > 360) A -= 360;
		glutPostRedisplay();
		break;
	case 'W':
		B -= 5;
		if (B > 360) B -= 360;
		glutPostRedisplay();
		break;
	case 'E':
		C -= 5;
		if (C > 360) C -= 360;
		glutPostRedisplay();
		break;
	case 'a':	//head_nam
		q -= 30;
		if (q > 360) q -= 360;
		glutPostRedisplay();
		break;
	case 'r':	//left_leg_nam
		w += 30;
		if (w > 60) w -= 30;
		glutPostRedisplay();
		break;
	case 'R':
		w -= 30;
		if (w < -60) w += 30;
		glutPostRedisplay();
		break;
	case 't':	//right_leg_nam
		e += 30;
		if (e > 60) e -= 30;
		glutPostRedisplay();
		break;
	case 'T':
		e -= 30;
		if (e < -60) e += 30;
		glutPostRedisplay();
		break;


		//SANG
	case 'y':	//left_base
		aa -= 5;
		if (aa > 360) aa -= 360;
		glutPostRedisplay();
		break;
	case 'u':	//right_upper_arm
		bb -= 5;
		if (bb > 360) bb -= 360;
		glutPostRedisplay();
		break;
	case 'i':	//right_lower_arm
		cc -= 5;
		if (cc > 360) cc -= 360;
		glutPostRedisplay();
		break;
	case 'Y':
		AA += 5;
		if (AA > 360) AA -= 360;
		glutPostRedisplay();
		break;
	case 'U':
		BB -= 5;
		if (BB > 360) BB -= 360;
		glutPostRedisplay();
		break;
	case 'I':
		CC -= 5;
		if (CC > 360) CC -= 360;
		glutPostRedisplay();
		break;
	case 'h':	//head
		qq -= 30;
		if (qq > 360) qq -= 360;
		glutPostRedisplay();
		break;
	case 'o':	//left_leg
		ww += 30;
		if (ww > 60) ww -= 30;
		glutPostRedisplay();
		break;
	case 'O':
		ww -= 30;
		if (ww < -60) ww += 30;
		glutPostRedisplay();
		break;
	case 'p':	//right_leg
		ee += 30;
		if (ee > 60) ee -= 30;
		glutPostRedisplay();
		break;
	case 'P':
		ee -= 30;
		if (ee < -60) ee += 30;
		glutPostRedisplay();
		break;
	case 'l':
		p += 0.05;
		if (p > 0.4) p = 0.4;
		glutPostRedisplay();
		ww += 10;
		if (ww > 80) ww -= 120;
		glutPostRedisplay();
		ee -= 10;
		if (ee < -80) ee += 120;
		glutPostRedisplay();
		break;

	case 'L':
		p -= 0.05;
		if (p < -0.4) p = -0.4;
		glutPostRedisplay();
		ww -= 10;
		if (ww < -80) ww += 120;
		glutPostRedisplay();
		ee += 10;
		if (ee > 80) ee -= 120;
		glutPostRedisplay();
		break;
	}

}


int main(int argc, char** argv)
{
	// main function: program starts here

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Drawing a Cube");


	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}
