// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponLibrary.h"
#include "CombatLibrary.h"

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "CharacterLibrary.generated.h"

class UGameplaySkillBase;
class UAnimMontage;

/**
 * This enum describes the character movement direction relative to character's line of sight.
 * e.g, If the character is moving to the left of the direction it is looking in then ECharMovementDirection would be Left.
 */
UENUM(BlueprintType)
enum class ECharMovementDirection : uint8
{
	None,
	F 		UMETA(DisplayName = "Forward"),
	B 		UMETA(DisplayName = "Backward"),
	L 		UMETA(DisplayName = "Left"),
	R 		UMETA(DisplayName = "Right"),
	FL 		UMETA(DisplayName = "Forward Left"),
	FR 		UMETA(DisplayName = "Forward Right"),
	BL 		UMETA(DisplayName = "Backward Left"),
	BR 		UMETA(DisplayName = "Backward Right"),
};

/** This enum describes the condition for a gameplay effect from skill to activate */
UENUM(BlueprintType)
enum class EGameplayEffectActivationCondition : uint8
{
	ActivatesOnSkillTrigger,
	ActivatesOnSkillRelease,
	ActivatesOnSkillCancel,
	ActivatesOnSkillFinish,
	ActivatesOnSuccessfulAttack,
	ActivatesOnUnsuccessfulAttack,
	ActivatesOnGettingHit,
};

UENUM(BlueprintType)
enum class ESkillEventTriggerCondition : uint8
{
	TriggersOnSkillTrigger,
	TriggersOnSkillRelease,
	TriggersOnSkillCancel,
	TriggersOnSkillFinish,
	TriggersOnSkillHitSuccess,
	TriggersOnSkillHitFailure
};

UENUM(BlueprintType)
enum class EGameplayEffectAuthority : uint8
{
	Server,
	ClientOwner,
	None
};

/** This enum describes the cause of character death */
UENUM(BlueprintType)
enum class ECauseOfDeath : uint8
{
	ZeroHP,
	KilledByEnemy,
	KilledByEnvironment,
	Suicide,
	FellOutOfWorld,
};

/** This enum descibes the faction of in-game character */
UENUM(BlueprintType)
enum class EFaction : uint8
{
	Rendel_Commoner, // People of rendel kingdom
	Rendel_Soldier,
	Unknown,
	Undead,
	Corrupted,
	Player,
};

/** Species of in-game character */
UENUM(BlueprintType)
enum class ESpecies : uint8
{
	Human,
	Goblin,
	Ghoul,
	Skeleton,
	Yeti,
	Troll,
	Ogre,
};

/**
 * This enum describes the gender of human character
 * Mainly used to determine the armor mesh and animations to use
 */
UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
	Female,
	Male
};

/** This enum describes the current action/state of character */
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	IdleWalkRun,
	SwitchingWeapon,
	Jumping,
	Dodging,
	Blocking,
	Attacking,
	Looting,
	Interacting, 			// Interacting with another character, i.e., engaged in dialogue
	UsingActiveSkill,
	CastingSpell,
	GotHit,
	Dead
};

/** This enum describes the effect of this skill */
UENUM(BlueprintType)
enum class ESkillEffect : uint8
{
	DamageMelee,
	DamageRanged,
	HealSelf,
	HealParty,
	BuffSelf,
	BuffParty,
	DebuffEnemy
};

UENUM(BlueprintType)
enum class EVocations : uint8
{
	Assassin,
	Berserker,
	Cleric,
	Defender,
	Sorcerer
};

UENUM(BlueprintType)
enum class ESkillSlotStatus : uint8
{
	Blocked,
	Unblocked,
	Available,
};

UENUM(BlueprintType)
enum class EGameplayEventClassType : uint8
{
	GameplayEffect,
	None,
};


USTRUCT(BlueprintType)
struct EOD_API FGameplayEventInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	TArray<AActor*> Targets;

	UPROPERTY()
	UClass* EventClass;

	UPROPERTY()
	EGameplayEventClassType EventClassType;

	UPROPERTY()
	int32 EventSubIndex;

	UPROPERTY()
	bool bDetermineTargetsDynamically;

	FGameplayEventInfo() :
		Instigator(nullptr),
		EventClass(nullptr),
		EventClassType(EGameplayEventClassType::None),
		bDetermineTargetsDynamically(false)
	{
	}
};


/** Struct containing information regarding last used character skill */
USTRUCT(BlueprintType)
struct EOD_API FLastUsedSkillInfo
{
	GENERATED_USTRUCT_BODY()

	/** SkillID of last used skill */
	UPROPERTY(Transient, BlueprintReadOnly, Category = LastUsedSkill)
	FName LastUsedSkillID;

	/** True if the last skill was interrupted */
	UPROPERTY(Transient, BlueprintReadOnly, Category = LastUsedSkill)
	bool bInterrupted;

	FLastUsedSkillInfo()
	{
		bInterrupted = false;
		LastUsedSkillID = NAME_None;
	}

	FLastUsedSkillInfo(FName SkillID, bool bWasInterrupted) :
		LastUsedSkillID(SkillID),
		bInterrupted(bWasInterrupted)
	{
	}
};

/** A struct containing current skill state */
USTRUCT(BlueprintType)
struct EOD_API FSkillState
{
	GENERATED_USTRUCT_BODY()

	/** The current level that this skill has been upgraded to */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Skill State")
	int32 CurrentUpgradeLevel;

	/** The maximum level that this skill can be upgraded to */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Skill State")
	int32 MaxUpgradeLevel;

	/** Determines whether this skill has been unlocked on the skill tree */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Skill State")
	bool bUnlocked;

	FSkillState()
	{
		CurrentUpgradeLevel = 0;
		MaxUpgradeLevel = 1;
		bUnlocked = false;
	}
};

/**
 * This struct contains the references for player animations based on equipped weapon type
 * and player gender
 */
USTRUCT(BlueprintType)
struct EOD_API FPlayerAnimationReferencesTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	/** Reference to player animation montage that contains animations for flinching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> Flinch;

	/** Reference to player animation montage that contains animations for getting interrupted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> Interrupt;

	/** Reference to player animation montage that contains animations for getting knocked down */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> Knockdown;

	/** Reference to player animation montage that contains animations for getting stunned */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> Stun;

	/** Reference to player animation montage that contains animations for character attack getting deflected */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> AttackDeflect;

	/** Reference to player animation montage that contains animations for blocking an attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> BlockAttack;

	/** Reference to player animation montage that contains animations for normal attacks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> NormalAttacks;

	/** Reference to player animation montage that contains animations for jumping */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> Jump;

	/** Reference to player animation montage that contains animations for dodging */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> Dodge;

	/** Reference to player animation montage that contains animations for starting loot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> LootStart;

	/** Reference to player animation montage that contains animations for ending loot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> LootEnd;

	/** Animation montage containing animation for weapon switch - Full Body Slot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> WeaponSwitchFullBody;

	/** Animation montage containing animation for weapon switch - Upper Body Slot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> WeaponSwitchUpperBody;

	/** Reference to player animation montage that contains animations for player death */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	TSoftObjectPtr<UAnimMontage> Die;

	////////////////////////////////
	//~ @todo Remove following

	//~ @note Add AnimationMontage_WeaponChange animations here
	/** Reference to player animation montage that contains animations for special actions (@todo list special actions) */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimations)
	// TSoftObjectPtr<UAnimMontage> SpecialActions;

};

USTRUCT(BlueprintType)
struct EOD_API FSkillTreeSlotSaveData
{
	GENERATED_USTRUCT_BODY()

	/** The slot upgrade level that player has unlocked  */
	UPROPERTY()
	int32 CurrentUpgrade;

	FSkillTreeSlotSaveData()
	{
		CurrentUpgrade = 0;
	}
};

USTRUCT(BlueprintType)
struct EOD_API FSkillPointsAllocationInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 AvailableSkillPoints;

	UPROPERTY()
	int32 UsedSkillPoints;

	UPROPERTY()
	int32 AssassinPoints;

	UPROPERTY()
	int32 BerserkerPoints;

	UPROPERTY()
	int32 ClericPoints;

	UPROPERTY()
	int32 DefenderPoints;

	UPROPERTY()
	int32 SorcererPoints;

	FSkillPointsAllocationInfo()
	{
		AvailableSkillPoints = 0;
		UsedSkillPoints = 0;
		AssassinPoints = 0;
		BerserkerPoints = 0;
		ClericPoints = 0;
		DefenderPoints = 0;
		SorcererPoints = 0;
	}
};

USTRUCT(BlueprintType)
struct EOD_API FGameplaySkillTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** Character skill associated with this slot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplaySkillBase> SkillClass;

	FGameplaySkillTableRow()
	{
		SkillClass = NULL;
	}
};

USTRUCT(BlueprintType)
struct EOD_API FPlayerSystemSounds
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* NotEnoughGold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* NotEnoughStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* NotEnoughEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* NotEnoughHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SkillNotAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* CannotEquipItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* CannotUseItem;
};

USTRUCT(BlueprintType)
struct EOD_API FPlayerGameplaySounds
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DialogueTriggeredSound;

};

USTRUCT(BlueprintType)
struct EOD_API FWeaponHitSounds
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> BluntCritHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> BluntFleshHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> BluntMetalHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> BluntHitMissSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> BluntStoneHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> BluntUndeadHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> BluntWoodHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt2CritHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt2FleshHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt2MetalHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt2HitMissSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt2StoneHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt2UndeadHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt2WoodHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt3CritHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt3FleshHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt3MetalHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt3HitMissSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt3StoneHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt3UndeadHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Blunt3WoodHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SlashCritHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SlashFleshHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SlashMetalHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SlashHitMissSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SlashStoneHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SlashUndeadHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SlashWoodHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Slash2CritHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Slash2FleshHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Slash2MetalHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Slash2HitMissSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Slash2StoneHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Slash2UndeadHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> Slash2WoodHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SliceCritHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SliceFleshHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SliceMetalHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SliceHitMissSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SliceStoneHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SliceUndeadHitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TArray<USoundBase*> SliceWoodHitSounds;

};

/** Struct containing skill tree slot information */
USTRUCT(BlueprintType)
struct EOD_API FSkillTreeSlot : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** Determines if this slot is unlocked by default */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUnlockedByDefault;

	/** Vocation that this slot belongs to */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EVocations Vocation;

	/** Player skill associated with this slot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplaySkillBase> PlayerSkill;

	/** The skill tree column that this slot should be placed in */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Position")
	int32 ColumnPosition;

	/** The skill tree row that this slot should be placed in */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot Position")
	int32 RowPosition;

	/** The skill slot that must be unlocked first before unlocking this slot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SkillRequiredToUnlock;

	/** Minimum skill points required to be invested in vocation tree to unlock the first upgrade of this skill slot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MinimumPointsToUnlock;

	/** The number of additional points required to be invested in tree above MinimumPointsToUnlock for each skill upgrade */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 UpgradePointsGap;

	/**
	 * Maximum number of upgrades available for this skill
	 * This should match the MaxUpgrades value in PlayerSkill.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxUpgrades;

	/** Returns true if this skill tree slot contains valid information */
	bool IsValidSlot() const
	{
		return (ColumnPosition >= 0) && (RowPosition >= 0);
	}

	FSkillTreeSlot()
	{
		bUnlockedByDefault = true;
		Vocation = EVocations::Berserker;
		PlayerSkill = NULL;
		ColumnPosition = -1;
		RowPosition = -1;
		SkillRequiredToUnlock = NAME_None;
		MinimumPointsToUnlock = 0;
		UpgradePointsGap = 0;
		MaxUpgrades = 1;
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct EOD_API FPlayerStatsTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	int32 ExpRequired;
	
	UPROPERTY(EditAnywhere)
	int32 Health;
	
	UPROPERTY(EditAnywhere)
	int32 Mana;
	
	UPROPERTY(EditAnywhere)
	int32 Stamina;
	
	UPROPERTY(EditAnywhere)
	int32 HealthRegenRate;
	
	UPROPERTY(EditAnywhere)
	int32 ManaRegenRate;
	
	UPROPERTY(EditAnywhere)
	int32 StaminaRegenRate;
	
	UPROPERTY(EditAnywhere)
	int32 PhysicalAttack;
	
	UPROPERTY(EditAnywhere)
	int32 MagickalAttack;
	
	UPROPERTY(EditAnywhere)
	float PhysicalCritRate;
	
	UPROPERTY(EditAnywhere)
	float MagickalCritRate;
	
	UPROPERTY(EditAnywhere)
	int32 PhysicalResistance;
	
	UPROPERTY(EditAnywhere)
	int32 MagickalResistance;
	
	UPROPERTY(EditAnywhere)
	int32 BleedResistance;

	FPlayerStatsTableRow() :
		ExpRequired(0),
		Health(0),
		Mana(0),
		Stamina(0),
		HealthRegenRate(0),
		ManaRegenRate(0),
		StaminaRegenRate(0),
		PhysicalAttack(0),
		MagickalAttack(0),
		PhysicalCritRate(0.f),
		MagickalCritRate(0.f),
		PhysicalResistance(0),
		MagickalResistance(0),
		BleedResistance(0)
	{
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct EOD_API FCharacterStatsTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	int32 Level;

	UPROPERTY(EditAnywhere)
	int32 Health;
	
	UPROPERTY(EditAnywhere)
	int32 Mana;
	
	UPROPERTY(EditAnywhere)
	int32 Stamina;
	
	UPROPERTY(EditAnywhere)
	int32 HealthRegenRate;
	
	UPROPERTY(EditAnywhere)
	int32 ManaRegenRate;
	
	UPROPERTY(EditAnywhere)
	int32 StaminaRegenRate;
	
	UPROPERTY(EditAnywhere)
	int32 PhysicalAttack;
	
	UPROPERTY(EditAnywhere)
	int32 MagickalAttack;
	
	UPROPERTY(EditAnywhere)
	float PhysicalCritRate;
	
	UPROPERTY(EditAnywhere)
	float MagickalCritRate;
	
	UPROPERTY(EditAnywhere)
	int32 PhysicalResistance;
	
	UPROPERTY(EditAnywhere)
	int32 MagickalResistance;
	
	UPROPERTY(EditAnywhere)
	int32 BleedResistance;

	UPROPERTY(EditAnywhere)
	float PhysicalDamageReductionOnBlock;
	
	UPROPERTY(EditAnywhere)
	float MagickalDamageReductionOnBlock;

	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;

	FCharacterStatsTableRow() :
		Level(0),
		Health(0),
		Mana(0),
		Stamina(0),
		HealthRegenRate(0),
		ManaRegenRate(0),
		StaminaRegenRate(0),
		PhysicalAttack(0),
		MagickalAttack(0),
		PhysicalCritRate(0.f),
		MagickalCritRate(0.f),
		PhysicalResistance(0),
		MagickalResistance(0),
		BleedResistance(0)
	{
	}
};

/** Struct for in-game skills */
USTRUCT(BlueprintType, Blueprintable)
struct EOD_API FSkillTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** In-game name of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString InGameName;

	/** In-game description of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString Description;

	/** In-game icon representation of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	UTexture* Icon;

	/** Animation montage containing main animation (full body slot) for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	TSoftObjectPtr<UAnimMontage> AnimMontage;

	/** Animation montage containing upper body animation for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	TSoftObjectPtr<UAnimMontage> UpperBodyAnimMontage;

	/** Section name of skill start animation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillStartMontageSectionName;

	/** Section name of skill loop animation (used in spell casting) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillLoopMontageSectionName;

	/** Section name of skill end animation (used in spell casting) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillEndMontageSectionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "EWeaponType"))
	uint8 SupportedWeapons;

	/** Type of damage inflicted by this skill if this skill can inflict damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	EDamageType DamageType;

	/** Determines skill type which will be used by AI for calculating most suitable skill to use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	ESkillEffect SkillEffect;

	/** Minimum stamina required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 StaminaRequired;
	
	/** Minimum mana required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 ManaRequired;

	/** Determines whether this skill can be used while moving */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bAllowsMovement;

	/** Determines whether this skill has different animations to be used while walking in different directions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bHasDirectionalAnimations;

	/** Determines whether this skill is a passive skill or an active skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bPassiveSkill;

	/** Determines whether this skill can be used while character has been hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bCanBeUsedWhileHit;

	/** Skills, any of which MUST be used before using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TArray<FString> PrecedingSkillGroups;

	/** Skill that can be used after using this skill (skill chaining) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString SupersedingSkillGroup;

	/**
	 * SkillID for skill that comes on upgrading this skill.
	 * This will be NAME_None if the skill can't be upgraded any further
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName UpgradeSkillID;
	
	/**
	 * SkillID for skill from which you can upgrade to this skill.
	 * This will be NAME_None for a base skill
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName DowngradeSkillID;
	
	/** Current upgrade level of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 CurrentUpgradeLevel;

	/** Maximum upgrades available for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 MaxUpgrades;

	/** The status effect required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName RequiredStatusID;

	/** The duration before which the same skill can be used again */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float Cooldown;

	/** The duration of loop section if the skill has loop section */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float LoopDuration;

	/** Damage in percentage of player's magickal or physical attack that will be inflicted on enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 DamagePercent;

	/** Determines if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUnblockable;

	/** Determines if this skill can be dodged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUndodgable;

	/** Determines if the 'skill deflected' animation should play on getting blocked. Only applicable if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bIgnoresBlock;

	/** Crowd control effect on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	ECrowdControlEffect CrowdControlEffect;

	/** The duration for which the crowd control effect should last (if applicable) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float CrowdControlEffectDuration;

	/** The camera shake type that should play when this skill hits an enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	ECameraShakeType CameraShakeOnHit;

	/** Immunities from crowd control effects granted on using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;

	/** Status effect that this skill triggers */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	// TSubclassOf<class UStatusEffectBase> StatusEffect;

	FSkillTableRow()
	{
		Icon = nullptr;
		SupportedWeapons = 0;
		DamageType = EDamageType::Physical;
		SkillEffect = ESkillEffect::DamageMelee;
		StaminaRequired = 0;
		ManaRequired = 0;
		bAllowsMovement = false;
		bHasDirectionalAnimations = false;
		bPassiveSkill = false;
		bCanBeUsedWhileHit = false;
		CurrentUpgradeLevel = 1;
		MaxUpgrades = 1;
		Cooldown = 1.f;
		LoopDuration = 0.f;
		DamagePercent = 0;
		bUnblockable = false;
		bUndodgable = false;
		bIgnoresBlock = false;
		CrowdControlEffect = ECrowdControlEffect::Flinch;
		CrowdControlEffectDuration = 1.f;
		CameraShakeOnHit = ECameraShakeType::Medium;
		CrowdControlImmunities = 0;
		SkillStartMontageSectionName = FName("Default");
		SkillLoopMontageSectionName = NAME_None;
		SkillEndMontageSectionName = NAME_None;
	}
};

USTRUCT(BlueprintType)
struct EOD_API FCharacterStat
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 MaxValue;

	UPROPERTY()
	int32 CurrentValue;

	FCharacterStat() :
		MaxValue(0),
		CurrentValue(0)
	{
	}

	FCharacterStat(int32 Max, int32 Current) :
		MaxValue(Max),
		CurrentValue(Current)
	{
	}

	bool operator!=(const FCharacterStat& OtherStat)
	{
		return this->MaxValue != OtherStat.MaxValue || this->CurrentValue != OtherStat.CurrentValue;
	}

	bool operator==(const FCharacterStat& OtherStat)
	{
		return this->MaxValue == OtherStat.MaxValue && this->CurrentValue == OtherStat.CurrentValue;
	}
};

/**
 * CharacterLibrary contains static helper functions for in-game characters.
 * @note Do not inherit from this class
 */
UCLASS()
class EOD_API UCharacterLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	UCharacterLibrary(const FObjectInitializer& ObjectInitializer);
	
	/** Returns player skill */
	static FSkillTableRow* GetPlayerSkill(const FName SkillID, const FString& ContextString);

	/** Returns player skill */
	UFUNCTION(BlueprintCallable, Category = "Character Library", meta = (DisplayName = "Get Player Skill"))
	static FSkillTableRow BP_GetPlayerSkill(const FName SkillID, const FString& ContextString, bool& bOutSkillFound);

	static bool AreEnemies(AEODCharacterBase* CharacterOne, AEODCharacterBase* CharacterTwo);

	//~ Begin anim montage section names
	static const FName SectionName_ForwardFlinch;
	
	static const FName SectionName_BackwardFlinch;

	static const FName SectionName_ForwardInterrupt;

	static const FName SectionName_BackwardInterrupt;

	static const FName SectionName_AttackDeflected;

	static const FName SectionName_BlockAttack;

	static const FName SectionName_KnockdownStart;

	static const FName SectionName_KnockdownIdle;

	static const FName SectionName_KnockdownEnd;

	static const FName SectionName_LavitationStart;

	static const FName SectionName_LavitationEnd;

	static const FName SectionName_StunStart;

	static const FName SectionName_StunLoop;

	static const FName SectionName_StunEnd;

	static const FName SectionName_JumpStart;

	static const FName SectionName_JumpLoop;

	static const FName SectionName_JumpEnd;

	static const FName SectionName_ForwardJumpStart;

	static const FName SectionName_ForwardJumpLoop;

	static const FName SectionName_ForwardJumpEnd;

	static const FName SectionName_BackwardJumpStart;

	static const FName SectionName_BackwardJumpLoop;

	static const FName SectionName_BackwardJumpEnd;

	static const FName SectionName_FirstSwing;

	static const FName SectionName_FirstSwingEnd;

	static const FName SectionName_SecondSwing;

	static const FName SectionName_SecondSwingEnd;

	static const FName SectionName_ThirdSwing;

	static const FName SectionName_ThirdSwingEnd;

	static const FName SectionName_FourthSwing;

	static const FName SectionName_FourthSwingEnd;

	static const FName SectionName_FifthSwing;

	static const FName SectionName_FifthSwingEnd;

	static const FName SectionName_ForwardSPSwing;

	static const FName SectionName_ForwardSPSwingEnd;

	static const FName SectionName_BackwardSPSwing;

	static const FName SectionName_BackwardSPSwingEnd;

	static const FName SectionName_ForwardDodge;

	static const FName SectionName_BackwardDodge;

	static const FName SectionName_LeftDodge;

	static const FName SectionName_RightDodge;

	static const FName SectionName_Default;

	static const FName SectionName_SheatheWeapon;

	static const FName SectionName_UnsheatheWeapon;
	//~ End anim montage section names

};
