// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

#include "Soldier.h"

#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ASoldier>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player != nullptr) 
	{
		SetMovement();
	} 
}

void AEnemyAIController::SetMovement()
{
	SetFocus(Player);
	GetWorldTimerManager().SetTimer(MovingTimer, this, &AEnemyAIController::Move, 1, true);
}

void AEnemyAIController::Move()
{
	if (IsPlayerAlive())
	{
		MoveToActor(Player, AcceptanceRadius);
	}
}

bool AEnemyAIController::IsPlayerAlive() const
{
	return Player != nullptr && Player->IsAlive();
}

void AEnemyAIController::HandlePlayerOrOwnerDeath()
{
	GetWorldTimerManager().ClearTimer(MovingTimer);
	ClearFocus(EAIFocusPriority::Gameplay);
	StopMovement();
}


















