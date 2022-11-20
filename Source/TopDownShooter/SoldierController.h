// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoldierController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ASoldierController : public APlayerController
{
	GENERATED_BODY()

public:
	ASoldierController();

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	void BackToMainMenu();

	bool IsGameOver() const;

	void SetIsKillMode(bool ModeToSet);

private:

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY()
	class UUserWidget* HUD;

	UPROPERTY(EditAnywhere, Category = "End Game")
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY()
	class UUserWidget* WinScreen;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* WinSound;

	UPROPERTY(EditAnywhere, Category = "End Game")
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* LoseSound;

	UPROPERTY()
	class UUserWidget* LoseScreen;

	UPROPERTY(EditAnywhere, Category = "End Game")
	TSubclassOf<class UUserWidget> GameOverScreenClass;

	UPROPERTY()
	class UUserWidget* GameOverScreen;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent;

	bool bIsGameOver = false;
	bool isKillMode = false;

	void SetupHUD();
	void StopShowingHUD();
	void StartGameMusic();

	void HandleWin();
	void ShowWinScreen();
	void SpawnWinFX();

	void HandleLose();
	void ShowLoseScreen();
	void ShowGameOverScreen();
	void SpawnLoseFX();

	void SetGameIsOver(bool IsGameOver);
	void StopGameMusic();

protected:

	virtual void BeginPlay() override;
};
