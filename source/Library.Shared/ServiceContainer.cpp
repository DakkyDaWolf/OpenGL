#include "pch.h"
#include "ServiceContainer.h"

using namespace std;

namespace Library
{
	ServiceContainer GlobalServices;

	void ServiceContainer::AddService(RTTI::IdType typeID, void* service)
	{
		mServices.insert(pair<RTTI::IdType, void*>(typeID, service));
	}

	void ServiceContainer::RemoveService(RTTI::IdType typeID)
	{
		mServices.erase(typeID);
	}

	void* ServiceContainer::GetService(RTTI::IdType typeID) const
	{
		map<RTTI::IdType, void*>::const_iterator it = mServices.find(typeID);

		return (it != mServices.end() ? it->second : nullptr);
	}
}