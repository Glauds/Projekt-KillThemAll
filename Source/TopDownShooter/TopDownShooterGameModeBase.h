// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ATopDownShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void PlayerKilled();

	virtual void EnemyKilled();
};
