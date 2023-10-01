// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "Tank.h"
#include "TimerManager.h"
#include "ToonTanksPlayerController.h"
#include "Tower.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"


void AToonTanksGameMode::ActorDied(AActor* ActorDied)
{
	if (ActorDied == Tank)
	{
		Tank->MuteTankEngine();
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false, true);
			ToonTanksPlayerController->CurrentMouseCursor = EMouseCursor::Default;
		}
		GameOver(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(ActorDied))
	{
		DestroyedTower->HandleDestruction();
		
		if (GetEnemiesCount() == 0)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false, true);
			ToonTanksPlayerController->CurrentMouseCursor = EMouseCursor::Default;
			Tank->MuteTankEngine();
			GameOver(true);
		}
	}
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetInitialGameSettings();
	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();
	
	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false, false);
		ToonTanksPlayerController->CurrentMouseCursor = EMouseCursor::Crosshairs;
		
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true,
			true);
		
		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate,
			StartDelay,
			false);
	}
}

int AToonTanksGameMode::GetEnemiesCount() const
{
	TArray<AActor*> TowerActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), TowerActors);
	return TowerActors.Num();
}

void AToonTanksGameMode::SetInitialGameSettings()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetOverallScalabilityLevel(1);
	Settings->SetFullscreenMode(EWindowMode::Fullscreen);
	Settings->SetVSyncEnabled(true);
	Settings->SetScreenResolution(FIntPoint(1920, 1080));
	Settings->ApplySettings(true);
}
