// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AKillEmAllGameMode : public ATopDownShooterGameModeBase
{
	GENERATED_BODY()

public:

	virtual void PlayerKilled() override;

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	float GetGameTime() const;

private:

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<class ASpawner> SpawnerClass;

	UPROPERTY()
	ASpawner* Spawner;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float GameTime = 10;

	FTimerHandle GameTimer;
	FTimerHandle CheckForAliveEnemiesTimer;

	void SetupSpawner();
	void StartGameTimer();
	void StartSpawner();
	void DecreaseGameTime();
	
	void HandleGameTimerOver();
	void StopSpawner();
	void StopSpawningEnemies();
	void StopGameTimer();

	void CheckForAliveEnemies();
	bool AllEnemiesAreDead() const;
	void StopSpawningPickups();
	void HandlePlayerWin();

	void HandlePlayerDeath();
	void StopAliveEnemies();
	void StopAliveEnemiesSounds();

	void EndGame(bool bIsWinner);

protected:

	virtual void StartPlay() override;
};

