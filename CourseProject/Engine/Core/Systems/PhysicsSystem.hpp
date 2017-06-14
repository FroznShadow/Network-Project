#ifndef Engine_RenderingSystem_hpp
#define Engine_RenderingSystem_hpp

#include <chrono>

#include <glew.h>
#include <gl\GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <lib/Helper.hpp>

#include <Core/Shaders/ShaderProgram.hpp>
#include <Core/Managers/SystemManager.hpp>

#include <Core/Components/Transform.hpp>
#include <Core/Components/Render.hpp>
#include <Core/Components/Color.hpp>
#include <Core/Components/AABB.hpp>
#include <Core/Components/Shader.hpp>
#include <Core/Components/Texture.h>
#include <Core/Components/Camera.hpp>

#include <Core/Systems/Buffer.hpp>

namespace Engine
{
	class RenderingSystem : public System
	{
	public:
		RenderingSystem(Window* window, const char* vertexShaderPath = "Resources/Vert.txt", const char* fragmentShaderPath = "Resources/Frag.txt")
			: _window(window), _defaultShader(new ShaderProgram), System() {

			_defaultShader->CompileShader(vertexShaderPath, GL_VERTEX_SHADER);
			_defaultShader->CompileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
			GLAssert();
		};

		virtual ~RenderingSystem() {};

		virtual void Init() override;
		virtual void Cleanup() override;

		virtual void Pause() override;
		virtual void Resume() override;

		virtual void Update(DeltaTime deltaTime) override;

		void SetCamera(std::shared_ptr<Camera> cam) { _cam = cam; };

	private:
		EntityManager* _entityManager;
		ShaderProgram* _defaultShader;
		Window* _window;

		Buffer _vertexBuffer;
		Buffer _indiceBuffer;

		std::shared_ptr<Camera> _cam;
	};

	inline void RenderingSystem::Init() {
		_entityManager = EntityManager::GetInstance();

		GLAssert();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		GLAssert();

		glUseProgram(0);

		_vertexBuffer.CreateBuffer(GL_ARRAY_BUFFER);
		_indiceBuffer.CreateBuffer(GL_ELEMENT_ARRAY_BUFFER);

		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		GLAssert();
	};

	inline void RenderingSystem::Cleanup() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SwapBuffers(_window->GetHDC());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};

	inline void RenderingSystem::Pause() {
		if (!_paused) {
			_paused = true;
		}
	};

	inline void RenderingSystem::Resume() {
		if (_paused) {
			_paused = false;
		}
	};

	inline void RenderingSystem::Update(DeltaTime deltaTime) {
		if (!_paused) {
			GLAssert();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			std::vector<std::shared_ptr<Entity>> entities = _entityManager->GetEntities();
			for (std::shared_ptr<Entity> entity : entities) {

				//Get components needded for rendering
				std::shared_ptr<Transform>	transform = entity->GetComponent<Transform>();
				std::shared_ptr<Render>		render = entity->GetComponent<Render>();
				std::shared_ptr<AABB>		aabb = entity->GetComponent<AABB>();
				std::shared_ptr<Texture>	texture = entity->GetComponent<Texture>();
				std::shared_ptr<Shader>		shader = entity->GetComponent<Shader>();
				GLAssert();

				//Check for the bare minimum components needed for rendering
				if (transform != nullptr && render != nullptr) {

					//If entity has shader component use it. Otherwise use default shader
					GLuint shaderID;
					if (shader != nullptr) { shaderID = shader->GetProgramID(); }
					else { shaderID = _defaultShader->GetProgramID(); }
					glUseProgram(shaderID);
					GLAssert();

					//Get Data
					glm::mat4 Model = glm::translate(glm::mat4(1), transform->GetPosition());

					glm::mat4 Scale = glm::scale(glm::mat4(1), transform->GetScale());

					glm::mat4 Rotate = glm::toMat4(transform->GetRotationQuat());

					Model = Model * Rotate * Scale;

					glm::mat4 View = _cam->GetViewMatrix();

					glm::mat4 Projection = glm::perspective(glm::radians(60.0f), _window->GetSize().x / _window->GetSize().y, 0.01f, 400.0f);

					glm::vec3 ViewPosition = _cam->GetPosition();

					glm::vec3 LightPosition = glm::vec3(150.0f, 150.0f, 0.0f);
					GLAssert();

					//Bind Data
					if (texture != nullptr) {
						texture->BindTexture(shaderID);
					}

					_vertexBuffer.BindBufferData(render->GetVertexData().size(), &render->GetVertexData()[0].x);
					_indiceBuffer.BindBufferData(render->GetIndiceData().size(), &render->GetIndiceData()[0].x);
					GLAssert();

					GLint ModelLocation = glGetUniformLocation(shaderID, "Model");
					GLint ViewLocation = glGetUniformLocation(shaderID, "View");
					GLint ProjectionLocation = glGetUniformLocation(shaderID, "Projection");
					GLint ViewPositionLocation = glGetUniformLocation(shaderID, "ViewPosition");
					GLint LightPositionLocation = glGetUniformLocation(shaderID, "LightPosition");
					GLAssert();

					GLint PositionLocation = glGetAttribLocation(shaderID, "in_Position");
					GLint TexCoordinateLocation = glGetAttribLocation(shaderID, "in_TexCoord");
					GLint NormalLocation = glGetAttribLocation(shaderID, "in_Normal");
					GLAssert();

					glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));
					glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(View));
					glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(Projection));
					glUniform3fv(ViewPositionLocation, 1, glm::value_ptr(ViewPosition));
					glUniform3fv(LightPositionLocation, 1, glm::value_ptr(LightPosition));
					GLAssert();

					if (shader != nullptr) { shader->BindShader(); }

					if (PositionLocation != -1) {
						glEnableVertexAttribArray(PositionLocation);
						glVertexAttribPointer(PositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(0 * sizeof(GLfloat)));
						GLAssert();
					} //if (PositionLocation != -1)

					if (TexCoordinateLocation != -1) {
						glEnableVertexAttribArray(TexCoordinateLocation);
						glVertexAttribPointer(TexCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(3 * sizeof(GLfloat)));
						GLAssert();
					} //if (TexCoordinateLocation != -1)

					if (NormalLocation != -1) {
						glEnableVertexAttribArray(NormalLocation);
						glVertexAttribPointer(NormalLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*)(6 * sizeof(GLfloat)));
						GLAssert();
					} //if (NormalLocation != -1)

					  //Draw object
					glDrawElements(GL_TRIANGLES, render->GetIndiceData().size() * 3, GL_UNSIGNED_INT, (void*)0);
					GLAssert();
#ifdef  DRAW_AABB
					if (aabb != nullptr) {
						_vertexBuffer.BindBufferData(aabb->GetVertexData().size(), &aabb->GetVertexData()[0].x);
						_indiceBuffer.BindBufferData(aabb->GetIndiceData().size(), &aabb->GetIndiceData()[0].x);

						Model = glm::translate(glm::mat4(1), transform->GetPosition());

						glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));

						if (PositionLocation != -1) {
							glEnableVertexAttribArray(PositionLocation);
							glVertexAttribPointer(PositionLocation, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(glm::vec3), (void*)(0 * sizeof(GLfloat)));
							GLAssert();
						} //if (PositionLocation != -1)

						glDrawElements(GL_LINE_STRIP, aabb->GetIndiceData().size() * 3, GL_UNSIGNED_INT, (void*)0);
					} //if (aabb != nullptr)
#endif // DRAW_AABB

				} //if (transform != nullptr && render != nullptr)
			} //for (std::shared_ptr<Entity> entity : entities)

			SwapBuffers(_window->GetHDC());
			glUseProgram(0);
			GLAssert();

		} //if (!_paused)
	};
}

#endif