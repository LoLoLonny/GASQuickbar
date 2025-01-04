// Copyright (C) 2024, Max Schindele

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGASQuickbarModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
