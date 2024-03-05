#include "./include/ResourceMgr.h"
#include "./include/Resource.h"
#include "./include/IBuilder.h"


namespace flt
{
	namespace global
	{
		ResourceMgr g_resourceMgr;
	}
}

void flt::ResourceMgr::GetResource(ResourceBase* outResource, const IBuilderBase& builder)
{
	std::lock_guard<std::recursive_mutex> lock(resourceMutex);
	outResource->_key = builder.key;
	if (resources.find(builder.key) == resources.end())
	{
		// 관리하지 않는 데이터일 경우 생성
		std::wstring typeName = builder.GetTypeName();
		outResource->_pData = builder();
		// 생성 실패 시 nullptr 반환
		if (outResource->_pData == nullptr)
		{
			return;
		}
		resources[builder.key] = { outResource->_pData, typeName };

		//std::wcerr << this << " - " << L"Created : " << typeName << std::endl;
		//auto[iter, ret] = resources.emplace(builder.key, data, typeName);
	}
	else
	{
		// 관리중일 데이터일 경우 참조 카운트 증가
		outResource->_pData = resources[builder.key].GetData();
	}
}

bool flt::ResourceMgr::ReleaseResource(ResourceBase* resource)
{
	std::lock_guard<std::recursive_mutex> lock(resourceMutex);
	if (resources.find(resource->_key) != resources.end())
	{
		if (resources[resource->_key].Release())
		{
			//std::wcerr << this << " - " << L"Released" << resources[resource->_key].typeName << std::endl;

			resources.erase(resource->_key);
			return true;
			// 빌더의 Release 를 호출해야함.
			// Resource의 소멸자가 virtual이기 때문에 해당 객체가 소멸할 때 소멸자에서 Release를 호출해줌.
		}
	}

	return false;
}

bool flt::ResourceMgr::AddRefResource(const ResourceBase* resource)
{
	std::lock_guard<std::recursive_mutex> lock(resourceMutex);
	if (resources.find(resource->_key) == resources.end())
	{
		// 관리하지 않는 데이터일 경우 별다른 작업을 하지 않음.
		return false;
	}
	else
	{
		// 관리중일 데이터일 경우 참조 카운트 증가
		resources[resource->_key].AddRef();
	}
	return true;
}
