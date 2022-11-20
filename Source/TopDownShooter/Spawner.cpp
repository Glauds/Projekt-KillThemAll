// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"

#include "Enemy.h"
#include "EnemySpawnPoint.h"
#include "HealthPickup.h"
#include "HealthSpawnPoint.h"
#include "AmmoPickup.h"
#include "AmmoSpawnPoint.h"

#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	GetAllEnemySpawners();
	GetAllHealthSpawnPoints();
	GetAllAmmoSpawnPoints();
}

void ASpawner::GetAllEnemySpawners()
{
	if (EnemySpawnPointClass != nullptr) 
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemySpawnPointClass, EnemySpawnPoints);
	}
}

void ASpawner::StartSpawningEnemies(float SpawnRate)
{
	EnemySpawnRate = SpawnRate;
	GetWorldTimerManager().SetTimer(EnemySpawnerTimer, this, &ASpawner::SpawnEnemies, EnemySpawnRate, true);
}

void ASpawner::SpawnEnemies()
{
	for (AActor* EnemySpawnPoint : EnemySpawnPoints)
	{
		if (EnemySpawnPoint != nullptr)
		{
			SpawnEnemy(EnemySpawnPoint);
		}
	}
}

void ASpawner::SpawnEnemy(AActor* EnemySpawnPoint)
{
	if (EnemySpawnPoint != nullptr) 
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FVector Location = EnemySpawnPoint->GetActorLocation();
		FRotator Rotation = EnemySpawnPoint->GetActorRotation();

		int32 RandomIndex = FMath::RandHelper(EnemyClassess.Num());
		SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClassess[RandomIndex], Location, Rotation, SpawnParams);
	}
}

void ASpawner::StopSpawningEnemies()
{
	GetWorldTimerManager().ClearTimer(EnemySpawnerTimer);
}

void ASpawner::GetAllHealthSpawnPoints()
{
	if (HealthSpawnPointClass != nullptr) 
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), HealthSpawnPointClass, HealthSpawnPoints);
	}
}

void ASpawner::StartSpawningHealthPickups()
{
	GetWorldTimerManager().SetTimer(HealthPickupSpawnerTimer, this, &ASpawner::SpawnHealthPickups, HealthPickupSpawnRate, true);
}

void ASpawner::SpawnHealthPickups()
{
	for (AActor* HealthPickupSpawnPoint : HealthSpawnPoints) 
	{
		if (HealthPickupSpawnPoint != nullptr) 
		{
			SpawnHealthPickup(HealthPickupSpawnPoint);
		}
	}
}

void ASpawner::SpawnHealthPickup(AActor* HealthPickupSpawnPoint)
{
	if (HealthPickupSpawnPoint != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

		FVector Location = HealthPickupSpawnPoint->GetActorLocation();
		FRotator Rotation = HealthPickupSpawnPoint->GetActorRotation();
		SpawnedHealthPickup = GetWorld()->SpawnActor<AHealthPickup>(HealthPickupClass, Location, Rotation, SpawnParams);
	}
}

void ASpawner::StopSpawningHealthPickups()
{
	GetWorldTimerManager().ClearTimer(HealthPickupSpawnerTimer);
}

void ASpawner::GetAllAmmoSpawnPoints()
{
	if (AmmoSpawnPointClass != nullptr) 
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AmmoSpawnPointClass, AmmoSpawnPoints);
	}
}

void ASpawner::StartSpawningAmmoPickups()
{
	GetWorldTimerManager().SetTimer(AmmoPickupSpawnerTimer, this, &ASpawner::SpawnAmmoPickups, AmmoPickupSpawnRate, true);
}

void ASpawner::SpawnAmmoPickups()
{
	for (AActor* AmmoPickupSpawnPoint : AmmoSpawnPoints)
	{
		if (AmmoPickupSpawnPoint != nullptr)
		{
			SpawnAmmoPickup(AmmoPickupSpawnPoint);
		}
	}
}

void ASpawner::SpawnAmmoPickup(AActor* AmmoPickupSpawnPoint)
{
	if (AmmoPickupSpawnPoint != nullptr) 
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

		FVector Location = AmmoPickupSpawnPoint->GetActorLocation();
		FRotator Rotation = AmmoPickupSpawnPoint->GetActorRotation();
		SpawnedAmmoPickup = GetWorld()->SpawnActor<AAmmoPickup>(AmmoPickupClass, Location, Rotation, SpawnParams);
	}
}

void ASpawner::StopSpawningAmmoPickups()
{
	GetWorldTimerManager().ClearTimer(AmmoPickupSpawnerTimer);
}



