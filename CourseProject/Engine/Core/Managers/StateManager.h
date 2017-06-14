#ifndef StateManager_hpp
#define StateManager_hpp

//--------------------------------------------------------------------------------------//
// StateManager.hpp																		//
// [3/10/2016 Rasmus]																	//
//--------------------------------------------------------------------------------------//
//
namespace Engine {																		//
																						//
																						//--------------------------------------------------------------------------//		//
																						// STRUCT - State															//		//
																						//--------------------------------------------------------------------------//		//
	struct State																//		//
	{																			//		//
		State() {};																//		//
		virtual ~State() {};													//		//
																				//		//
		virtual void Init() = 0;												//		//
		virtual void Cleanup() = 0;												//		//
																				//		//
		virtual void Pause() = 0;												//		//
		virtual void Resume() = 0;												//		//
																				//		//
		virtual void Update() = 0;												//		//
		virtual void Display() = 0;												//		//
	};																			//		//
																				//--------------------------------------------------------------------------//		//
																				//
																				//--------------------------------------------------------------------------//		//
																				// CLASS - StateManager														//		//
																				//--------------------------------------------------------------------------//		//
	class StateManager															//		//
	{																			//		//
	public:																		//		//
		static StateManager* getInstance() {									//		//
			static StateManager instance;										//		//
			return &instance;													//		//
		};																		//		//
																				//		//
		void ChangeState(State* state);											//		//
		void PushState(State* state);											//		//
		void PopState();														//		//
		void Clear();															//		//
																				//		//
	private:																	//		//
		StateManager();															//		//
		~StateManager();														//		//
																				//		//
		StateManager(StateManager const&);										//		//
		void operator=(StateManager const&);									//		//
																				//		//
		State** _states;														//		//
		size_t _size;															//		//
	};																			//		//
																				//--------------------------------------------------------------------------//		//
																				//
																				//--------------------------------------------------------------------------//		//
																				// DEFINITIONS - StateManager												//		//
																				//--------------------------------------------------------------------------//		//
	inline void StateManager::ChangeState(State* state) {						//		//
		if (_states[_size] != nullptr) {										//		//
			_states[_size]->Cleanup();											//		//
			delete _states[_size];												//		//
			_states[_size] = nullptr;											//		//
		}																		//		//
		_states[_size] = state;													//		//
		_states[_size]->Init();													//		//
	};																			//		//
																				//		//
	inline void StateManager::PushState(State* state) {							//		//
		if (_states[_size] != nullptr) {										//		//
			_states[_size++]->Pause();											//		//
		}																		//		//
		_states[_size] = state;													//		//
		_states[_size]->Init();													//		//
	};																			//		//
																				//		//
	inline void StateManager::PopState() {										//		//
		if (_states[_size] != nullptr) {										//		//
			_states[_size]->Cleanup();											//		//
			delete _states[_size];												//		//
			if (_size == 0) { _states[_size] = nullptr; }						//		//
			else { _states[_size--] = nullptr; }								//		//
		}																		//		//
		if (_states[_size] != nullptr) {										//		//
			_states[_size]->Resume();											//		//
		}																		//		//
	};																			//		//
																				//		//
	inline void StateManager::Clear() {											//		//
		while (_states[_size] != nullptr) {										//		//
			if (_size == 0) {													//		//
				_states[_size]->Cleanup();										//		//
				delete _states[_size];											//		//
				_states[_size] = nullptr;										//		//
			}																	//		//
			else {																//		//
				_states[_size]->Cleanup();										//		//
				delete _states[_size];											//		//
				_states[_size--] = nullptr;										//		//
			}																	//		//
		}																		//		//
	};																			//		//
																				//		//
	inline StateManager::StateManager() : _size(0) {							//		//
		_states = new State*;													//		//
		_states[_size] = nullptr;												//		//
	};																			//		//
																				//		//
	inline StateManager::~StateManager() {};									//		//
																				//--------------------------------------------------------------------------//		//
																				//
}																						//
																						//
																						//--------------------------------------------------------------------------------------//

#endif