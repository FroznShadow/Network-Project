#pragma once
#include <chrono>

class DeltaTime
{
public:
	DeltaTime() :Start(std::chrono::high_resolution_clock::now()), End(Start) {};

	double update()
	{
		Start = End;
		End = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<double>(End - Start).count();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> Start;
	std::chrono::time_point<std::chrono::high_resolution_clock> End;
};