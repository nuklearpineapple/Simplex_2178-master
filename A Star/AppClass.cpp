#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Tim Ascencio - ta3755 @rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(vector3(2.0f, 2.0f, 15.0f), vector3(2.0f, 2.0f, 0.0f), AXIS_Y);

	m_pModel = new Simplex::Model();
	m_pModel->Load("Sorted\\WallEye.bto");

	// initialize and run pathfinder, create path to travel on
	pathfinder->Run();

	// add initial all stops based on all nodes
	for (uint i = 0; i < pathfinder->initialNodes.size(); i++) {
		vector3 nodeCoordinates = pathfinder->initialNodes[i]->GetCoordinates();
		m_allStops.push_back(nodeCoordinates);
		
	}

	for (uint i = 0; i < pathfinder->finalPath.size(); i++) {
		vector3 finalStops = pathfinder->finalPath[i]->GetCoordinates();
		m_stopsList.push_back(finalStops);
	}

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Draw the model
	m_pModel->PlaySequence();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//calculate the current position
	vector3 v3CurrentPos;

	//your code goes here

	static int positionCount = 0; // retains current point in array (starting point)

	float endTime = 2.0f; // amount of time spent traveling

	int startPoint = positionCount; // current point in array (starting point)
	int endPoint;

	if (positionCount < m_stopsList.size() - 1)
		endPoint = positionCount + 1; // the following point in array (goal | end point)
	else
		endPoint = 0;

	vector3 startCoord = m_stopsList[startPoint]; // starting coordinates for lerp
	vector3 endCoord = m_stopsList[endPoint]; // goal and/or ending coordinates for lerp

	static float currentX = m_stopsList[0].x;

	vector3 time = vector3(fTimer / endTime, fTimer / endTime, fTimer / endTime); // time percent

	// Linear Interpolation Formula (start + percent*(end - start));
	v3CurrentPos = vector3(startCoord + time * (endCoord - startCoord)); // move

	if (fTimer >= 2) {
		if (positionCount < m_stopsList.size())
			positionCount++;

		fTimer = 0;
	}

	if (positionCount >= m_stopsList.size()) {
		positionCount = 0;
	}

	//-------------------
	
	
	matrix4 m4Model = glm::translate(v3CurrentPos);
	m_pModel->SetModelMatrix(m4Model);

	m_pMeshMngr->Print("\nTimer: ");//Add a line on top
	m_pMeshMngr->PrintLine(std::to_string(fTimer), C_YELLOW);

	for (uint i = 0; i < m_allStops.size(); ++i)
	{
		// draw stops
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_allStops[i]) * glm::scale(vector3(0.2f)), C_RED, RENDER_SOLID);

		// find coord to draw lines
		vector3 sPath = pathfinder->FindNode("s")->GetCoordinates();
		vector3 aPath = pathfinder->FindNode("a")->GetCoordinates();
		vector3 bPath = pathfinder->FindNode("b")->GetCoordinates();
		vector3 cPath = pathfinder->FindNode("c")->GetCoordinates();
		vector3 dPath = pathfinder->FindNode("d")->GetCoordinates();
		vector3 gPath = pathfinder->FindNode("g")->GetCoordinates();
		vector3 fPath = pathfinder->FindNode("f")->GetCoordinates();

		// draw lines
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), sPath, aPath, C_RED, C_RED); // s to a
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), sPath, bPath, C_RED, C_RED); // s to b
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), aPath, cPath, C_RED, C_RED); // a to c
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), aPath, gPath, C_RED, C_RED); // a to g
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), gPath, bPath, C_RED, C_RED); // g to b
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), gPath, cPath, C_RED, C_RED); // g to c
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), bPath, dPath, C_RED, C_RED); // b to d 
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), cPath, fPath, C_RED, C_RED); // c to f
		m_pMeshMngr->AddLineToRenderList(glm::translate(ZERO_V3), dPath, fPath, C_RED, C_RED); // d to f
	}
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pModel);
	//release GUI
	ShutdownGUI();
}