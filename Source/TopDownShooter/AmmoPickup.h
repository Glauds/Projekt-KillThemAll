// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Top;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Down;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* PickupSound;

	void HandlePickup(class AGun* Gun);

	void SetPicked();
	bool bWasPicked = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
