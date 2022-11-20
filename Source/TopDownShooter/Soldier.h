// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soldier.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API ASoldier : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldier();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	bool IsDead() const;

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	bool IsAlive() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealtAmmount() const;

	UFUNCTION(BlueprintPure, Category = "Gun")
	class AGun* GetGun() const;

	TArray<class AGun*> GetSpawnedGuns() const;

	UFUNCTION(BlueprintPure, Category = "Gun")
	int32 GetActiveGunIndex() const;

	void RestoreHealth(float HealthToRestore);

	bool IsHealthFull() const;

	UFUNCTION(BlueprintCallable, Category = "Shooting")
	void StopFiringWeapon();

private:

	UPROPERTY()
	class ASoldierController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TArray<TSubclassOf<class AGun>> GunClassess;

	TArray<AGun*> SpawnedGuns;

	UPROPERTY()
	AGun* SpawnedGun;

	UPROPERTY()
	AGun* ActiveGun;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* GunChangeSound;

	UPROPERTY(EditAnywhere, Category = "Gun")
	int32 ActiveGunIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float ActualHealth;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* HitEffect;

	void SetPlayerController();
	void SpawnGuns();
	void SetStartinGun();
	void HideOtherGuns();
	void SetStartingHealth(float HealthToSet);

	void MoveForward(float AvisValue);
	void MoveRight(float AxisValue);
	void Rotate(float DeltaTime);

	void SetActualGunToWeapon1();
	void SetActualGunToWeapon2();
	void ActivateGun(int32 GunToActivateIndex);
	void SpawnGunChangeFX();

	void StartFiringWeapon();

	void SpawnHitFX();
	void CalculatHealthWhenDamaged(float DamageApplied);

	void ProccessDeath();
	void SpawnDeathFX();
	void NotifyGameModeAboutDead();

	void BackToMainMenu();
	 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
