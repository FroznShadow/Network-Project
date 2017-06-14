#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include "Window.h"
#include "Core/Time.h"

#include <glew.h>
#include <windows.h>
#include <Core/Input/Input.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define DRAW_AABB

#include <lib\Helper.h>

#include <Core/Managers/EntityManager.h>
#include <Core/Managers/SystemManager.h>
#include <Core/Systems/RenderingSystem.h>
#include <Core/Systems/PhysicsSystem.hpp>

#include <Core/Components/Render.h>
#include <Core/Components/Color.h>
#include <Core/Components/AABB.hpp>
#include <Core/Managers/SceneManager.h>

#include <Core/ResourceManager.h>

#include <Core/Engine_Camera.h>

#include <Core/Rectangle.h>

class player : public Engine::Entity
{
public:
	player() {};
	virtual ~player() {};

	void Init() {};
	void Cleanup() {};
	void Update(DeltaTime dt) {};
};

Window window;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		window.Resize();
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

class TestScene : public Engine::Scene
{
public:
	TestScene() {};
	virtual ~TestScene() {};

	virtual void Init()  override {
		cam = new Camera();

		EM = Engine::EntityManager::GetInstance();
		SM = Engine::SystemManager::GetInstance();

		SM->AddSystem<Engine::PhysicsSystem>();
		SM->AddSystem<Engine::RenderingSystem>(&window, "Resources/Vert.txt", "Resources/Frag.txt");

		//if (SM->GetSystem<Engine::RenderingSystem>() != nullptr) {
		//	SM->GetSystem<Engine::RenderingSystem>()->SetCamera(cam);
		//}

		player1 = EM->AddEntity("player", std::make_shared<player>());
		test = EM->AddEntity("box", std::make_shared<player>());

		Resource* box = ResourceManager::GetInstance()->LoadResource("Resources/cube.obj");
		Resource* audiores = ResourceManager::GetInstance()->LoadResource("Resources/bossMusic.wav");

		EM->AddComponent<Engine::Render>("player", box->_vertices, box->_indices);
		EM->AddComponent<Engine::Transform>("player", glm::vec3(2.0f, 0.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		EM->AddComponent<Engine::AABB>("player");
		EM->AddComponent<Engine::Texture>("player", "Resources/Texture1.png");
		//EM->AddComponent<Engine::Audio>("player");

		EM->AddComponent<Engine::Render>("box", box->_vertices, box->_indices);
		EM->AddComponent<Engine::Transform>("box", glm::vec3(-5.0f, 0.0f, -17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		EM->AddComponent<Engine::AABB>("box");
		EM->AddComponent<Engine::Texture>("box", "Resources/Texture4.png");

		//EM->AddEntity("asd", std::make_shared<Engine::Rectangle>(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), -1.0f));
		//EM->AddComponent<Engine::Texture>("asd", "Resources/Texture4.png");

		player1->AddChild(test);

		trans = player1->GetComponent<Engine::Transform>();
		//std::shared_ptr<Engine::Audio> audio = player1->GetComponent<Engine::Audio>();

		//audiores->getAudioSystem()->audioPlay();
		//audiores->Play();
		precision = 1.0;
		//Json::Value event;
		//for (auto it : EM->GetEntities())
		//{
		//	event["Entities"][it->GetName()];
		//	for (auto j : it->GetChildren())
		//	{
		//		event["Entities"][it->GetName()]["Child"] = j->GetName();

		//	}
		//	/*for (auto k: it->GetComponents())
		//	{
		//		event["Entities"][it->GetName()]["Component"]  = NULL;
		//	}*/


		//}
		//Json::StyledWriter writer;
		//std::string outputConfig = writer.write(event);

		//std::cout << event;
	};

	virtual void Cleanup()  override {
		EM->Clear();
		SM->Clear();
	};

	virtual void Pause()  override {};

	virtual void Resume()  override {};

	virtual void Update(DeltaTime deltaTime)  override {
		float direction = 1.0f;
		Inputs::Input* inp = nullptr;
		if (inp->getKeyDown(VK_SHIFT)) { direction = -1.0f; };
		if (inp->getKeyDown(VK_CONTROL)) { precision += 0.1f*direction; };
		if (inp->getKeyDown(VK_LEFT)) { trans->Move(glm::vec3(precision*deltaTime*-5.0f, 0.0f, 0.0f)); }
		if (inp->getKeyDown(VK_RIGHT)) { trans->Move(glm::vec3(precision*deltaTime*5.0f, 0.0f, 0.0f)); }
		if (inp->getKeyDown(VK_UP)) { trans->Move(glm::vec3(0.0f, precision*deltaTime*5.0f, 0.0f)); }
		if (inp->getKeyDown(VK_DOWN)) { trans->Move(glm::vec3(0.0f, precision*deltaTime*-5.0f, 0.0f)); }
		if (inp->getKeyDown(VK_SPACE)) { trans->SetRotation(glm::vec3(45.0f, 45.0f, .0f)); }
		if (inp->getKeyDown('X')) { trans->Rotate(glm::vec3(precision*direction*deltaTime*25.0f, 0.0f, 0.0f)); }
		if (inp->getKeyDown('Y')) { trans->Rotate(glm::vec3(0.0f, precision*direction*deltaTime*25.0f, 0.0f)); }
		if (inp->getKeyDown('Z')) { trans->Rotate(glm::vec3(0.0f, 0.0f, precision*direction*deltaTime*25.0f)); }

		EM->Update(deltaTime);
		SM->Update(deltaTime);
	};

private:
	float precision;

	Camera* cam;

	Engine::EntityManager* EM;
	Engine::SystemManager* SM;

	std::shared_ptr<Engine::Entity> player1;
	std::shared_ptr<Engine::Entity> test;
	std::shared_ptr<Engine::Transform> trans;
};

int main(int argc, char** argv) {
	window.createWindow("Dickbutt!", glm::vec2(1920, 1080), glm::vec2(0, 0), "Resources/Cursor.ico", "Resources/Cursor.ico", ENGINE_WINDOWED, WndProc);
	window.InitOpenGL();

	Engine::Time timer;
	Engine::SceneManager::GetInstance()->ChangeScene(new TestScene());

	while (window.IsOpen()) {

		DeltaTime dt = timer.Update();

		Engine::SceneManager::GetInstance()->Update(dt);

		window.getMessage();
	}

	window.Uninit();

	return 0;
}