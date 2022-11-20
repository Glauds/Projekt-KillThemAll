// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"

#include "Enemy.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (Root != nullptr) { SetRootComponent(Root); }

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (Mesh != nullptr) { Mesh->SetupAttachment(Root); }
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	SetAmmo(StartingAmmo);
}

void AGun::SetAmmo(int32 AmmoToSet)
{
	if (AmmoToSet > 0) 
	{
		ActualAmmo = AmmoToSet;
	}
}

void AGun::Shoot()
{
	if (IsSingleShotPossible())
	{
		SingleShot();
	}

	AutoShot();
}

bool AGun::IsSingleShotPossible() 
{
	float ActualTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	return ActualTime - LastShotTime >= FireRate;
}

void AGun::SingleShot()
{
	ProcessShot();
}

void AGun::AutoShot()
{
	GetWorldTimerManager().SetTimer(FiringTimer, this, &AGun::ProcessShot, FireRate, true);
}

void AGun::StopShooting()
{
	GetWorldTimerManager().ClearTimer(FiringTimer);
}

void AGun::ProcessShot()
{
	if (IsMagazineEmpty())
	{
		HandleEmptyMagazine();
		return;
	}

	SetShotTime(); 
	SpawnMuzzleFX();
	ReduceAmountOfAmmo();

	FHitResult HitResult;
	bool bIsHit = WeaponTrace(HitResult);

	if (bIsHit)
	{
		ProccessHit(HitResult);
	}
}

void AGun::HandleEmptyMagazine()
{
	PlayGunEmptySound();
	GetWorldTimerManager().ClearTimer(FiringTimer);
}

void AGun::SetShotTime()
{
	LastShotTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

void AGun::SpawnMuzzleFX()
{
	if (MuzzleFlash != nullptr && MuzzleSound != nullptr) 
	{
		SpawnMuzzleVFX();
		//UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("Muzzle"));
	}
}

void AGun::ReduceAmountOfAmmo() 
{
	ActualAmmo = FMath::Clamp<int32>(ActualAmmo - 1, 0, MaxAmmo);
}

bool AGun::WeaponTrace(FHitResult& HitResult)
{
	if (Mesh != nullptr) 
	{
		FVector TraceStart = Mesh->GetSocketLocation(TEXT("Muzzle"));
		FVector Direction = GetOwner()->GetActorForwardVector();
		FVector TraceEnd = CalculateTraceEnd(TraceStart, Direction);
		
		if (bShowDebug)
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2);
		}

		return GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	}
	else 
	{
		return false;
	}
}

FVector AGun::CalculateTraceEnd(FVector TraceStart, FVector Direction)
{
	float X = FMath::Abs(Direction.X);
	if (X >= 0.45 && X <= 0.95)
	{
		float OffsetX = 0.15;

		if (Direction.Y > 0)
		{
			return TraceStart + (FVector(Direction.X - (OffsetX / 2), Direction.Y, Direction.Z) * WeaponRange);
		}
		else
		{
			return TraceStart + (FVector(Direction.X - OffsetX, Direction.Y, Direction.Z) * WeaponRange);
		}
	}
	else if (X >= 0 && X < 0.45)
	{
		float OffsetX = 0.1;

		if (Direction.Y > 0)
		{
			return TraceStart + (FVector(Direction.X - (OffsetX / 2), Direction.Y, Direction.Z) * WeaponRange);
		}
		else
		{
			return TraceStart + (FVector(Direction.X - OffsetX, Direction.Y, Direction.Z) * WeaponRange);
		}

	}
	else
	{
		return TraceStart + (FVector(Direction.X, Direction.Y, Direction.Z) * WeaponRange);
	}
}

void AGun::ProccessHit(FHitResult HitResult)
{
	if (bShowDebug) 
	{
		DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Orange, false, 5);
	}

	AEnemy* Enemy = Cast<AEnemy>(HitResult.GetActor());

	if (Enemy != nullptr)
	{
		SpawnEnemyHitFX(Enemy);
		UGameplayStatics::ApplyDamage(Enemy, HitDamage, GetInstigatorController(), this, nullptr);
	}
}

void AGun::SpawnEnemyHitFX(AEnemy* Enemy)
{
	if (BloodSplash != nullptr && EnemyHitSound != nullptr) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodSplash, Enemy->GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), EnemyHitSound, Enemy->GetActorLocation());
	}
}

int32 AGun::GetActualAmmo() const
{
	return ActualAmmo;
}

float AGun::GetAmmoPercent() const
{
	return UKismetMathLibrary::Conv_IntToFloat(ActualAmmo) / UKismetMathLibrary::Conv_IntToFloat(MaxAmmo);
}

bool AGun::IsAmmoFull() const
{
	return ActualAmmo >= MaxAmmo;
}

void AGun::RestoreAmmo()
{
	ActualAmmo = FMath::Clamp<int32>(ActualAmmo + AmmoPerPickup, 0, MaxAmmo);
}

bool AGun::IsMagazineEmpty() const 
{
	return ActualAmmo <= 0;
}

void AGun::PlayGunEmptySound() 
{
	if (GunEmptySound != nullptr) 
	{
		UGameplayStatics::SpawnSoundAttached(GunEmptySound, Mesh);
	}
}








