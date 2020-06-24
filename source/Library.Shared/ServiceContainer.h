#pragma once

#include <map>
#include "RTTI.h"

namespace Library
{
	class ServiceContainer final
	{
	public:
		ServiceContainer() = default;
		ServiceContainer(const ServiceContainer&) = delete;
		ServiceContainer& operator=(const ServiceContainer&) = delete;
		ServiceContainer(ServiceContainer&&) = delete;
		ServiceContainer& operator=(ServiceContainer&&) = delete;
		~ServiceContainer() = default;

		void AddService(Library::RTTI::IdType typeID, void* service);
		void RemoveService(RTTI::IdType typeID);
		void* GetService(RTTI::IdType typeID) const;

		std::map<RTTI::IdType, void*> mServices;
	};
}