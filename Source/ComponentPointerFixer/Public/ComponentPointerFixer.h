// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct FToolMenuSection;

class FComponentPointerFixerModule : public IModuleInterface
{

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void RegisterMenus();
	void RegisterFixAction(FToolMenuSection& section);
};
