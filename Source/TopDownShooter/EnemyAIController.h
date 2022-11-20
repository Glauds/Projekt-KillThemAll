// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	void HandlePlayerOrOwnerDeath();

private:

	UPROPERTY()
	class ASoldier* Player;

	FTimerHandle MovingTimer;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptanceRadius = 50;

	void SetMovement();
	void Move();

	bool IsPlayerAlive() const;

protected:

	virtual void BeginPlay() override;
};
