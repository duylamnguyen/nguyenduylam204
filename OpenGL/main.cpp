//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
// Hop nhat

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
vec3 normals[5000];
GLuint program, model_loc, projection_loc, view_loc;

mat4 mv, dccanh, dcxe, dcxanh, dcdo, dcvang, dcmaynang, nanggo;
mat4  model, dccanh_xekhach, dcxekhach, dc_canh, dc_nhatru;

GLfloat ttxs, quaymaynang, nangthanhgo;

GLfloat l = -1.0, r = 1.0;
GLfloat bottom = -1.0, top = 1.0;
//GLfloat zNear = 1, zFar = 10;
GLfloat zNear = 1, zFar = 10.0;
GLfloat dcc[] = { 0,0 };
GLfloat dcc1[] = { 0,0 };
GLfloat   theta[] = { 0,0,0 };
GLfloat x[] = { 0,0,0 };

// Mã màu
vec4 black = color4(0.0, 0.0, 0.0, 1.0); // black
vec4 red = color4(1.0, 0.0, 0.0, 1.0); // red
vec4 yellow = color4(1.0, 1.0, 0.0, 1.0); // yellow
vec4 green = color4(0.0, 1.0, 0.0, 1.0); // green
vec4 blue = color4(0.0, 0.0, 1.0, 1.0); // blue
vec4 magenta = color4(1.0, 0.0, 1.0, 1.0); // magenta
vec4 white = color4(1.0, 1.0, 1.0, 1.0); // white
vec4 cyan = color4(0.0, 1.0, 1.0, 1.0); // cyan
vec4 orange = color4(1, 0.6, 0); // orange




/* Khởi tạo các tham số chiếu sáng - tô bóng*/
point4 light_position(0.0, 0.0, 1.0, 0.0);
color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);

color4 material_ambient(1.0, 1.0, 1.0, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1.0, 0.8, 0.0, 1.0);
float material_shininess = 100.0;

color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;

bool Dat_BatDen = false;

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
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);
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

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);


	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.8, 0.8, 0.8, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}

// Hàm để tô màu đối tượng
void to_mau(color4 color) {
	material_diffuse = color;  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
}
mat4 model_view, base_model;

void ve() {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

// Vẽ không gian 
// mặt dưới 
void matduoi() {
	material_diffuse = vec4(0.2, 0.5, 0.2, 0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(0.5, -2.05, 0.0) * Scale(20, 0.1, 20); //rotate X Y Z
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}


// Vẽ map
void map() {
	matduoi();

}

void ve2() {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxekhach * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

//vạch kẻ đường
void vachke(GLfloat a, GLfloat b, GLfloat c)
{
	mv = Translate(a, b, c) * Scale(0.3, 0.01, 1.0);
	material_diffuse = vec4(1, 1.5, 1.0, 0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void vach_dung() {
	material_diffuse = vec4(1.0, 1.0, 1.0, 0);// mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(2.1, -2.0, 1.5) * Scale(20.0, 0.01, 0.1);
	ve();
}
void vach_ke_duong() {
	for (double i = -3.5; i < 3; i++) {
		vachke(i + 0.5, -2.0, 2.5);
	}
	vach_dung();
}


float check1 = 1;





// ô tô buýt
void gam_xe1() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(1.5, -1.4, -1.9) * Scale(1.8, 0.5, 3.0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void banhxe(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	mv = Translate(a, b, c) * RotateX(d) * Scale(0.2, 0.6, 0.6);
	material_diffuse = vec4(0, 0, 0, 0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void den_xe(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	mv = Translate(a, b, c) * RotateZ(d) * Scale(0.15, 0.15, 0.1);
	material_diffuse = vec4(1, 0.6, 0, 0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanh_doc1() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(2.3, -0.15, -0.48) * Scale(0.15, 2.0, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void thanh_doc2() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(0.7, -0.15, -0.48) * Scale(0.15, 2.0, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void thanh_trai() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(0.65, -0.7, -2.0) * Scale(0.1, 1.0, 2.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void thanh_phai() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(2.35, -0.7, -2.4) * Scale(0.1, 1.0, 2.0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void thanh_sau() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(1.45, -0.15, -3.35) * Scale(1.7, 2.0, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void thanh_truoc1() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(1.45, -0.75, -0.55) * Scale(1.7, 0.8, 0.3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void thanh_truoc2() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(1.45, -0.34, -0.56) * RotateX(30) * Scale(1.65, 0.17, 0.27);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void maixe() {
	material_diffuse = vec4(1.0, 1.0, 0.0, 1.0);   // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(1.5, 0.85, -1.9) * Scale(1.8, 0.05, 3.0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void thanh_cua(GLfloat a, GLfloat b, GLfloat c)
{
	mv = Translate(a, b, c) * Scale(0.1, 1.0, 0.1);
	material_diffuse = vec4(0, 0, 0, 0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void cua_xe() {
	material_diffuse = vec4(1.0, 1.0, 0.5, 1.0);  // mau vat
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	mv = Translate(2.3, -0.15, -0.98) * Scale(0.15, 2.0, 0.88);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * dccanh * dcxe * mv);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void oto() {

	gam_xe1();
	for (int j = 0; j < 20; j++) {
		banhxe(2.5, -1.6, -1.2, (6 * j));
		banhxe(2.5, -1.6, -2.8, (6 * j));
		banhxe(0.5, -1.6, -1.2, (6 * j));
		banhxe(0.5, -1.6, -2.8, (6 * j));
		den_xe(0.9, -1.4, -0.4, (6 * j));
		den_xe(2.2, -1.4, -0.4, (6 * j));

		thanh_cua(2.35, 0.3, -1.5);
		thanh_cua(2.35, 0.3, -2.0);
		thanh_cua(2.35, 0.3, -2.5);
		thanh_cua(2.35, 0.3, -3.0);

		thanh_cua(0.7, 0.3, -1.5);
		thanh_cua(0.7, 0.3, -2.0);
		thanh_cua(0.7, 0.3, -2.5);
		thanh_cua(0.7, 0.3, -3.0);
	}
	thanh_doc1();
	thanh_doc2();
	thanh_trai();
	thanh_phai();
	thanh_truoc1();
	thanh_truoc2();
	thanh_sau();
	maixe();
	cua_xe();
	dccanh = Translate(2.3, -0.15, -0.54 + ttxs) * RotateY(dcc[0]) * Translate(-2.3, 0.15, 0.54 - ttxs);
	dcxe = Translate(0, 0, ttxs);
}

// ô tô

GLfloat cam_pos_x = 0;
GLfloat cam_pos_y = 0;
GLfloat cam_pos_z = 2;
/// Ve Hien thi
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	map();

	vach_ke_duong();

	oto();
	//oto2();

	//nen()


	model = RotateX(theta[1]) * RotateY(theta[2]) * RotateZ(theta[3]);
	// view
	point4 eye(0.0, 0.0, 6.0, 1.0);
	point4 at(0.0, 0.0, 0.0, 1.0);
	vec4 up(0.0, 1.0, 0.0, 1.0);
	mat4 mv = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, mv);
	// projection
	mat4 projection = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	glutSwapBuffers();


}

void spinCube() {

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case '1': zNear *= 1.1f; zFar *= 1.1f; break;
	case '!': zNear *= 0.9f; zFar *= 0.9f; break;

	case 'x':
		theta[1] += 5;
		if (theta[1] > 360) theta[1] -= 360;
		break;
	case 'X':
		theta[1] -= 5;
		if (theta[1] > 360) theta[1] -= 360;
		break;
	case 'y':
		theta[2] += 5;
		if (theta[2] > 360) theta[2] -= 360;
		break;
	case 'Y':
		theta[2] -= 5;
		if (theta[2] > 360) theta[2] -= 360;
		break;

	case 'c': // đóng mở cửa xe
		dcc[0] -= 5;
		if (dcc[0] - 5 <= -90) dcc[0] = -90;
		glutPostRedisplay();
		break;
	case 'C':
		dcc[0] += 5;
		if (dcc[0] + 5 >= 0) dcc[0] = 0;
		glutPostRedisplay();
		break;


	case '3': // xe đi
		ttxs += 0.05;
		if (ttxs >= 1.5) { ttxs = 1.5; }
		glutPostRedisplay();
		break;
	case '#':// xe về
		ttxs -= 0.05;
		if (ttxs <= 0) { ttxs = 0; }
		glutPostRedisplay();
		break;



	}
}




void SpecialFunc(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		cam_pos_x += 0.03f;

		//cam_moved = true;
		break;
	case GLUT_KEY_LEFT:
		cam_pos_x -= 0.03f;

		//cam_moved = true;
		break;
	case GLUT_KEY_UP:
		cam_pos_z -= 0.03f;

		//cam_moved = true;
		break;
	case GLUT_KEY_DOWN:
		cam_pos_z += 0.03f;

		break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	// main function: program starts here

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Ben xe Khach");

	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(spinCube);

	glutSpecialFunc(SpecialFunc);

	glutMainLoop();
	return 0;
}




