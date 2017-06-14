#ifndef Engine_SceneManager_hpp
#define Engine_SceneManager_hpp

//--------------------------------------------------------------------------------------//
// StateManager.hpp																		//
// [3/10/2016 Rasmus]																	//
//--------------------------------------------------------------------------------------//
//																						//
namespace Engine {																		//
																						//
																						//--------------------------------------------------------------------------//		//
																						// STRUCT - State															//		//
																						//--------------------------------------------------------------------------//		//
	struct Scene																//		//
	{																			//		//
		Scene() {};																//		//
		virtual ~Scene() {};													//		//
																				//		//
		virtual void Init() = 0;												//		//
		virtual void Cleanup() = 0;												//		//
																				//		//
		virtual void Pause() = 0;												//		//
		virtual void Resume() = 0;												//		//
																				//		//
		virtual void Update(DeltaTime deltaTime) = 0;							//		//
	};																			//		//
																				//--------------------------------------------------------------------------//		//
																				//
																				//--------------------------------------------------------------------------//		//
																				// CLASS - StateManager														//		//
																				//--------------------------------------------------------------------------//		//
	class SceneManager															//		//
	{																			//		//
	public:																		//		//
		static SceneManager* GetInstance() {									//		//
			static SceneManager _sceneManager;									//		//
			return &_sceneManager;												//		//
		};																		//		//
																				//		//
		void ChangeScene(Scene* scene);											//		//
		void PushScene(Scene* scene);											//		//
		void PopScene();														//		//
		void Clear();															//		//
																				//		//
		void Update(DeltaTime deltaTime);										//		//
																				//		//
		Scene* GetCurrentScene();												//		//
																				//		//
	private:																	//		//
		SceneManager();															//		//
		~SceneManager();														//		//
																				//		//
		SceneManager(SceneManager const&);										//		//
		void operator=(SceneManager const&);									//		//
																				//		//
		Scene** _scenes;														//		//
		size_t _size;															//		//
	};																			//		//
																				//--------------------------------------------------------------------------//		//
																				//
																				//--------------------------------------------------------------------------//		//
																				// DEFINITIONS - StateManager												//		//
																				//--------------------------------------------------------------------------//		//
	inline void SceneManager::ChangeScene(Scene* scene) {						//		//
		if (_scenes[_size] != nullptr) {										//		//
			_scenes[_size]->Cleanup();											//		//
			delete _scenes[_size];												//		//
			_scenes[_size] = nullptr;											//		//
		}																		//		//
		_scenes[_size] = scene;													//		//
		_scenes[_size]->Init();													//		//
	};																			//		//
																				//		//
	inline void SceneManager::PushScene(Scene* scene) {							//		//
		if (_scenes[_size] != nullptr) {										//		//
			_scenes[_size++]->Pause();											//		//
		}																		//		//
		_scenes[_size] = scene;													//		//
		_scenes[_size]->Init();													//		//
	};																			//		//
																				//		//
	inline void SceneManager::PopScene() {										//		//
		if (_scenes[_size] != nullptr) {										//		//
			_scenes[_size]->Cleanup();											//		//
			delete _scenes[_size];												//		//
			if (_size == 0) { _scenes[_size] = nullptr; }						//		//
			else { _scenes[_size--] = nullptr; }								//		//
		}																		//		//
		if (_scenes[_size] != nullptr) {										//		//
			_scenes[_size]->Resume();											//		//
		}																		//		//
	};																			//		//
																				//		//
	inline void SceneManager::Clear() {											//		//
		while (_scenes[_size] != nullptr) {										//		//
			if (_size == 0) {													//		//
				_scenes[_size]->Cleanup();										//		//
				delete _scenes[_size];											//		//
				_scenes[_size] = nullptr;										//		//
			}																	//		//
			else {																//		//
				_scenes[_size]->Cleanup();										//		//
				delete _scenes[_size];											//		//
				_scenes[_size--] = nullptr;										//		//
			}																	//		//
		}																		//		//
	};																			//		//
																				//		//
	inline void SceneManager::Update(DeltaTime deltaTime) {						//		//
		_scenes[_size]->Update(deltaTime);										//		//
	};																			//		//
																				//		//
	inline Scene* SceneManager::GetCurrentScene() {								//		//
		return _scenes[_size];													//		//
	};																			//		//
																				//		//
	inline SceneManager::SceneManager() : _size(0) {							//		//
		_scenes = new Scene*;													//		//
		_scenes[_size] = nullptr;												//		//
	};																			//		//
																				//		//
	inline SceneManager::~SceneManager() {										//		//
		this->Clear();															//		//
	};																			//		//
																				//--------------------------------------------------------------------------//		//
																				//
}																						//
																						//
																						//--------------------------------------------------------------------------------------//

#endif