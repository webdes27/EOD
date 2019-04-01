// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PassiveSkillBase.h"

UPassiveSkillBase::UPassiveSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPassiveSkillBase::CancelSkill()
{
}

bool UPassiveSkillBase::CanCancelSkill() const
{
	return false;
}