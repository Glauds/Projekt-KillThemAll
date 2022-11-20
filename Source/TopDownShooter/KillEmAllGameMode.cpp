// Fill out your copyright notice in the Description page of Project Settings.

#include "KillEmAllGameMode.h"

#include "SoldierController.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "Spawner.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void AKillEmAllGameMode::StartPlay()
{
	Super::StartPlay();

	SetupSpawner();
	StartGameTimer();
	StartSpawner();
}

void AKillEmAllGameMode::SetupSpawner()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (SpawnerClass != nullptr) 
	{
		Spawner = GetWorld()->SpawnActor<ASpawner>(SpawnerClass, SpawnParams);
	}
}

void AKillEmAllGameMode::StartGameTimer()
{
	GetWorldTimerManager().SetTimer(GameTimer, this, &AKillEmAllGameMode::DecreaseGameTime, 1, true);
}

void AKillEmAllGameMode::StartSpawner()
{
	if (Spawner != nullptr)
	{
		Spawner->SpawnEnemies();
		Spawner->StartSpawningEnemies();
		Spawner->StartSpawningHealthPickups();
		Spawner->StartSpawningAmmoPickups();
	}
}

void AKillEmAllGameMode::DecreaseGameTime() 
{
	GameTime--;

	if (GameTime <= 0) 
	{
		HandleGameTimerOver();
	}
}

void AKillEmAllGameMode::HandleGameTimerOver() 
{
	StopGameTimer();
	StopSpawningEnemies();

	GetWorldTimerManager().SetTimer(CheckForAliveEnemiesTimer, this, &AKillEmAllGameMode::CheckForAliveEnemies, 0.5, true);
}

void AKillEmAllGameMode::StopGameTimer()
{
	GetWorldTimerManager().ClearTimer(GameTimer);
}

void AKillEmAllGameMode::StopSpawningEnemies()
{
	if (Spawner != nullptr)
	{
		Spawner->StopSpawningEnemies();
	}
}

void AKillEmAllGameMode::CheckForAliveEnemies() 
{
	if (AllEnemiesAreDead())
	{
		HandlePlayerWin();
	}
}

bool AKillEmAllGameMode::AllEnemiesAreDead() const
{
	for (AEnemy* Enemy : TActorRange<AEnemy>(GetWorld()))
	{
		if (Enemy != nullptr && Enemy->IsAlive())
		{
			return false;
		}
	}

	return true;
}

void AKillEmAllGameMode::HandlePlayerWin()
{
	GetWorldTimerManager().ClearTimer(CheckForAliveEnemiesTimer);
	StopSpawningPickups();

	EndGame(true);
}

void AKillEmAllGameMode::StopSpawningPickups()
{
	Spawner->StopSpawningHealthPickups();
	Spawner->StopSpawningAmmoPickups();
}

void AKillEmAllGameMode::PlayerKilled()
{
	Super::PlayerKilled();

	HandlePlayerDeath();
}

void AKillEmAllGameMode::HandlePlayerDeath() 
{
	StopGameTimer();
	StopSpawner();
	StopAliveEnemies();
	StopAliveEnemiesSounds();

	EndGame(false);
}

void AKillEmAllGameMode::StopSpawner()
{
	if (Spawner != nullptr)
	{
		Spawner->StopSpawningEnemies();
		Spawner->StopSpawningHealthPickups();
		Spawner->StopSpawningAmmoPickups();
	}
}

void AKillEmAllGameMode::StopAliveEnemies()
{
	for (AEnemyAIController* EnemyController : TActorRange<AEnemyAIController>(GetWorld()))
	{
		if (EnemyController != nullptr)
		{
			EnemyController->HandlePlayerOrOwnerDeath();
		}
	}
}

void AKillEmAllGameMode::StopAliveEnemiesSounds() 
{
	for (AEnemy* Enemy : TActorRange<AEnemy>(GetWorld()))
	{
		if (Enemy != nullptr && Enemy->IsAlive())
		{
			Enemy->StopSpawningAliveSounds();
		}
	}
}

void AKillEmAllGameMode::EndGame(bool bIsWinner)
{
	ASoldierController* SoldierController = Cast<ASoldierController>(GetWorld()->GetFirstPlayerController());
	if (SoldierController != nullptr) 
	{
		SoldierController->SetIsKillMode(false);
		SoldierController->GameHasEnded(SoldierController->GetPawn(), bIsWinner);
	}
}

float AKillEmAllGameMode::GetGameTime() const
{
	return GameTime;
}









