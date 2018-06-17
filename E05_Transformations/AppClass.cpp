#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);

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

	static float posX = -8.0f;

	// Drawing boxes from top left and going down column by column

	// Column One - Draw 3 boxes
	for (int i = 0; i < 3; i++) {
		static float posY = 3.0f;
		m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX, posY - i, 0.0f)));
	}

	// Column Two - Draw 2 boxes
	for (int i = 0; i < 2; i++) {
		static float posY = 4.0f;
		m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 1.0f, posY - i, 0.0f)));
	}

	// Column Three - Draw 7 boxes, skip [1]
	for (int i = 0; i < 7; i++) {
		static float posY = 7.0f;
		if(i!=1)
			m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 2.0f, posY - i, 0.0f)));
	}

	// Column Four - Draw 7 boxes, skip [2,5]
	for (int i = 0; i < 7; i++) {
		static float posY = 6.0f;
		if(i != 2 && i != 5)
			m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 3.0f, posY - i, 0.0f)));
	}
	
	// Column Five - Draw 6 boxes, skip [4]
	for (int i = 0; i < 6; i++) {
		static float posY = 5.0f;
		if (i != 4)
			m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 4.0f, posY - i, 0.0f)));
	}
	
	// Column Six - Draw 4 boxes
	for (int i = 0; i < 4; i++) {
		static float posY = 5.0f;
		m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 5.0f, posY - i, 0.0f)));
	}
	
	// Column Seven - Draw 6 boxes, skip [4]
	for (int i = 0; i < 6; i++) {
		static float posY = 5.0f;
		if (i != 4)
			m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 6.0f, posY - i, 0.0f)));
	}

	// Column Eight - Draw 7 boxes, skip [2,5]
	for (int i = 0; i < 7; i++) {
		static float posY = 6.0f;
		if (i != 2 && i!= 5)
			m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 7.0f, posY - i, 0.0f)));
	}
	
	// Column Nine - Draw 7 boxes, skip [1]
	for (int i = 0; i < 7; i++) {
		static float posY = 7.0f;

		if(i != 1)
			m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 8.0f, posY - i, 0.0f)));
	}

	// Column Ten - Draw 2 boxes
	for (int i = 0; i < 2; i++) {
		static float posY = 4.0f;
		m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 9.0f, posY - i, 0.0f)));
	}

	// Column Eleven - Draw 3 boxes
	for (int i = 0; i < 3; i++) {
		static float posY = 3.0f;
		m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(posX + 10.0f, posY - i, 0.0f)));
	}

	// increment float so entire entity travels to the right of the screen
	posX += 0.01f;

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
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}