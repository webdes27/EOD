// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeComponent.h"
#include "EODCharacterBase.h"
#include "EODPlayerController.h"
#include "GameplaySkillBase.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"
#include "DynamicHUDWidget.h"
#include "DynamicSkillTreeWidget.h"
#include "SkillPointsInfoWidget.h"

USkillTreeComponent::USkillTreeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
}

void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void USkillTreeComponent::InitializeSkillTreeWidget()
{
	//~ @todo modularize the logic for widgets and local skill data

	AEODPlayerController* PC = Cast<AEODPlayerController>(GetOuter());
	UDynamicHUDWidget* HUDWidget = PC ? PC->GetHUDWidget() : nullptr;
	SkillTreeWidget = HUDWidget ? HUDWidget->GetSkillTreeWidget() : nullptr;

	UEODGameInstance* GI = Cast<UEODGameInstance>(PC->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;

	if (!SkillTreeWidget || !SaveGame)
	{
		return;
	}

	SkillPointsInfoWidget = SkillTreeWidget->GetSkillPointsInfoWidget();
	if (SaveGame)
	{
		SkillTreeWidget->InitializeSkillTreeLayout(this, SkillTreeLayoutTable, SaveGame->SkillTreeSlotsSaveData);
	}
	else
	{
		SkillTreeWidget->InitializeSkillTreeLayout(this, SkillTreeLayoutTable);
	}

	SkillTreeWidget->UpdateSkillSlots();

	SkillPointsInfoWidget = SkillTreeWidget->GetSkillPointsInfoWidget();
	if (!SkillPointsInfoWidget)
	{
		return;
	}

	SkillPointsAllocationInfo = SaveGame->SkillPointsAllocationInfo;
	if (SkillPointsAllocationInfo.AvailableSkillPoints + SkillPointsAllocationInfo.UsedSkillPoints < SkillPointsUnlockedByDefault)
	{
		SkillPointsAllocationInfo.AvailableSkillPoints = SkillPointsUnlockedByDefault - SkillPointsAllocationInfo.UsedSkillPoints;
	}

	SkillPointsInfoWidget->UpdateSkillPointAllocationText(SkillPointsAllocationInfo);

}

bool USkillTreeComponent::AttemptPointAllocationToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	FSkillTreeSlot* SkillTreeSlot = SkillSlotInfo;
	if (SkillTreeSlot == nullptr)
	{
		FString ContextString = FString("USkillTreeComponent::AttemptPointAllocationToSlot()");
		SkillTreeSlot = SkillTreeLayoutTable->FindRow<FSkillTreeSlot>(SkillGroup, ContextString);
	}	

	if (!CanAllocatePointToSlot(SkillGroup, SkillTreeSlot))
	{
		return false;
	}

	switch (SkillTreeSlot->Vocation)
	{
	case EVocations::Assassin:
		SkillPointsAllocationInfo.AssassinPoints += 1;
		break;
	case EVocations::Berserker:
		SkillPointsAllocationInfo.BerserkerPoints += 1;
		break;
	case EVocations::Cleric:
		SkillPointsAllocationInfo.ClericPoints += 1;
		break;
	case EVocations::Defender:
		SkillPointsAllocationInfo.DefenderPoints += 1;
		break;
	case EVocations::Sorcerer:
		SkillPointsAllocationInfo.SorcererPoints += 1;
		break;
	default:
		break;
	}	

	SkillPointsAllocationInfo.AvailableSkillPoints -= 1;
	SkillPointsAllocationInfo.UsedSkillPoints += 1;

	return true;
}

bool USkillTreeComponent::CanAllocatePointToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	// If there is no point available for allocation or if our skill tree reference table is missing or if SkillGroup is invalid
	if (SkillPointsAllocationInfo.AvailableSkillPoints == 0 || SkillTreeLayoutTable == nullptr || SkillGroup == NAME_None)
	{
		return false;
	}

	FSkillTreeSlot* SkillTreeSlot = SkillSlotInfo;
	if (SkillTreeSlot == nullptr)
	{
		FString ContextString = FString("USkillTreeComponent::CanAllocatePointToSlot()");
		SkillTreeSlot = SkillTreeLayoutTable->FindRow<FSkillTreeSlot>(SkillGroup, ContextString);
	}
	
	// If skil tree slot was not found
	if (SkillTreeSlot == nullptr)
	{
		return false;
	}

	int32 CurrentSkillGroupUpgrade = SkillTreeSlotsSaveData.Contains(SkillGroup) ? SkillTreeSlotsSaveData[SkillGroup].CurrentUpgrade : 0;
	// If the skill upgrade is already maxxed out
	if (CurrentSkillGroupUpgrade == SkillTreeSlot->MaxUpgrades)
	{
		return false;
	}

	int32 UsedSkillPointsRequired = SkillTreeSlot->MinimumPointsToUnlock + CurrentSkillGroupUpgrade * SkillTreeSlot->UpgradePointsGap;
	if (SkillPointsAllocationInfo.UsedSkillPoints < UsedSkillPointsRequired)
	{
		return false;
	}

	if (SkillTreeSlot->SkillRequiredToUnlock != NAME_None)
	{
		int32 UnlockSkillUpgradeLevel = SkillTreeSlotsSaveData.Contains(SkillTreeSlot->SkillRequiredToUnlock) ? SkillTreeSlotsSaveData[SkillTreeSlot->SkillRequiredToUnlock].CurrentUpgrade : 0;
		if (UnlockSkillUpgradeLevel == 0)
		{
			return false;
		}
	}

	return true;
}
