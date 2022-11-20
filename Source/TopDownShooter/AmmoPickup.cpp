// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"

#include "Soldier.h"
#include "Gun.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	if (Box != nullptr) { SetRootComponent(Box); }

	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	if (Top != nullptr) { Top->SetupAttachment(Box); }

	Down = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	if (Top != nullptr) { Down->SetupAttachment(Top); }
}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();

	if (Box != nullptr) 
	{
		Box->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnBoxBeginOverlap);
	}
}

void AAmmoPickup::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASoldier* Soldier = Cast<ASoldier>(OtherActor);
	if (Soldier != nullptr) 
	{
		TArray<AGun*> SpawnedGuns = Soldier->GetSpawnedGuns();
		for (AGun* SpawnedGun : SpawnedGuns)
		{
			if (SpawnedGun != nullptr && !SpawnedGun->IsAmmoFull())
			{
				HandlePickup(SpawnedGun);
			}
		}
	}

	if (bWasPicked && PickupSound != nullptr) 
	{
		UGameplayStatics::SpawnSoundAttached(PickupSound, Top);
		Destroy();
	}
}

void AAmmoPickup::HandlePickup(AGun* Gun)
{
	if (Gun != nullptr) 
	{
		Gun->RestoreAmmo();
		SetPicked();
	}
}

void AAmmoPickup::SetPicked()
{
	bWasPicked = true;
}

