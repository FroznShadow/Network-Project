#ifndef Engine_h
#define Engine_h

//Core
#include <Core/Window.h>
#include <Core/Camera.hpp>
#include <Core/Rectangle.hpp>
#include <Core/Input/Input.h>

//Managers
#include <Core/Managers/EntityManager.hpp>
#include <Core/Managers/SceneManager.hpp>
#include <Core/Managers/StateManager.hpp>
#include <Core/Managers/SystemManager.hpp>

//Systems
#include <Core/Systems/Buffer.hpp>
#include <Core/Systems/PhysicsSystem.hpp>
#include <Core/Systems/RenderingSystem.hpp>

//Components
#include <Core/Components/AABB.hpp>
#include <Core/Components/Audio.h>
#include <Core/Components/Color.hpp>
#include <Core/Components/Material.hpp>
#include <Core/Components/Render.hpp>
#include <Core/Components/Shader.hpp>
#include <Core/Components/Texture.h>
#include <Core/Components/Transform.hpp>
#include <Core/Components/Camera.hpp>

#endif