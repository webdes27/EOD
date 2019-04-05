// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatInterface.h"

UCombatInterface::UCombatInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

TSharedPtr<FAttackInfo> ICombatInterface::GetAttackInfoPtr() const
{
	return TSharedPtr<FAttackInfo>();
}

void ICombatInterface::SetAttackInfoFromActiveSkill(UActiveSkillBase* ActiveSkill)
{
}

void ICombatInterface::ResetAttackInfo()
{
}

bool ICombatInterface::IsEnemyOf(ICombatInterface* TargetCI) const
{
	return true;
}

TSharedPtr<FAttackResponse> ICombatInterface::ReceiveAttack(ICombatInterface* InstigatorCI, const TSharedPtr<FAttackInfo>& AttackInfoPtr)
{
	return TSharedPtr<FAttackResponse>();
}
