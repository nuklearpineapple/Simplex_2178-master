#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	m_pOctant = new MyOctant(vector3(0.0f), 36.0f);

	m_pCube = new MyMesh();
	m_pMesh = new MyMesh();

#ifdef DEBUG
	uint uInstances = 900;
#else
	uint uInstances = 1849;
#endif
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			uIndex++;
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
			vector3 v3Position = vector3(glm::sphericalRand(34.0f));
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position);

			m_pOctant->Populate(uIndex);
			m_pOctant->SetEntityMngr(m_pEntityMngr);

			// contains entity check
			//m_pOctant->Contains(m_pEntityMngr->GetRigidBody(uIndex));
		}
	}
	m_uOctantLevels = 1;
	m_pOctant->Update(); // custom update
	//m_pEntityMngr->Update();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//Update Entity Manager
	//m_pEntityMngr->Update();

	// Custom update
	m_pOctant->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	//m_pOctant->Display();

	//Calculate the model, view and projection matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qArcBall));

	double dAngle = 360.0 / 6.0;
	vector3 v3Position;
	static uint uClock = m_pSystem->GenClock();
	static double dTimer = m_pSystem->GetDeltaTime(uClock);
	dTimer += m_pSystem->GetDeltaTime(uClock);

	if (changeCube == false) {
		m_pMesh->GenerateCube(62.0f, C_YELLOW);

		//Cube
		v3Position = vector3(0.0f);
		m_pCube->Render(m4Projection, m4View, glm::translate(v3Position));
	}

	if (changeCube == true) {
		m_pMesh->GenerateCube(30.0f, C_YELLOW);

		//Cube
		v3Position = vector3(0.0f);
		m_pCube->Render(m4Projection, m4View, glm::translate(v3Position));
	}

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}