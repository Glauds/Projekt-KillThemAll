// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AGun();

	void Shoot();
	void StopShooting();

	void RestoreAmmo();
	bool IsAmmoFull() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* Mesh;

	UFUNCTION(BlueprintImplementableEvent, Category = "VFX")
	void SpawnMuzzleVFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UParticleSystem* MuzzleFlash;

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float WeaponRange = 5000;

	FTimerHandle FiringTimer;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float FireRate = 0.08;

	float LastShotTime = 0;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float HitDamage = 25;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 MaxAmmo = 120;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 StartingAmmo = 30;

	UPROPERTY(VisibleAnywhere, Category = "Ammo")
	int32 ActualAmmo;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 AmmoPerPickup = 30;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* MuzzleSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* EnemyHitSound;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* BloodSplash;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* GunEmptySound;

	bool IsSingleShotPossible();

	void SingleShot();
	void AutoShot();

	void ProcessShot();
	void SetShotTime();
	void SpawnMuzzleFX();

	bool WeaponTrace(FHitResult& HitResult);
	FVector CalculateTraceEnd(FVector TraceStart, FVector Direction);
	void ProccessHit(FHitResult HitResult);
	void SpawnEnemyHitFX(class AEnemy* Enemy);

	void SetAmmo(int32 AmmoToSet);
	void ReduceAmountOfAmmo();

	UFUNCTION(BlueprintPure, Category = "Ammo")
	int32 GetActualAmmo() const;

	UFUNCTION(BlueprintPure, Category = "Ammo")
	float GetAmmoPercent() const;

	bool IsMagazineEmpty() const;
	void PlayGunEmptySound();
	void HandleEmptyMagazine();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bShowDebug = false;

};
