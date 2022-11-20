// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "Soldier.h"
#include "EnemyAIController.h"
#include "TopDownShooterGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Animation/AnimInstance.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ASoldier>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player != nullptr) 
	{
		SetStartingHealth(MaxHealth);
		StartSpawningAliveSounds();
	} 
}

void AEnemy::SetStartingHealth(float HealthToSet)
{
	if (HealthToSet > 0) 
	{
		ActualHealth = HealthToSet;
	}
}

void AEnemy::StartSpawningAliveSounds()
{
	TimeBetweenAliveSFX = FMath::FRandRange(MinTimeBetweenAliveSFX, MaxTimeBetweenAliveSFX);

	GetWorldTimerManager().SetTimer(AliveSoundEffectTimer, this, &AEnemy::SpawnAliveSFX, TimeBetweenAliveSFX, true);
}

void AEnemy::SpawnAliveSFX()
{
	if (AliveSound != nullptr) 
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AliveSound, GetActorLocation());
	}
}

void AEnemy::StopSpawningAliveSounds()
{
	GetWorldTimerManager().ClearTimer(AliveSoundEffectTimer);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShowDebug)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * AttackRange * 2, FColor::Blue, false, 0.1);
	}

	if (CanAttack())
	{
		PlayAttackAnimation(); // Attack Animation notify Attack which couse Damage to Player if Hit was detected
	}
}

bool AEnemy::CanAttack() const
{
	return  Player != nullptr && Player->IsAlive() && IsAlive() && !IsAttacking() && IsWithingAttackRange();
}

bool AEnemy::IsWithingAttackRange() const
{
	return GetDistanceTo(Player) < AttackRange;
}

bool AEnemy::IsAttacking() const
{
	if (AttackMontage != nullptr) 
	{
		return GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage);
	}
	else  
	{ 
		return false; 
	}
}

void AEnemy::PlayAttackAnimation()
{
	if (AttackMontage != nullptr) 
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	} 
}

void AEnemy::Attack()
{
	// Protect when Enemy Die during Attack Animation 
	if (Player == nullptr || IsDead()) { return; }

	if (IsPlayerHitted())
	{
		UGameplayStatics::ApplyDamage(Player, AttackDamage, GetInstigatorController(), this, nullptr);
	}
}

bool AEnemy::IsPlayerHitted()
{
	FHitResult HitResult;
	bool bIsHit = AttackTrace(HitResult);
	
	if (bShowDebug) 
	{
		DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Orange, false, 5);
	}

	if (bIsHit)
	{
		if (Cast<ASoldier>(HitResult.GetActor()) == Player) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	return false;
}

bool AEnemy::AttackTrace(FHitResult& HitResult)
{
	FVector TraceStart = GetActorLocation();
	FVector Direction = GetActorForwardVector();
	FVector TraceEnd = TraceStart + Direction * AttackRange * 2; // Multiply by 2 bcs Trace Start from Enemy Actor center, not from hand etc 

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, Params);
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CalculatHealthWhenDamaged(DamageApplied);

	if (IsDead()) 
	{
		ProccessDeath();
	}

	return DamageApplied;
}

void AEnemy::CalculatHealthWhenDamaged(float DamageApplied)
{
	ActualHealth = FMath::Clamp<float>(ActualHealth - DamageApplied, 0, MaxHealth);
}
void AEnemy::ProccessDeath()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::DestroyActorAfterDelay, DestroyDelay, false);
	
	NotifyGameModeAboutDead();
	SpawnDeathFX();
	StopEnemy();
	StopAttackAnimation();
	StopSpawningAliveSounds();
	SetActorEnableCollision(false);
}

void AEnemy::SpawnDeathFX()
{
	if (Player != nullptr) 
	{
		if (Player->GetActiveGunIndex() == 0 && RifleDeathSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RifleDeathSound, GetActorLocation());
		}

		else if (Player->GetActiveGunIndex() == 1 && ShotgunDeathSound != nullptr) 
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShotgunDeathSound, GetActorLocation());
		}
	}
}

void AEnemy::StopEnemy() 
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController != nullptr)
	{
		AIController->HandlePlayerOrOwnerDeath();
	}
}

void AEnemy::DestroyActorAfterDelay()
{
	Destroy();
}

void AEnemy::StopAttackAnimation() 
{
	if (AttackMontage != nullptr) 
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0, AttackMontage);
	} 
}

bool AEnemy::IsDead() const
{
	return ActualHealth <= 0;
}

bool AEnemy::IsAlive() const 
{
	return ActualHealth > 0;
}

void AEnemy::NotifyGameModeAboutDead()
{
	ATopDownShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ATopDownShooterGameModeBase>();
	if (GameMode != nullptr)
	{
		GameMode->EnemyKilled();
	}
}



