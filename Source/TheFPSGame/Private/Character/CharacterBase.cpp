// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include <type_traits>

#include "TheFPSGame/TheFPSGame.h"

template <typename T>
std::underlying_type_t<T> underlying_cast(T value)
{
	return static_cast<std::underlying_type_t<T>>(value);
}


// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UMainAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	// Setting to mixed due to we want to replicate gameplay effects etc.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UASBase>("Attributes");
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::InitializeAttributes()
{
	if(DefaultAttributes && AbilitySystemComponent)
	{
		auto EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		auto SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
		if(SpecHandle.IsValid())
		{
			auto GameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ACharacterBase::GiveAbilities()
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		for (const auto& Ability : Abilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	GiveAbilities();
}

void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	BindAbility();
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

float ACharacterBase::GetHealth()
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetHealth();
	}

	return 1.0f;
}

void ACharacterBase::ApplyDamage(float Damage)
{
	if (AttributeSetBase)
	{
		AttributeSetBase->SetHealth(AttributeSetBase->GetHealth() - Damage);
	}
}

void ACharacterBase::RestoreHealth()
{
	if (AttributeSetBase)
	{
		AttributeSetBase->SetHealth(AttributeSetBase->GetMaxHealth());
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindAbility();
}

void ACharacterBase::BindAbility()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Ability Binding"));
	if(AbilitySystemComponent && InputComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Valid Binding"));
		const FGameplayAbilityInputBinds Binds{"Confirm", "Cancel", "EAbilityInputID", underlying_cast(EAbilityInputID::Confirm), underlying_cast(EAbilityInputID::Cancel)};
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

