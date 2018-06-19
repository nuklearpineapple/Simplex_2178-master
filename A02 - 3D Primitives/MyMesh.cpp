#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

																									   // Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader);

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));

	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
		//A--B
//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //Front
	AddQuad(point0, point1, point3, point2);

	//Back
	AddQuad(point5, point4, point6, point7);

	//Left
	AddQuad(point4, point0, point7, point3);

	//Right
	AddQuad(point1, point5, point2, point6);

	//Up
	AddQuad(point3, point2, point7, point6);

	//Down
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

													   //Front
	AddQuad(point0, point1, point3, point2);

	//Back
	AddQuad(point5, point4, point6, point7);

	//Left
	AddQuad(point4, point0, point7, point3);

	//Right
	AddQuad(point1, point5, point2, point6);

	//Up
	AddQuad(point3, point2, point7, point6);

	//Down
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code

	//GenerateCube(a_fRadius * 2.0f, a_v3Color);

	// radius 0.5f
	// height 1.0f
	// subdivisions 7

	vector3 coneCenter(0, 0, 0); // COOR: Center of the base 
	vector3 coneHeight(0, 0, a_fHeight); // COOR: Height of the cone

	std::vector<vector3> coorArray; // Point Coordinates Array

	float currentX = a_fRadius; // radius is current x + radius x=0
	float currentY = 0; // radius is current y y=0
	float step = ((float)TWOPI / a_nSubdivisions); // Define Angle based on subdivisions
	float currentAngle = step; // store angle

	for (int i = 0; i <= a_nSubdivisions; i++) {

		float newX = (float)cos(currentAngle); // cosine of angle
		float newY = (float)sin(currentAngle); // sin of angle

		newX = newX * a_fRadius;
		newY = newY * a_fRadius;

		vector3 currentPoint = vector3(currentX, currentY, 0); // generate x y z coord

		coorArray.push_back(currentPoint); // push back x y z coord into array

										   // ADD TRIANGLES TO DRAW SHAPE
		if (i != 0) {
			// Base
			AddTri(coorArray[i], coorArray[i - 1], coneCenter);
			AddTri(coorArray[i - 1], coorArray[i], coneCenter);

			// Height
			AddTri(coorArray[i], coorArray[i - 1], coneHeight);
			AddTri(coorArray[i - 1], coorArray[i], coneHeight);
		}

		currentX = newX;
		currentY = newY;

		currentAngle += step; // next angle
	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// GenerateCube(a_fRadius * 2.0f, a_v3Color);

	vector3 cylCenter(0, 0, 0); // COOR: Center of the base 
	vector3 cylHeight(0, 0, a_fHeight); // COOR: Height of the cylinder

	std::vector<vector3> coorArray; // Point Coordinates Array

	float currentX = a_fRadius; // radius is current x + radius x=0
	float currentY = 0; // radius is current y y=0
	float step = ((float)TWOPI / a_nSubdivisions); // Define Angle based on subdivisions
	float currentAngle = step; // store angle

	for (int i = 0; i <= a_nSubdivisions; i++) {

		float newX = (float)cos(currentAngle); // cosine of angle
		float newY = (float)sin(currentAngle); // sin of angle

		newX = newX * a_fRadius;
		newY = newY * a_fRadius;

		vector3 currentPoint = vector3(currentX, currentY, 0); // generate x y z coord

		coorArray.push_back(currentPoint); // push back x y z coord into array

										   // DRAW SHAPE
		if (i != 0) {

			// Bottom Base
			AddTri(coorArray[i], coorArray[i - 1], cylCenter);
			AddTri(coorArray[i - 1], coorArray[i], cylCenter);

			// Top Base
			AddTri(
				vector3(coorArray[i].x, coorArray[i].y, a_fHeight),
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fHeight),
				cylHeight);
			AddTri(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fHeight),
				vector3(coorArray[i].x, coorArray[i].y, a_fHeight),
				cylHeight);

			// Walls
			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fHeight), // top left
				vector3(coorArray[i].x, coorArray[i].y, a_fHeight)); // top (i) right (a_fHeight)

		}

		currentX = newX;
		currentY = newY;

		currentAngle += step; // next angle
	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);

	vector3 tubeHeight(0, 0, a_fHeight); // COOR: Height of the tube

	std::vector<vector3> coorArray; // Point Coordinates Array

	float tubeRadius = a_fOuterRadius - a_fInnerRadius;
	float currentX = a_fOuterRadius; // radius is current x + radius x=0
	float currentY = 0; // radius is current y y=0
	float step = ((float)TWOPI / a_nSubdivisions); // Define Angle based on subdivisions
	float currentAngle = step; // store angle

	for (int i = 0; i <= a_nSubdivisions; i++) {

		float newX = (float)cos(currentAngle); // cosine of angle
		float newY = (float)sin(currentAngle); // sin of angle

		newX = newX * a_fOuterRadius;
		newY = newY * a_fOuterRadius;

		vector3 currentPoint = vector3(currentX, currentY, 0); // generate x y z coord

		coorArray.push_back(currentPoint); // push back x y z coord into array

										   // DRAW SHAPE
		if (i != 0) {

			// Bottom Base
			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, 0.0f), // top left
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, 0.0f)); // top (i) right (a_fHeight)
			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom right
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, 0.0f), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, 0.0f)); // top (i) right (a_fHeight)

																					 // Top Base
			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fHeight), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y, a_fHeight), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, a_fHeight), // top left
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, a_fHeight)); // top (i) right (a_fHeight)
			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y, a_fHeight), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fHeight), // bottom right
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, a_fHeight), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, a_fHeight)); // top (i) right (a_fHeight)

																						  // Outer Wall
			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fHeight), // top left
				vector3(coorArray[i].x, coorArray[i].y, a_fHeight)); // top (i) right (a_fHeight)

			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom right
				vector3(coorArray[i].x, coorArray[i].y, a_fHeight), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fHeight)); // top (i) right (a_fHeight)

																			 // Inner Wall
			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, 0.0f), // bottom right
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, a_fHeight), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, a_fHeight)); // top (i) right (a_fHeight)

			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, 0.0f), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, a_fHeight), // top left
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, a_fHeight)); // top (i) right (a_fHeight)
		}

		currentX = newX;
		currentY = newY;

		currentAngle += step; // next angle
	}


	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	// GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);


	vector3 tubeHeight(0, 0, 1.0f); // COOR: Height of the tube

	std::vector<vector3> coorArray; // Point Coordinates Array

	float tubeRadius = a_fOuterRadius - a_fInnerRadius;
	float currentX = a_fOuterRadius; // radius is current x + radius x=0
	float currentY = 0; // radius is current y y=0
	float step = ((float)TWOPI / a_nSubdivisionsA); // Define Angle based on subdivisions
	float currentAngle = step; // store angle

	for (int i = 0; i <= a_nSubdivisionsA; i++) {

		float newX = (float)cos(currentAngle); // cosine of angle
		float newY = (float)sin(currentAngle); // sin of angle

		newX = newX * a_fOuterRadius;
		newY = newY * a_fOuterRadius;

		vector3 currentPoint = vector3(currentX, currentY, 0); // generate x y z coord

		coorArray.push_back(currentPoint); // push back x y z coord into array

										   // DRAW SHAPE
		if (i != 0) {

			// Bottom Base
			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, 0.0f), // top left
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, 0.0f)); // top (i) right (a_fHeight)
			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom right
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, 0.0f), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, 0.0f)); // top (i) right (a_fHeight)

																					 // Top Base
			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fInnerRadius), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y, a_fInnerRadius), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, a_fInnerRadius), // top left
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, a_fInnerRadius)); // top (i) right (a_fHeight)
			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y, a_fInnerRadius), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fInnerRadius), // bottom right
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, a_fInnerRadius), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, a_fInnerRadius)); // top (i) right (a_fHeight)

																							   // Outer Wall
			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fInnerRadius), // top left
				vector3(coorArray[i].x, coorArray[i].y, a_fInnerRadius)); // top (i) right (a_fHeight)

			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom right
				vector3(coorArray[i].x, coorArray[i].y, a_fInnerRadius), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fInnerRadius)); // top (i) right (a_fHeight)

																				  // Inner Wall
			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, 0.0f), // bottom right
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, a_fInnerRadius), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, a_fInnerRadius)); // top (i) right (a_fHeight)

			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, 0.0f), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y - tubeRadius, a_fInnerRadius), // top left
				vector3(coorArray[i].x, coorArray[i].y - tubeRadius, a_fInnerRadius)); // top (i) right (a_fHeight)
		}

		currentX = newX;
		currentY = newY;

		currentAngle += step; // next angle
	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	// GenerateCube(a_fRadius * 2.0f, a_v3Color);

	// (x, y, z) = (sin(Pi * m/M) cos(2Pi * n/N), sin(Pi * m/M) sin(2Pi * n/N), cos(Pi * m/M))
	// Where m is current latitude and M is last latitude in array
	// Where n is current longitude and N is last longitude in array

	vector3 bottomSphere(0, 0, a_fRadius - (2 * a_fRadius)); // COOR: bottom of the sphere
	vector3 topSphere(0, 0, a_fRadius + a_fRadius); // COOR: top of the sphere

	std::vector<vector3> coorArray; // Point Coordinates Array

	float currentX = a_fRadius; // radius is current x + radius x=0
	float currentY = 0; // radius is current y y=0
	float step = ((float)TWOPI / a_nSubdivisions); // Define Angle based on subdivisions
	float currentAngle = step; // store angle

	for (int i = 0; i <= a_nSubdivisions; i++) {

		float newX = (float)cos(currentAngle); // cosine of angle
		float newY = (float)sin(currentAngle); // sin of angle

		newX = newX * a_fRadius;
		newY = newY * a_fRadius;

		vector3 currentPoint = vector3(currentX, currentY, 0); // generate x y z coord

		coorArray.push_back(currentPoint); // push back x y z coord into array

										   // DRAW SHAPE
		if (i != 0) {

			// Bottom Base
			AddTri(coorArray[i], coorArray[i - 1], bottomSphere);
			AddTri(coorArray[i - 1], coorArray[i], bottomSphere);

			// Top Base
			AddTri(
				vector3(coorArray[i].x, coorArray[i].y, a_fRadius),
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fRadius),
				topSphere);
			AddTri(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fRadius),
				vector3(coorArray[i].x, coorArray[i].y, a_fRadius),
				topSphere);


			// (x, y, z) = (sin(Pi * m/M) cos(2Pi * n/N), sin(Pi * m/M) sin(2Pi * n/N), cos(Pi * m/M))
			// Where m is current latitude and M is last latitude in array
			// Where n is current longitude and N is last longitude in array

			// Build sphere with latitude and logitude coordinate plotting

			// Outer Wall
			AddQuad(
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom right
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fRadius), // top left
				vector3(coorArray[i].x, coorArray[i].y, a_fRadius) // top (i) right (a_fHeight)
			);

			// Inner Wall
			AddQuad(
				vector3(coorArray[i].x, coorArray[i].y, 0.0f), // bottom (i-1) left (0)
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, 0.0f), // bottom right
				vector3(coorArray[i].x, coorArray[i].y, a_fRadius), // top left
				vector3(coorArray[i - 1].x, coorArray[i - 1].y, a_fRadius) // top (i) right (a_fHeight)
			);

			// Middle
			//AddQuad(
			//	vector3(
			//		((float)sin(PI*coorArray[i - 1].x)) * ((float)cos(TWOPI * coorArray[i - 1].z)),
			//		((float)sin(PI*coorArray[i - 1].x)) * ((float)sin(TWOPI * coorArray[i - 1].z)),
			//		((float)cos(PI * coorArray[i - 1].x))
			//	),
			//	vector3(
			//		((float)sin(PI*coorArray[i].x)) * ((float)cos(TWOPI * coorArray[i - 1].z)),
			//		((float)sin(PI*coorArray[i].x)) * ((float)sin(TWOPI * coorArray[i - 1].z)),
			//		((float)cos(PI * coorArray[i].x))
			//	),
			//	vector3(
			//		((float)sin(PI*coorArray[i - 1].x)) * ((float)cos(TWOPI * coorArray[i].z)),
			//		((float)sin(PI*coorArray[i - 1].x)) * ((float)sin(TWOPI * coorArray[i].z)),
			//		((float)cos(PI * coorArray[i - 1].x))
			//	),
			//	vector3(
			//	((float)sin(PI*coorArray[i].x)) * ((float)cos(TWOPI * coorArray[i].z)),
			//		((float)sin(PI*coorArray[i].x)) * ((float)sin(TWOPI * coorArray[i].z)),
			//		((float)cos(PI * coorArray[i].x))
			//	)
			//);

			//// Middle
			//AddQuad(
			//	vector3(
			//	((float)sin(PI*coorArray[i].x)) * ((float)cos(TWOPI * coorArray[i].z)),
			//		((float)sin(PI*coorArray[i].x)) * ((float)sin(TWOPI * coorArray[i].z)),
			//		((float)cos(PI * coorArray[i].x))
			//	),
			//	vector3(
			//	((float)sin(PI*coorArray[i - 1].x)) * ((float)cos(TWOPI * coorArray[i].z)),
			//		((float)sin(PI*coorArray[i - 1].x)) * ((float)sin(TWOPI * coorArray[i].z)),
			//		((float)cos(PI * coorArray[i - 1].x))
			//	),
			//	vector3(
			//	((float)sin(PI*coorArray[i].x)) * ((float)cos(TWOPI * coorArray[i - 1].z)),
			//		((float)sin(PI*coorArray[i].x)) * ((float)sin(TWOPI * coorArray[i - 1].z)),
			//		((float)cos(PI * coorArray[i].x))
			//	),
			//	vector3(
			//	((float)sin(PI*coorArray[i - 1].x)) * ((float)cos(TWOPI * coorArray[i - 1].z)),
			//		((float)sin(PI*coorArray[i - 1].x)) * ((float)sin(TWOPI * coorArray[i - 1].z)),
			//		((float)cos(PI * coorArray[i - 1].x))
			//	)
			//);

		}

		currentX = newX;
		currentY = newY;

		currentAngle += step; // next angle
	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}