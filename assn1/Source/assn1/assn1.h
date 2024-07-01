// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FAssn1Module : public IModuleInterface {
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};