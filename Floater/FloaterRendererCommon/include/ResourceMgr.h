#pragma once
#include <unordered_map>
#include <string>
#include <atomic>
#include <mutex>

#include <iostream>

/// <summary>
/// 앞으로 할 일
/// 같은 KEY값을 가지고 있지만 다른 데이터를 가진 Builder가 들어왔을 경우에 체크할 수 있도록.
/// </summary>


namespace flt
{
	struct ResourceBase;
	struct IBuilderBase;

	//extern template class __declspec(dllexport) std::wstring;

	class ResourceMgr
	{
	public:
		ResourceMgr() : 
			resources(), 
			resourceMutex()
		{
			std::wcerr << this << std::endl;
		}
		~ResourceMgr()
		{
			CheckManagedData();
		}

	public:
		void GetResource(ResourceBase* resource, const IBuilderBase& builder);
		bool ReleaseResource(ResourceBase* resource);

		bool AddRefResource(const ResourceBase* resource);

	private:
		void CheckManagedData()
		{
			for (const auto& [key, value] : resources)
			{
				wprintf(L"%s, Address : 0x%p, refcount : %d\n", value.typeName.c_str(), value.data, value.refCount);
			}
		}

	private:
		struct ManagedData
		{
			ManagedData() : data(nullptr), refCount(0) {}
			ManagedData(void* data, const std::wstring& typeName) : data(data), refCount(1), typeName(typeName) {}

			bool AddRef()
			{
				refCount++;
				return true;
			}

			void* GetData()
			{
				refCount++;
				return data;
			}

			// refCount 를 감소시키고, 0 이 되면 true를 반환.
			// 성공 실패 여부가 아닌 리소스 해제 여부를 반환.
			bool Release()
			{
				refCount--;
				if (refCount <= 0)
				{
					return true;
				}
				return false;
			}

			void* data;
			int refCount;
			std::wstring typeName;
		};

		std::unordered_map<std::wstring, ManagedData> resources;
		std::recursive_mutex resourceMutex;
	};

	//extern template class __declspec(dllexport) std::unordered_map< std::wstring, ResourceMgr::ManagedData>;
	namespace global
	{
		extern ResourceMgr g_resourceMgr;
	}
}
