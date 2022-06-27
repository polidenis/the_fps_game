// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityBase.h"
#include "Abilities/MainAbilitySystemComponent.h"
#include "Abilities/AttributeSets/ASBase.h"
#include "CharacterBase.generated.h"

UCLASS()
class THEFPSGAME_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();

	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UMainAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	UASBase* AttributeSetBase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbilityBase>> Abilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamageCharacter(float Damage);

	UFUNCTION(BlueprintCallable)
	virtual void RestoreHealth();
	
	UFUNCTION(BlueprintCallable)
	virtual void SetMovementSpeed(float WalkSpeed);

	UFUNCTION(BlueprintCallable)
	virtual float GetMovementSpeed();

	UFUNCTION(BlueprintCallable)
	virtual void AdjustMovementSpeed();

	UFUNCTION(BlueprintCallable)
	virtual void AdjustMovementSpeedToNormalState();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void BindAbility();

};
