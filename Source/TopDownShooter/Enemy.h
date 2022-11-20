// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	bool IsDead() const;

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	bool IsAlive() const;

	void StopSpawningAliveSounds();

private:

	UPROPERTY()
	class ASoldier* Player;

	UPROPERTY(EditAnywhere, Category = "Attack")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRange = 100;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDamage = 10;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;

	float ActualHealth;

	UPROPERTY(EditAnywhere, Category = "Death")
	float DestroyDelay = 5;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* RifleDeathSound;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* ShotgunDeathSound;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float MinTimeBetweenAliveSFX = 5;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float MaxTimeBetweenAliveSFX = 10;

	float TimeBetweenAliveSFX;

	FTimerHandle AliveSoundEffectTimer;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* AliveSound;

	void SetStartingHealth(float HealthToSet);

	bool CanAttack() const;
	bool IsWithingAttackRange() const;
	bool IsAttacking() const;
	bool IsPlayerHitted();

	bool AttackTrace(FHitResult& HitResult);

	void CalculatHealthWhenDamaged(float DamageApplied);

	void ProccessDeath();
	void SpawnDeathFX();
	void StopEnemy();
	void DestroyActorAfterDelay();

	void PlayAttackAnimation();
	void StopAttackAnimation();

	void StartSpawningAliveSounds();
	void SpawnAliveSFX();

	void NotifyGameModeAboutDead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bShowDebug = false;

};
