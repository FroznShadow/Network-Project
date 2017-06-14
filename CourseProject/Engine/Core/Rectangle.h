#ifndef ENGINE_Rectangle_hpp
#define ENGINE_Rectangle_hpp

#include <iostream>

#include <glew.h>
#include <glm/glm.hpp>

#include <Core/Managers/EntityManager.h>

#include <Core/Components/Transform.h>
#include <Core/Components/Render.h>
#include <Core/Components/AABB.hpp>

namespace Engine
{
	class Rectangle : public Entity
	{
	public:
		Rectangle(glm::vec2 position, glm::vec2 size, GLfloat depth);
		virtual ~Rectangle();

		virtual void Init() override;
		virtual void Cleanup() override;

		virtual void Update(DeltaTime deltaTime) override;

		bool PointIntersect(Window* window, Camera* cam, glm::vec2 point);
	private:
		std::shared_ptr<Transform>	_transform;
		std::shared_ptr<Render>		_render;
		std::shared_ptr<AABB>		_aabb;

		glm::vec2					_position;
		glm::vec2					_size;
		GLfloat						_depth;
	};

	inline Rectangle::Rectangle(glm::vec2 position, glm::vec2 size, GLfloat depth)
		: _position(position), _size(size), _depth(depth), Entity()
	{

	};

	inline Rectangle::~Rectangle()
	{
	};

	inline void Rectangle::Init() {
		std::vector<glm::vec3> vertices = {
			glm::vec3(-0.1f, -0.1f, _depth),
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.1f, -0.1f, _depth),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-0.1f, 0.1f, _depth),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.1f, 0.1f, _depth),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		std::vector<glm::uvec3> indices = {
			glm::vec3(0, 1, 2),
			glm::vec3(1, 3, 2)
		};

		EntityManager* EM = EntityManager::GetInstance();

		EM->AddComponent<Transform>(this->GetName(), glm::vec3(_position, _depth), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(_size, 1.0f));
		EM->AddComponent<Render>(this->GetName(), vertices, indices);
		EM->AddComponent<AABB>(this->GetName());

		_transform = EM->GetEntity(this->GetName())->GetComponent<Transform>();
		_render = EM->GetEntity(this->GetName())->GetComponent<Render>();
		_aabb = EM->GetEntity(this->GetName())->GetComponent<AABB>();
	};

	inline void Rectangle::Cleanup() {

	};

	inline void Rectangle::Update(DeltaTime deltaTime) {

	};

	inline bool Rectangle::PointIntersect(Window* window, Camera* cam, glm::vec2 point) {
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), window->GetSize().x / window->GetSize().y, 0.01f, 100.0f);
		glm::mat4 view = cam->GetViewMatrix();

		glm::mat4 Inverse = glm::inverse(view * projection);

		float in[2];
		in[0] = (2.0f*((float)(point.x - 0) / (window->GetSize().x - 0))) - 1.0f;
		in[1] = 1.0f - (2.0f*((float)(point.y - 0) / (window->GetSize().y - 0)));
		glm::vec4 mouse = glm::vec4(in[0], in[1], _depth, 1.0f);

		mouse = mouse * Inverse;

		mouse.w = 1.0 / mouse.w;

		mouse.x *= mouse.w;
		mouse.y *= mouse.w;
		mouse.z *= mouse.w;

		std::cout << mouse.x << " " << mouse.y << " " << mouse.z << std::endl;

		return true;
	}
}

#endif