// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TheFPSGame/TheFPSGame.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class THEFPSGAME_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbilityBase();

public:
	bool bCharacterAbilitiesGiven{};
	bool bStartupEffectsApplied{};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;
};
