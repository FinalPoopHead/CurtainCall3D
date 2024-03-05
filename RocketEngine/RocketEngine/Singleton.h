#pragma once

template <typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() = default;

private:
	Singleton(const Singleton& rhs) = delete;
	Singleton(Singleton&& rhs) = delete;
	Singleton& operator=(const Singleton& rhs) = delete;
	Singleton& operator=(Singleton&& rhs) = delete;
};