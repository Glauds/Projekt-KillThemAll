// Fill out your copyright notice in the Description page of Project Settings.

#include "HighScoreGameMode.h"

#include "SoldierController.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "Spawner.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void AHighScoreGameMode::StartPlay()
{
	Super::StartPlay();

	SetupSpawner();
	StartSpawner();
}

void AHighScoreGameMode::SetupSpawner()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (SpawnerClass != nullptr)
	{
		Spawner = GetWorld()->SpawnActor<ASpawner>(SpawnerClass, SpawnParams);
	}
}

void AHighScoreGameMode::StartSpawner()
{
	if (Spawner != nullptr)
	{
		Spawner->SpawnEnemies();
		Spawner->StartSpawningEnemies(EnemySpawnRateInWave);
		StartWaveTimer();

		Spawner->StartSpawningHealthPickups();
		Spawner->StartSpawningAmmoPickups();
	}
}

void AHighScoreGameMode::StartWaveTimer()
{
	GetWorldTimerManager().SetTimer(WaveTimer, this, &AHighScoreGameMode::SpawnWave, WaveTime);
}

void AHighScoreGameMode::SpawnWave() 
{
	EnemySpawnRateInWave--;

	Spawner->StopSpawningEnemies();
	Spawner->SpawnEnemies();
	Spawner->StartSpawningEnemies(EnemySpawnRateInWave);

	if (EnemySpawnRateInWave > MinEnemySpawnRateInWave)
	{
		StartWaveTimer();
	}
}

void AHighScoreGameMode::StopWaveTimer() 
{
	GetWorldTimerManager().ClearTimer(WaveTimer);
}

void AHighScoreGameMode::EnemyKilled()
{
	EnemiesKilled++;
}

void AHighScoreGameMode::PlayerKilled()
{
	Super::PlayerKilled();

	HandlePlayerDeath();
}

void AHighScoreGameMode::HandlePlayerDeath()
{
	StopWaveTimer();
	StopSpawner();
	StopAliveEnemies();
	StopAliveEnemiesSounds();

	EndGame(false);
}

void AHighScoreGameMode::StopSpawner()
{
	if (Spawner != nullptr)
	{
		Spawner->StopSpawningEnemies();
		Spawner->StopSpawningHealthPickups();
		Spawner->StopSpawningAmmoPickups();
	}
}

void AHighScoreGameMode::StopAliveEnemies()
{
	for (AEnemyAIController* EnemyController : TActorRange<AEnemyAIController>(GetWorld()))
	{
		if (EnemyController != nullptr)
		{
			EnemyController->HandlePlayerOrOwnerDeath();
		}
	}
}

void AHighScoreGameMode::StopAliveEnemiesSounds()
{
	for (AEnemy* Enemy : TActorRange<AEnemy>(GetWorld()))
	{
		if (Enemy != nullptr && Enemy->IsAlive())
		{
			Enemy->StopSpawningAliveSounds();
		}
	}
}

void AHighScoreGameMode::EndGame(bool bIsWinner)
{
	ASoldierController* SoldierController = Cast<ASoldierController>(GetWorld()->GetFirstPlayerController());
	if (SoldierController != nullptr)
	{
		SoldierController->SetIsKillMode(true);
		SoldierController->GameHasEnded(SoldierController->GetPawn(), bIsWinner);
	}
}

int32 AHighScoreGameMode::GetEnemiesKilled() const 
{
	return EnemiesKilled;
}




