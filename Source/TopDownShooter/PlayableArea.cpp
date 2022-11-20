// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayableArea.h"

#include "Soldier.h"
#include "SoldierController.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayableArea::APlayableArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	if(Box != nullptr) { SetRootComponent(Box); }
}

// Called when the game starts or when spawned
void APlayableArea::BeginPlay()
{
	Super::BeginPlay();

	if (Box != nullptr) 
	{
		bInPlayableArea = true;

		Box->OnComponentEndOverlap.AddDynamic(this, &APlayableArea::OnBoxEndOverlap);
		Box->OnComponentBeginOverlap.AddDynamic(this, &APlayableArea::OnBoxBeginOverlap);
	}
}

void APlayableArea::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASoldier* Soldier = Cast<ASoldier>(OtherActor);
	if (Soldier != nullptr) 
	{
		ASoldierController* Controller = Cast<ASoldierController>(Soldier->GetController());
		if (Controller != nullptr) 
		{
			if (!Controller->IsGameOver() && bInPlayableArea) 
			{
				HandlePlayerLeavesPlayableArea();
			}
		}
	}
}

void APlayableArea::HandlePlayerLeavesPlayableArea() 
{
	bInPlayableArea = false;
	GetWorldTimerManager().SetTimer(PlayerKillTimer, this, &APlayableArea::KillPlayer, SecondsBeforePlayerKill, false);
}

void APlayableArea::KillPlayer()
{
	ASoldier* Soldier = Cast<ASoldier>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)); 

	if(Soldier != nullptr)
	{
		ASoldierController* Controller = Cast<ASoldierController>(Soldier->GetController());
		if (Controller != nullptr && !Controller->IsGameOver())
		{
			UGameplayStatics::ApplyDamage(Soldier, Soldier->GetHealtAmmount(), Controller, this, nullptr);
		}
	}
}

void APlayableArea::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASoldier* Soldier = Cast<ASoldier>(OtherActor);

	if (Soldier != nullptr && !bInPlayableArea)
	{
		HandlePlayerEnterPlayableArea();
	}
}

void APlayableArea::HandlePlayerEnterPlayableArea()
{
	bInPlayableArea = true;
	GetWorldTimerManager().ClearTimer(PlayerKillTimer);
}

bool APlayableArea::PlayerInPlayableArea() const 
{
	return bInPlayableArea;
}




