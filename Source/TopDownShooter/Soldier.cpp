// Fill out your copyright notice in the Description page of Project Settings.

#include "Soldier.h"

#include "Gun.h"
#include "SoldierController.h"
#include "TopDownShooterGameModeBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"

// Sets default values
ASoldier::ASoldier()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASoldier::BeginPlay()
{
	Super::BeginPlay();
	
	SetPlayerController();
	SpawnGuns();
	SetStartinGun();
	HideOtherGuns();
	SetStartingHealth(MaxHealth);
}

void ASoldier::SetPlayerController() 
{
	PlayerController = Cast<ASoldierController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController != nullptr) 
	{
		PlayerController->bShowMouseCursor = true;
	} 
}

void ASoldier::SpawnGuns()
{
	for (int32 i = 0; i < GunClassess.Num(); i++) 
	{
		if (GunClassess[i] != nullptr) 
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SpawnedGun = GetWorld()->SpawnActor<AGun>(GunClassess[i], SpawnParams);
			if (SpawnedGun != nullptr) 
			{
				SpawnedGuns.Add(SpawnedGun);
				SpawnedGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponPoint"));
				SpawnedGun->SetOwner(this);
			}
		}
	}
}

void ASoldier::SetStartinGun()
{
	if (SpawnedGuns[ActiveGunIndex] != nullptr) 
	{
		ActiveGun = SpawnedGuns[ActiveGunIndex];
	}
}

void ASoldier::HideOtherGuns()
{
	for (int32 i = 0; i < SpawnedGuns.Num(); i++)
	{
		if (SpawnedGuns[i] != nullptr)
		{
			if (i != ActiveGunIndex)
			{
				SpawnedGuns[i]->SetActorHiddenInGame(true);
			}
		}
	}
}

void ASoldier::SetStartingHealth(float HealthToSet) 
{
	if (HealthToSet > 0) 
	{
		ActualHealth = HealthToSet;
	}
}

void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASoldier::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASoldier::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ASoldier::StartFiringWeapon);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &ASoldier::StopFiringWeapon);
	
	PlayerInputComponent->BindAction(TEXT("Weapon1"), EInputEvent::IE_Pressed, this, &ASoldier::SetActualGunToWeapon1);
	PlayerInputComponent->BindAction(TEXT("Weapon2"), EInputEvent::IE_Pressed, this, &ASoldier::SetActualGunToWeapon2);

	PlayerInputComponent->BindAction(TEXT("BackToMenu"), EInputEvent::IE_Pressed, this, &ASoldier::BackToMainMenu);
}

void ASoldier::MoveForward(float AxisValue)
{
	AddMovementInput(FVector::ForwardVector, AxisValue);
}

void ASoldier::MoveRight(float AxisValue)
{
	AddMovementInput(FVector::RightVector,  AxisValue);
}

void ASoldier::ActivateGun(int32 GunToActivateIndex)
{
	ActiveGunIndex = GunToActivateIndex;

	if (SpawnedGuns[ActiveGunIndex] != nullptr)
	{
		ActiveGun = SpawnedGuns[ActiveGunIndex];
		ActiveGun->SetActorHiddenInGame(false);
	}
}

void ASoldier::SetActualGunToWeapon1()
{
	if (IsAlive()) 
	{
		int32 GunToActivateIndex = 0;

		StopFiringWeapon();
		ActivateGun(GunToActivateIndex);
		SpawnGunChangeFX();
		HideOtherGuns();
	}
}

void ASoldier::SetActualGunToWeapon2()
{
	if (IsAlive()) 
	{
		int32 GunToActivateIndex = 1;

		StopFiringWeapon();
		ActivateGun(GunToActivateIndex);
		SpawnGunChangeFX();
		HideOtherGuns();
	}
}

void ASoldier::StartFiringWeapon()
{
	if (ActiveGun != nullptr && IsAlive())
	{
		ActiveGun->Shoot();
	}
}

void ASoldier::StopFiringWeapon()
{
	if (ActiveGun != nullptr)
	{
		ActiveGun->StopShooting();
	}
}

void ASoldier::BackToMainMenu()
{
	if (PlayerController != nullptr)
	{
		PlayerController->BackToMainMenu();
	}
}

// Called every frame
void ASoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive()) 
	{
		Rotate(DeltaTime);
	}
}

void ASoldier::Rotate(float DeltaTime)
{
	if (PlayerController != nullptr) 
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, HitResult);

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.Location);
		FRotator RotationToSet = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 33);

		PlayerController->SetControlRotation(RotationToSet);
	}
}

float ASoldier::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CalculatHealthWhenDamaged(DamageApplied);
	SpawnHitFX();

	if (IsDead())
	{
		ProccessDeath();
	}

	return DamageApplied;
}

void ASoldier::SpawnHitFX()
{
	if (HitSound != nullptr && HitEffect != nullptr) 
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
	}
}

void ASoldier::CalculatHealthWhenDamaged(float DamageApplied)
{
	ActualHealth = FMath::Clamp<float>(ActualHealth - DamageApplied, 0, MaxHealth);
}

void ASoldier::ProccessDeath()
{
	SpawnDeathFX();
	StopFiringWeapon();
	SetActorEnableCollision(false);
	NotifyGameModeAboutDead();
}

void ASoldier::SpawnDeathFX()
{
	if (DeathSound != nullptr) 
	{
		UGameplayStatics::SpawnSoundAttached(DeathSound, GetMesh());
	}
}

void ASoldier::NotifyGameModeAboutDead()
{
	ATopDownShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ATopDownShooterGameModeBase>();
	if (GameMode != nullptr)
	{
		GameMode->PlayerKilled();
	}
}

bool ASoldier::IsDead() const
{
	return ActualHealth <= 0;
}

bool ASoldier::IsAlive() const 
{
	return ActualHealth > 0;
}

float ASoldier::GetHealthPercent() const 
{
	return ActualHealth / MaxHealth;
}

float ASoldier::GetHealtAmmount() const
{
	return ActualHealth;
}

void ASoldier::RestoreHealth(float HealthToRestore)
{
	ActualHealth = FMath::Clamp<float>(ActualHealth + HealthToRestore, 0, MaxHealth);
}

bool ASoldier::IsHealthFull() const
{
	return ActualHealth >= MaxHealth;
}

AGun* ASoldier::GetGun() const
{
	if (ActiveGun != nullptr) 
	{
		return ActiveGun;
	}
	else
	{
		return nullptr;
	}
}

TArray<class AGun*> ASoldier::GetSpawnedGuns() const
{
	return SpawnedGuns;
}

void ASoldier::SpawnGunChangeFX()
{
	if (GunChangeSound != nullptr && ActiveGun != nullptr) 
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GunChangeSound, ActiveGun->GetActorLocation());
	}
}

int32 ASoldier::GetActiveGunIndex() const 
{
	return ActiveGunIndex;
}







