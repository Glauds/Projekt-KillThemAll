// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownShooterGameModeBase.h"
#include "HighScoreGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AHighScoreGameMode : public ATopDownShooterGameModeBase
{
	GENERATED_BODY()

public:

	virtual void PlayerKilled() override;
	virtual void EnemyKilled() override;


	UFUNCTION(BlueprintPure, Category = "Gameplay")
	int32 GetEnemiesKilled() const;

private:

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<class ASpawner> SpawnerClass;

	UPROPERTY()
	ASpawner* Spawner;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int32 EnemiesKilled = 0;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float WaveTime = 60;

	FTimerHandle WaveTimer;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float EnemySpawnRateInWave = 15;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float MinEnemySpawnRateInWave = 10;

	void SetupSpawner();
	void StartSpawner();

	void StartWaveTimer();
	void SpawnWave();
	void StopWaveTimer();

	void HandlePlayerDeath();
	void StopSpawner();
	void StopAliveEnemies();
	void StopAliveEnemiesSounds();

	void EndGame(bool bIsWinner);

protected:

	virtual void StartPlay() override;
};
