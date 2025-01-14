#include "App.h"
#pragma warning(disable : 4996)

void App::Init(std::string wndName, std::string className, HINSTANCE hInstance, const int width, const int height)
{
	this->width = width;
	this->height = height;
	timer.Start();
	//Overloaded initialize window
	if (!wnd.InitializeWindow(wndName, className, hInstance, width, height))
	{
		Error::Log("Something happened when initialize the window (overload)");
	}

	if (!gfx.InitializeGraphics(wnd.GetHWND(), width, height))
	{
		Error::Log("Failed to initialize graphics");
	}

	SetSavedValues();
}

void App::Update() noexcept
{
	const float deltaTime = static_cast<float>(timer.GetMilisecondsElapsed());
	timer.Restart();
	//Set any char to any event
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
	}

	//Set any key to any event
	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();

		if (keycode == VK_ESCAPE) {
			wnd.SetHWND(nullptr);
		}
	}
	float cameraSpeed = 0.0005f;
	//Set any mouse event to any event
	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				gfx.cam3D.AdjustRotation(static_cast<float>(me.GetPosY()) * 0.009f, static_cast<float>(me.GetPosX()) * 0.009f, 0);
			}
		}
	}

	if (keyboard.KeyIsPressed(VK_SHIFT))
	{
		//increase speed when pressed left shift
		cameraSpeed = 0.1f * deltaTime;
	}
	if (keyboard.KeyIsPressed('F'))
	{
		//increase speed when pressed F
		cameraSpeed = 0.001f * deltaTime;
	}
	if (keyboard.KeyIsPressed('W'))
	{
		//move forward when pressed W
		gfx.cam3D.AdjustPosition(gfx.cam3D.GetForwardVector() * cameraSpeed * deltaTime);
	}
	if (keyboard.KeyIsPressed('S'))
	{
		//move backward when pressed S
		gfx.cam3D.AdjustPosition(gfx.cam3D.GetBackwardVector() * cameraSpeed * deltaTime);
	}
	if (keyboard.KeyIsPressed('A'))
	{
		//move left when pressed A
		gfx.cam3D.AdjustPosition(gfx.cam3D.GetLeftVector() * cameraSpeed * deltaTime);
	}
	if (keyboard.KeyIsPressed('D'))
	{
		//move right when pressed D
		gfx.cam3D.AdjustPosition(gfx.cam3D.GetRightVector() * cameraSpeed * deltaTime);
	}
	if (keyboard.KeyIsPressed('Q'))
	{
		//add up position when pressed Q
		gfx.cam3D.AdjustPosition(0.0f, cameraSpeed * deltaTime, 0.0f);
	}
	if (keyboard.KeyIsPressed('E'))
	{
		//add down position when pressed E
		gfx.cam3D.AdjustPosition(0.0f, -cameraSpeed * deltaTime, 0.0f);
	}
	if (keyboard.KeyIsPressed('C'))
	{
		//set point light position to character pos when pressed C
		gfx.pPointLight.SetObjectPosition(gfx.cam3D.GetPositionFloat3().x,
			gfx.cam3D.GetPositionFloat3().y, gfx.cam3D.GetPositionFloat3().z);
	}

	//Set sky box position to camera position
	//that is makes spherical skybox spherical skybox :D
	gfx.pSkyBox.SetPos(gfx.cam3D.GetPositionFloat3(), deltaTime);
}

void App::RenderFrame()
{
	//Render Frame/sec (refresh rate)
	//Draw test triangle all shaded
	gfx.BeginFrame();
	//Run FPS Counter func
	FPSCounter();
	if (!gfx.SceneGraph())
	{
		Error::Log("Something happened to run scene graph");
	}
	//Run end frame func
	gfx.EndFrame();
}

bool App::ProcessMessages(HINSTANCE hInstance) const noexcept
{
	//Create msg and hwnd
	MSG msg;
	HWND hwnd = wnd.GetHWND();
	ZeroMemory(&msg, sizeof(MSG));
	//Get Message and dispatch theese messages
	while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(hwnd))
		{
			//open point light txt, stores point light settings
			gfx.pPointLight.pointLightSetting.OpenFileWrite("pointlight_settings.txt");
			gfx.pPointLight.pPointLightSavedItems.push_back(std::to_string(gfx.pPointLight.GetAmbientIntensity()));
			gfx.pPointLight.pPointLightSavedItems.push_back(std::to_string(gfx.pPointLight.GetDiffuseIntensity()));
			gfx.pPointLight.pPointLightSavedItems.push_back(std::to_string(gfx.pPointLight.GetSpecularIntensity()));
			gfx.pPointLight.pPointLightSavedItems.push_back(std::to_string(gfx.pPointLight.GetObjectPositionX()));
			gfx.pPointLight.pPointLightSavedItems.push_back(std::to_string(gfx.pPointLight.GetObjectPositionY()));
			gfx.pPointLight.pPointLightSavedItems.push_back(std::to_string(gfx.pPointLight.GetObjectPositionZ()));
			gfx.pPointLight.pPointLightSavedItems.push_back(std::to_string(gfx.pPointLight.GetNormalMapEnabled()));
			gfx.pPointLight.pointLightSetting.AddInfo(gfx.pPointLight.pPointLightSavedItems);
			gfx.pPointLight.pointLightSetting.CloseFile();
			//open camera txt, stores camera position and rotation data
			cameraSetting.OpenFileWrite("camera_settings.txt");
			pCameraSavedItems.push_back(std::to_string(gfx.cam3D.GetPositionFloat3().z));
			pCameraSavedItems.push_back(std::to_string(gfx.cam3D.GetPositionFloat3().y));
			pCameraSavedItems.push_back(std::to_string(gfx.cam3D.GetPositionFloat3().x));
			pCameraSavedItems.push_back(std::to_string(gfx.cam3D.GetRotationFloat3().z));
			pCameraSavedItems.push_back(std::to_string(gfx.cam3D.GetRotationFloat3().y));
			pCameraSavedItems.push_back(std::to_string(gfx.cam3D.GetRotationFloat3().x));
			cameraSetting.AddInfo(pCameraSavedItems);
			cameraSetting.CloseFile();

			//open dev menu txt, stores dev menu settings
			devMenuSettings.OpenFileWrite("devmenu_settings.txt");
			pDevMenuSavedItems.push_back(std::to_string(*GameObject::GetDepthBufferEnabled()));
			pDevMenuSavedItems.push_back(std::to_string(*GameObject::GetBlurEnabled()));
			pDevMenuSavedItems.push_back(std::to_string(*GameObject::GetFogEnabled()));
			pDevMenuSavedItems.push_back(std::to_string(*GameObject::GetFogStart()));
			pDevMenuSavedItems.push_back(std::to_string(*GameObject::GetFogEnd()));
			pDevMenuSavedItems.push_back(std::to_string(*GameObject::GetWireframeEnabled()));
			devMenuSettings.AddInfo(pDevMenuSavedItems);
			devMenuSettings.CloseFile();

			hwnd = nullptr; //Message processing loop takes care of destroying this window
			UnregisterClass("janus", hInstance);	//Unregister class (closing window)
			return false;	//closing program
		}
	}

	return true;
}

void App::SetSavedValues()
{
	//set camera saved values
	cameraSetting.OpenFileRead("camera_settings.txt");
	gfx.cam3D.SetPosition(float(std::atoi(cameraSetting.GetInfo(0).c_str())), float(std::atoi(cameraSetting.GetInfo(1).c_str())),
		float(std::atoi(cameraSetting.GetInfo(2).c_str())));
	gfx.cam3D.SetRotation(float(std::atoi(cameraSetting.GetInfo(3).c_str())), float(std::atoi(cameraSetting.GetInfo(4).c_str())),
		float(std::atoi(cameraSetting.GetInfo(3).c_str())));
	cameraSetting.CloseFile();

	//set dev menu saved values
	devMenuSettings.OpenFileRead("devmenu_settings.txt");
	if (devMenuSettings.GetInfo(0) == "1")
		GameObject::SetDepthBufferEnabled(true);
	else
		GameObject::SetDepthBufferEnabled(false);

	if (devMenuSettings.GetInfo(1) == "1")
		GameObject::SetBlurEnabled(true);
	else
		GameObject::SetBlurEnabled(false);

	if (devMenuSettings.GetInfo(2) == "1")
		GameObject::SetFogEnabled(true);
	else
		GameObject::SetFogEnabled(false);

	GameObject::SetFogStart(float(std::atoi(devMenuSettings.GetInfo(3).c_str())));
	GameObject::SetFogEnd(float(std::atoi(devMenuSettings.GetInfo(4).c_str())));
	GameObject::SetWireframeEnabled(float(std::atoi(devMenuSettings.GetInfo(5).c_str())));
	//close dev menu file
	devMenuSettings.CloseFile();
}

void App::FPSCounter()
{
	//fps counter
	static int fpsCounter = 0;
	fpsCounter += 1;
	static std::string fps;
	//set fps text value
	if (gfx.timer.GetMilisecondsElapsed() > 1000.0f)
	{
		fps = "FPS: " + std::to_string(fpsCounter) + ")";
		fpsCounter = 0;
		gfx.timer.Restart();
	}
	char tempString[17];
	char cpuString[17];
	//Print cpu usage data
	_itoa_s(gfx.pCPU.GetCpuPercentage(), tempString, 11);
	strcpy_s(cpuString, "CPU Usage: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");
	std::string a = cpuString;
	//dev menu creation
	UI::DeveloperUI(std::to_string(gfx.timer.GetMilisecondsElapsed()), a.c_str() ,fps.c_str(), &gfx.cam3D, GameObject::GetDepthBufferEnabled(), GameObject::GetBlurEnabled(), GameObject::GetWireframeEnabled(),
		GameObject::GetWireColor(), GameObject::GetFogEnabled(), GameObject::GetFogColor(), GameObject::GetFogStart(),
		GameObject::GetFogEnd());
}