// Fill out your copyright notice in the Description page of Project Settings.

#include "EODGameModeBase.h"
#include "EODPreprocessors.h"
#include "EODSaveGame.h"
#include "GameSingleton.h"
#include "StatusEffectsManager.h"

#include "EODPlayerController.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AEODGameModeBase::AEODGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AEODGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UGameSingleton* GameSingleton = IsValid(GEngine) ? Cast<UGameSingleton>(GEngine->GameSingleton) : nullptr;
	if (!IsValid(GameSingleton))
	{
		return;
	}

	/*
	UEODSaveGame* EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSingleton->DefaultSaveSlotName, GameSingleton->UserIndex));
	// If the default save game profile doesn't exist then create it
	if (!IsValid(EODSaveGame))
	{
		EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::CreateSaveGameObject(UEODSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(EODSaveGame, GameSingleton->DefaultSaveSlotName, GameSingleton->UserIndex);
	}

	if (IsValid(GetWorld()))
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		// We don't want status effects manager or combat manager to be saved into map
		SpawnInfo.ObjectFlags |= RF_Transient;
		StatusEffectsManager = GetWorld()->SpawnActor<AStatusEffectsManager>(AStatusEffectsManager::StaticClass(), SpawnInfo);
	}
	*/
}

UClass* AEODGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	AEODPlayerController* PC = Cast<AEODPlayerController>(InController);
	if (PC)
	{
		if (PC->GetGender() == ECharacterGender::Female)
		{
			return FemalePawnClass;
		}
		else
		{
			return MalePawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

AStatusEffectsManager* AEODGameModeBase::BP_GetStatusEffectsManager() const
{
	return GetStatusEffectsManager();
}