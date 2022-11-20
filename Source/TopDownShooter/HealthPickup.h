// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickup.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickup();

private:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Top;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Down;

	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthPerPickup = 15;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* PickupSound;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void HandlePickup(class ASoldier* Soldier);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
