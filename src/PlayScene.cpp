#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	// PlayScene.cpp -> draw()
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);

	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	// PlayScene.cpp -> handleEvents()
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_N)) pressed[0] = true;
	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_N) && pressed[0]) {
		m_pTarget->beThrowedToReachST();
		pressed[0] = false;
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_M)) pressed[1] = true;
	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_M) && pressed[1]) {
		m_pTarget->beThrowedInMaxDistance();
		pressed[1] = false;
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);

	// PlayScene.cpp -> start()
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");

	// PlayScene.cpp -> start()
	m_pTarget = new Target;
	addChild(m_pTarget);
	m_pPlayer->getTransform()->position = glm::vec2(20.0f, 500.0f);
	m_pTarget->getTransform()->position = glm::vec2(50.0f, 500.0f);
	m_pPlaneSprite->getTransform()->position = glm::vec2(485.0f + 50.0f, 500.0f);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	// PlayScene.cpp -> GUI_Function()
	ImGui::Begin("Physics Control", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	if (ImGui::Button("Throw in 95m/s velocity to reach Stormtrooper")) {
		m_pTarget->reSet();
		m_pTarget->beThrowedToReachST();
	}
	if (ImGui::Button("Throw with Max distance in 95m/s velocity")) {
		m_pTarget->reSet();
		m_pTarget->beThrowedInMaxDistance();
	}
	if (ImGui::Button("Reset")) {
		m_pTarget->reSet();
	}
	// PlayScene.cpp -> GUI_Function()
	ImGui::SameLine();
	if (ImGui::Button(m_pTarget->getPauseFlag() ? "resume" : "pause")) {
		m_pTarget->setPause(!m_pTarget->getPauseFlag());
	}
	std::string str1 = " X: " + std::to_string(m_pTarget->getTransform()->position.x)
		+ " Y: " + std::to_string(m_pTarget->getTransform()->position.y);
	const char* cstr1 = str1.c_str();
	ImGui::LabelText("Position", cstr1);
	std::string str2 = " X: " + std::to_string(m_pTarget->getRigidBody()->velocity.x)
		+ " Y: " + std::to_string(m_pTarget->getRigidBody()->velocity.y);
	const char* cstr2 = str2.c_str();
	ImGui::LabelText("Velocity", cstr2);
	std::string str3 = " X: " + std::to_string(m_pTarget->getRigidBody()->acceleration.x)
		+ " Y: " + std::to_string(m_pTarget->getRigidBody()->acceleration.y);
	const char* cstr3 = str3.c_str();
	ImGui::LabelText("Acceleration", cstr3);
	std::string str4 = " X: " + std::to_string(m_pPlaneSprite->getTransform()->position.x - m_pTarget->getTransform()->position.x)
		+ " Y: " + std::to_string(m_pPlaneSprite->getTransform()->position.y - m_pTarget->getTransform()->position.y);
	const char* cstr4 = str4.c_str();
	ImGui::LabelText("Distance between thermal detonator and stormtrooper", cstr4);

	ImGui::Separator();

	// PlayScene.cpp -> GUI_Function()
	static bool isGravityEnabled = m_pTarget->getGravityFlag();
	if (ImGui::Checkbox("Gravity", &isGravityEnabled)) {
		m_pTarget->setGravity(isGravityEnabled);
	}
	static float distance[1] = { 0.0f };
	ImGui::SliderFloat("Distance to StormTrooper", distance, 0, 1200 - 40 - 50);
	static float velocity[1] = { 0.0f };
	ImGui::SliderFloat("Throw Speed", velocity, 0, 100);
	static float angle[1] = { 0.0f };
	ImGui::SliderFloat("Throw Angle", angle, 0, 90);
	if (ImGui::Button("Throw with manipulated speed and angle")) {
		m_pTarget->reSet();
		m_pTarget->throwWithSpeed(velocity[0], angle[0] / 180 * 3.14);
		m_pPlaneSprite->getTransform()->position.x = distance[0] + 50.0f;
	}
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
