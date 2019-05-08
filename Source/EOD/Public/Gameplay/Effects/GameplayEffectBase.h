// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "UObject/NoExportTypes.h"
#include "GameplayEffectBase.generated.h"

class AEODCharacterBase;
class UGameplaySkillsComponent;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EOD_API UGameplayEffectBase : public UObject
{
	GENERATED_BODY()

public:

	UGameplayEffectBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Effect Interface
	// --------------------------------------

	virtual void InitEffect(AEODCharacterBase* Instigator, TArray<AEODCharacterBase*> Targets);

	virtual void ActivateEffect();

	virtual void DeactivateEffect();

	virtual void UpdateEffect(float DeltaTime);

	FORCEINLINE bool IsActive() const { return bActive; }

protected:

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------
	
	/** Name of this gameplay effect that will be displayed to player inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-Game Information")
	FString InGameName;

	/** Description of this gameplay effect that will be displayed to player inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-Game Information")
	FString Description;
	
	/** Icon used to represent this gameplay effect inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-Game Information")
	UTexture* Icon;
	
	/** The duration for which this effect will last */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	float GameplayEffectDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	EGameplayEffectAuthority GameplayEffectAuthority;

	// --------------------------------------
	//	Cache
	// --------------------------------------

	/** This effect's instigator */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Cache")
	TWeakObjectPtr<AEODCharacterBase> EffectInstigator;

	/** Skill component of this effect's instigator */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Cache")
	TWeakObjectPtr<UGameplaySkillsComponent> InstigatorSkillComponent;

	/** This effect's targets */
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AEODCharacterBase>> EffectTargets;

	// TWeakObjectPtr<AEODCharacterBase> EffectTarget;

	/** Skill component of this effect's target */
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UGameplaySkillsComponent>> TargetSkillComponents;

	// TWeakObjectPtr<UGameplaySkillsComponent> TargetSkillComponent;

	/** Determines if this gameplay effect is currently active */
	UPROPERTY(Transient)
	bool bActive;

	
};
