#pragma once
#include <string>
#include <iostream>
#include "ResourceMgr.h"
#include "../../FloaterUtil/include/FloaterMacro.h"

namespace flt
{
	struct IBuilderBase;
	template<typename T> struct IBuilder;


	struct ResourceBase
	{
		friend class ResourceMgr;

		ResourceBase() :
			ResourceBase(L"")
		{

		}
		ResourceBase(const std::wstring& key) :
			ResourceBase(key, nullptr)
		{

		}
		ResourceBase(const std::wstring& key, void* pData) :
			_key(key),
			_pData(pData)
		{

		}
		ResourceBase(const ResourceBase& other) = delete;
		ResourceBase(ResourceBase&& other) noexcept :
			_pData(other._pData),
			_key(other._key)
		{
			other._pData = nullptr;
		}
		virtual ~ResourceBase()
		{
		}

		ResourceBase& operator=(const ResourceBase& other) = delete;
		ResourceBase& operator=(ResourceBase&& other) noexcept
		{
			_pData = other._pData;
			_key = other._key;
			other._pData = nullptr;
			return *this;
		}

	protected:
		std::wstring _key;
		void* _pData;
	};

	template <typename T>
	concept HasReleaseFunc = requires(T a) {
		{ a.Release() } -> std::same_as<void>;
	};

	template<HasReleaseFunc T>
	struct Resource : ResourceBase
	{
	public:
		Resource() : ResourceBase() {}
		Resource(const typename IBuilder<T>& builder) : ResourceBase(builder.key)
		{
			Set(builder);
		}
		Resource(const Resource& other)
		{
			global::g_resourceMgr.AddRefResource(&other);
			_pData = other._pData;
			_key = other._key;
		}
		Resource(Resource&& other) = default;

		Resource& operator=(const Resource& other)
		{
			if (this == &other)
			{
				return *this;
			}

			Release();
			global::g_resourceMgr.AddRefResource(&other);
			_pData = other._pData;
			_key = other._key;
			return *this;
		}
		Resource& operator=(Resource&& other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}

			Release();
			_pData = other._pData;
			_key = other._key;
			other._pData = nullptr;
			return *this;
		}

		virtual ~Resource()
		{
			Release();
		}

		void Set(const IBuilderBase& builder)
		{
			Release();
			global::g_resourceMgr.GetResource(this, builder);
		}

		T* Get()
		{
			return (T*)_pData;
		}

		void Release()
		{
			if (_pData)
			{
				if (global::g_resourceMgr.ReleaseResource(this))
				{
					((T*)_pData)->Release();
					delete (T*)_pData;
				}
				_pData = nullptr;
			}
		}

		T& operator*() const
		{
			ASSERT(_pData, "Not Setting Data");
			return *((T*)_pData);
		}

		T* operator->() const
		{
			ASSERT(_pData, "Not Setting Data");
			return (T*)_pData;
		}
	};
}
