#pragma once

#include "ModuleManager.h"

class IBlockBuilderPlugin : public IModuleInterface
{
public:
	static inline IBlockBuilderPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IBlockBuilderPlugin >( "BlockBuilderPlugin" );
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "BlockBuilderPlugin" );
	}
};