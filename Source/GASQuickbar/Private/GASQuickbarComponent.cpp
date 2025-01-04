// Fill out your copyright notice in the Description page of Project Settings.


#include "GASQuickbarComponent.h"
#include "GASQuickbarSlot.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GASQuickbarSlot.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Command_Activate, "Ability.Command.Activate");

// Sets default values for this component's properties
UGASQuickbarComponent::UGASQuickbarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGASQuickbarComponent::UpdateSlots(TArray<UGASQuickbarSlot*> Abilities)
{
	Slots = Abilities;
	LoadSlots();
}

void UGASQuickbarComponent::LoadSlots() const
{
	OnSlotUpdate.Broadcast(Slots);
}


void UGASQuickbarComponent::TryExecuteSlot(int SlotIndex)
{
	if (!Slots.IsValidIndex(SlotIndex))
		return;

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner())) {
		UGASQuickbarSlot* Slot = Slots[SlotIndex];

		
		if (Slot) {
			TryExecuteSlotWithEvent(ASC, Slot->SlotAbility, Slot);
		}
		
	}
}

void UGASQuickbarComponent::TryExecuteSlotWithEvent(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayAbility> AbilityClass, UGASQuickbarSlot* Slot)
{
	if (!ASC)
		return;

	FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(AbilityClass);

	if (AbilitySpec) {

		FGameplayEventData Payload;
		Payload.Instigator = GetOwner();
		Payload.OptionalObject = Slot;

		AActor* TargetActor = const_cast<AActor*>(ToRawPtr(Payload.Instigator));

		// The actor info needed for the Command.
		FGameplayAbilityActorInfo ActorInfo;

		ActorInfo.InitFromActor(GetOwner(), TargetActor, ASC);

		// Trigger the ability using event tag.

		const bool bSuccess = ASC->TriggerAbilityFromGameplayEvent(
			AbilitySpec->Handle,
			&ActorInfo,
			TAG_Ability_Command_Activate,
			&Payload,
			*ASC
		);
	}
}
