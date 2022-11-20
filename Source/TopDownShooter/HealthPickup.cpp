// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPickup.h"

#include "Soldier.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	if (Box != nullptr) { SetRootComponent(Box); }

	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	if (Top != nullptr) { Top->SetupAttachment(Box); }

	Down = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	if (Down != nullptr) { Down->SetupAttachment(Top); }
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

	if (Box != nullptr) 
	{
		Box->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::OnBoxBeginOverlap);

	}
}

void AHealthPickup::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASoldier* Soldier = Cast<ASoldier>(OtherActor);
	if (Soldier != nullptr && !Soldier->IsHealthFull())
	{
		HandlePickup(Soldier);
	} 
}

void AHealthPickup::HandlePickup(ASoldier* Soldier)
{
	if (Soldier != nullptr && PickupSound != nullptr) 
	{
		UGameplayStatics::SpawnSoundAttached(PickupSound, Top);
		Soldier->RestoreHealth(HealthPerPickup);
		Destroy();
	}
}



