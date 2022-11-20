// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	void SpawnEnemies();
	void StartSpawningEnemies(float SpawnRate = 15);
	void StopSpawningEnemies();

	void StartSpawningHealthPickups();
	void StopSpawningHealthPickups();

	void StartSpawningAmmoPickups();
	void StopSpawningAmmoPickups();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<class AEnemySpawnPoint> EnemySpawnPointClass;

	TArray<AActor*> EnemySpawnPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TArray<TSubclassOf<class AEnemy>> EnemyClassess;

	UPROPERTY()
	AEnemy* SpawnedEnemy;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float EnemySpawnRate = 10;

	FTimerHandle EnemySpawnerTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	TSubclassOf<class AHealthSpawnPoint> HealthSpawnPointClass;

	TArray<AActor*> HealthSpawnPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	TSubclassOf<class AHealthPickup> HealthPickupClass;

	UPROPERTY()
	AHealthPickup* SpawnedHealthPickup;

	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthPickupSpawnRate = 10;

	FTimerHandle HealthPickupSpawnerTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	TSubclassOf<class AAmmoSpawnPoint> AmmoSpawnPointClass;

	TArray<AActor*> AmmoSpawnPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	TSubclassOf<class AAmmoPickup> AmmoPickupClass;

	UPROPERTY()
	AAmmoPickup* SpawnedAmmoPickup;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	float AmmoPickupSpawnRate = 10;

	FTimerHandle AmmoPickupSpawnerTimer;

	void GetAllEnemySpawners();
	void SpawnEnemy(AActor* EnemySpawnPoint);

	void GetAllHealthSpawnPoints();
	void SpawnHealthPickups();
	void SpawnHealthPickup(AActor* HealthPickupSpawnPoint);

	void GetAllAmmoSpawnPoints();
	void SpawnAmmoPickups();
	void SpawnAmmoPickup(AActor* AmmoPickupSpawnPoint);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
