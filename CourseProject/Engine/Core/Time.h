#ifndef Engine_Time_hpp
#define Engine_Time_hpp

#include <chrono>

typedef double DeltaTime;

namespace Engine
{
	class Time
	{
	public:
		Time();
		~Time();
		DeltaTime Update();
		DeltaTime GetDeltaTime();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _start;
		std::chrono::time_point<std::chrono::high_resolution_clock> _end;
		DeltaTime _deltaTime;
	};

	inline Time::Time() {
		_start = std::chrono::system_clock::now();
		_end = _start;

	}

	inline Time::~Time()
	{}

	inline DeltaTime Time::Update() {
		_start = _end;
		_end = std::chrono::high_resolution_clock::now();
		_deltaTime = std::chrono::duration<double>(_end - _start).count();
		return _deltaTime;
	}

	inline DeltaTime Time::GetDeltaTime() {
		return _deltaTime;
	}
}
#endif /*END TIME_H*/