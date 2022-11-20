// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayableArea.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API APlayableArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayableArea();

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool PlayerInPlayableArea() const;

private:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;

	bool bInPlayableArea;

	FTimerHandle PlayerKillTimer;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float SecondsBeforePlayerKill;

	void HandlePlayerLeavesPlayableArea();
	void HandlePlayerEnterPlayableArea();

	void KillPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
