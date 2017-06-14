#ifndef StateMachine_hpp
#define StateMachine_hpp

#include "SFML\Graphics.hpp"
class State
{
public:
	virtual ~State() {}
	virtual void Init() = 0;
	virtual void Cleanup() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void Update(DeltaTime time) = 0;
	virtual void Draw(sf::RenderWindow &window) = 0;
};

class StateMachine
{
public:
	static StateMachine* getInstance() { static StateMachine instance; return &instance; };
	void ChangeState(State* state);
	void PushState(State* state);
	void PopState();
	void Clear();
	void Update(DeltaTime time);
	void Draw(sf::RenderWindow &window);
private:
	StateMachine();
	~StateMachine() {}
	StateMachine(StateMachine const&);
	void operator=(StateMachine const&);
	State** stateList;
	size_t size;
};

inline void StateMachine::ChangeState(State* state)
{
	if (stateList[size] != nullptr)
	{
		stateList[size]->Cleanup();
		delete stateList[size];
		//stateList[size] = nullptr;
	}
	stateList[size] = state;
	stateList[size]->Init();
}

inline void StateMachine::PushState(State* state)
{
	if (stateList[size] != nullptr)
	{
		stateList[size++]->Pause();
	}
	stateList[size] = state;
	stateList[size]->Init();
}

inline void StateMachine::PopState()
{
	if (stateList[size] != nullptr)
	{
		stateList[size]->Cleanup();
		delete stateList[size];
		if (size == 0) { stateList[size] = nullptr; }
		else {
			stateList[size--] = nullptr;
			stateList[size]->Resume();
		}
	}
}
inline void StateMachine::Clear()
{
	while (stateList[size] != nullptr)
	{
		if (size == 0)
		{
			stateList[size]->Cleanup();
			delete stateList[size];
			stateList[size] = nullptr;
		}
		else
		{
			stateList[size]->Cleanup();
			delete stateList[size];
			stateList[size--] = nullptr;
		}
	}
}
inline StateMachine::StateMachine() : size(0)
{
	stateList = new State*;
	stateList[size] = nullptr;
}
inline void StateMachine::Update(DeltaTime time)
{
	stateList[size]->Update(time);
}
inline void StateMachine::Draw(sf::RenderWindow &window)
{
	stateList[size]->Draw(window);
}

#endif