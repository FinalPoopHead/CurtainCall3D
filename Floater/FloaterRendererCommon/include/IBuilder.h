#pragma once
#include <string>
#include "../../FloaterUtil/include/ConvString.h"


namespace flt
{
	//class ResourceMgr;

	/// <summary>
	/// 실제 데이터 객체를 만들기 위한 인터페이스.
	/// key는 리소스 메니져에서 리소스를 구분하기 위해 사용할 키값이다.
	/// 
	/// TODO key에 타입 이름을 붙여서 넘겨 다른 타입의 경우 같은 key를 넘기더라도 따로 동작할 수 있도록 해야한다.
	/// TODO 같은 key를 넘겼을 경우에 같은 builder인지 체크하고 다를 경우에 경고를 날릴 수 있도록 만들어야 한다.
	/// TODO key에 DX11 에대한 리소스인지 등 렌더러마다 다른 리소스 key값을 가지도록 만들어야한다.
	/// </summary>
	struct IBuilderBase
	{
		IBuilderBase() = default;
		IBuilderBase(const std::wstring& key) : key(key) {}
		IBuilderBase(const IBuilderBase& other) : key(other.key) {}
		IBuilderBase(IBuilderBase&& other) noexcept : key(std::move(other.key)) {}

		IBuilderBase& operator=(const IBuilderBase& other) = delete;
		IBuilderBase& operator=(IBuilderBase&& other) noexcept = delete;

		virtual void* operator()() const = 0;
		virtual std::wstring GetTypeName() const = 0;
		std::wstring key;
	};

	// 실제 데이터 객체의 타입을 알고 있는 계층.
	template<typename T>
	struct IBuilder : public IBuilderBase
	{
		using type = T;

		IBuilder() = default;
		IBuilder(const std::wstring& key) : IBuilderBase(key + L":" + ConvertToWstring(typeid(T).name())) {}
		IBuilder(const IBuilder& other) : IBuilderBase(other) {}
		IBuilder(IBuilder&& other) noexcept : IBuilderBase(std::move(other)) {}

		IBuilder& operator=(const IBuilder& other)
		{
			key = other.key;
			return *this;
		}
		IBuilder& operator=(IBuilder&& other) noexcept
		{
			key = std::move(other.key);
			return *this;
		}

		virtual void* operator()() const final
		{
			return build();
		}

		virtual std::wstring GetTypeName() const final
		{
			return ConvertToWstring(typeid(T).name());
		}

		virtual T* build() const = 0;
	};
}
