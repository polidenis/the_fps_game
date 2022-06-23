// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/ASBase.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UASBase::UASBase()
{
	ImmuneTag = FGameplayTag::RequestGameplayTag(FName("Effect.Immune"));
}

void UASBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if(Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if(Attribute == GetMaxArmorAttribute())
	{
		AdjustAttributeForMaxChange(Armor, MaxArmor, NewValue, GetArmorAttribute());
	}
	else if(Attribute == GetMaxMovementSpeedAttribute())
	{
		AdjustAttributeForMaxChange(MovementSpeed, MaxMovementSpeed, NewValue, GetMovementSpeedAttribute());
	}
	else if(Attribute == GetMinMovementSpeedAttribute())
	{
		AdjustAttributeForMaxChange(MovementSpeed, MinMovementSpeed, NewValue, GetMovementSpeedAttribute());
	}
}

void UASBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

#define CheckAttribute(x) Data.EvaluatedData.Attribute == Get##x##Attribute()
	
	if(CheckAttribute(Damage))
	{
		// TODO(i.aksoy): Do damage logic
	}
	else if(CheckAttribute(Health))
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if(CheckAttribute(Mana))
	{
		SetHealth(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	else if(CheckAttribute(Armor))
	{
		SetHealth(FMath::Clamp(GetArmor(), 0.0f, GetMaxArmor()));
	}
	else if(CheckAttribute(MovementSpeed))
	{
		SetHealth(FMath::Clamp(GetMovementSpeed(), GetMinMovementSpeed(), GetMaxMovementSpeed()));
	}

#undef CheckAttribute
}

void UASBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, MaxArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, MaxMovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UASBase, MinMovementSpeed, COND_None, REPNOTIFY_Always);

}

void UASBase::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityC = GetOwningAbilitySystemComponent();
	const auto CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if(!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityC)
	{
		const auto CurrentValue = AffectedAttribute.GetCurrentValue();
		// Calculation for MaxValue
		const float Delta = CurrentMaxValue > 0.0f ? CurrentValue * NewMaxValue / CurrentMaxValue - CurrentValue : NewMaxValue;

		// Change Attribute
		AbilityC->ApplyModToAttribute(AffectedAttributeProperty, EGameplayModOp::Additive, Delta);
	}
}

void UASBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, Health, OldHealth);
}

void UASBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, MaxHealth, OldMaxHealth);
}

void UASBase::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, Mana, OldMana);
}

void UASBase::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, MaxMana, OldMaxMana);
}

void UASBase::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, Armor, OldArmor);
}

void UASBase::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, MaxArmor, OldMaxArmor);
}

void UASBase::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, MovementSpeed, OldMovementSpeed);
}

void UASBase::OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, MaxMovementSpeed, OldMaxMovementSpeed);
}

void UASBase::OnRep_MinMovementSpeed(const FGameplayAttributeData& OldMinMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UASBase, MinMovementSpeed, OldMinMovementSpeed);
}

