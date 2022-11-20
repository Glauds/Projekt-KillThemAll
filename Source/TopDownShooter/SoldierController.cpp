// Fill out your copyright notice in the Description page of Project Settings.

#include "SoldierController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ASoldierController::ASoldierController() 
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Game Music"));
}

 void ASoldierController::BeginPlay() 
{
	 Super::BeginPlay();

	 SetupHUD();
	 StartGameMusic();
 }

 void ASoldierController::SetupHUD()
 {
	 if (HUDClass != nullptr) 
	 {
		 HUD = CreateWidget(this, HUDClass);
		 
		 if (HUD != nullptr)
		 {
			 HUD->AddToViewport();
		 }
	 }
 }

 void ASoldierController::StartGameMusic()
 {
	 if (AudioComponent != nullptr)
	 {
		 //AudioComponent->Play();
	 }
 }

 void ASoldierController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
 {
	 Super::GameHasEnded(EndGameFocus, bIsWinner);

	 SetGameIsOver(true);
	 StopShowingHUD();
	 StopGameMusic();

	 if (bIsWinner)
	 {
		 HandleWin();
	 }
	 else 
	 {
		 HandleLose();
	 }
 }

 void ASoldierController::StopShowingHUD()
 {
	 if (HUD != nullptr) 
	 {
		 HUD->RemoveFromViewport();
	 }
 }

 void ASoldierController::StopGameMusic() 
 {
	 if (AudioComponent != nullptr)
	 {
		 //AudioComponent->Stop();
	 }
 }

 void ASoldierController::HandleWin() 
 {
	 SpawnWinFX();
	 ShowWinScreen();
 }

 void ASoldierController::SpawnWinFX()
 {
	 if (WinSound != nullptr) 
	 {
		UGameplayStatics::SpawnSound2D(GetWorld(), WinSound);
	 }
 }

 void ASoldierController::ShowWinScreen() 
 {
	 if (WinScreenClass != nullptr) 
	 {
		 WinScreen = CreateWidget(this, WinScreenClass);

		 if (WinScreen != nullptr)
		 {
			 WinScreen->AddToViewport();
		 }
	 }
 }

 void ASoldierController::HandleLose() 
 {
	 SpawnLoseFX();

	 if (isKillMode)
	 {
		ShowGameOverScreen();
	 }
	 else 
	 {
		ShowLoseScreen();
	 }
 }

 void  ASoldierController::SpawnLoseFX() 
 {
	 if (LoseSound != nullptr) 
	 {
		UGameplayStatics::SpawnSound2D(GetWorld(), LoseSound);
	 }
 }

 void ASoldierController::ShowGameOverScreen()
 {
	 if (GameOverScreenClass != nullptr)
	 {
		 GameOverScreen = CreateWidget(this, GameOverScreenClass);

		 if (GameOverScreen != nullptr)
		 {
			 GameOverScreen->AddToViewport();
		 }
	 }
 }

 void ASoldierController::ShowLoseScreen()
 {
	 if (LoseScreenClass != nullptr) 
	 {
		 LoseScreen = CreateWidget(this, LoseScreenClass);

		 if (LoseScreen != nullptr)
		 {
			 LoseScreen->AddToViewport();
		 }
	 }
 }

 void ASoldierController::BackToMainMenu()
 {
	 if (IsGameOver())
	 {
		 UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
	 }
 }

 void ASoldierController::SetGameIsOver(bool IsGameOver)
 {
	 bIsGameOver = IsGameOver;
 }

 bool ASoldierController::IsGameOver() const
 {
	 return bIsGameOver;
 }

void ASoldierController::SetIsKillMode(bool ModeToSet)
{
	isKillMode = ModeToSet;
}






