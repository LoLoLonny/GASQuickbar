// Fill out your copyright notice in the Description page of Project Settings.


#include "GASQuickbarSlot.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

void UGASQuickbarSlot::InitAbilityInstance(TSubclassOf<UGameplayAbility>& Ability, UAbilitySystemComponent* ASC)
{
    if (!Ability || !ASC)
        return;

    SlotAbility = Ability;
    AbilitySystemComponent = ASC;

    if (UGameplayAbility* Ability = SlotAbility->GetDefaultObject<UGameplayAbility>()) {

        const FGameplayTagContainer* CooldownTags = Ability->GetCooldownTags();

        if (!CooldownTags)
            return;

        FGameplayTag CooldownTag = CooldownTags->First();
        if (CooldownTag.IsValid()) {
            //if the ability has a cooldown tag (we only check the first) we register the cooldown update event for this instance
            AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnCooldownChanged);
        }
    }
}

bool UGASQuickbarSlot::IsValid() const
{
    return (AbilitySystemComponent != nullptr && SlotAbility != nullptr);
}

void UGASQuickbarSlot::OnCooldownChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    if (NewCount == 1) {
        bIsBlocked = true;
        GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ThisClass::OnCooldownTimerUpdate, .04f, true);
        CooldownDuration = GetCooldownTimeRemaining();
    }
    else {
        GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
        bIsBlocked = false;
        CooldownDuration = 0.0f;
    }
    //Notify listeners that the state has changed
    OnAbilityStateUpdate.Broadcast(bIsBlocked);
}

void UGASQuickbarSlot::OnCooldownTimerUpdate()
{
    OnCooldownUpdate.Broadcast(1 - (GetCooldownTimeRemaining() / CooldownDuration));
}

const float UGASQuickbarSlot::GetCooldownTimeRemaining() const
{
    if (IsValid()) {

        FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(SlotAbility);
        
        const FGameplayAbilityActorInfo* const ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();

        
        if (AbilitySpec->GetPrimaryInstance()) {
            const float Cooldown = AbilitySpec->GetPrimaryInstance()->GetCooldownTimeRemaining(ActorInfo);
            return Cooldown;
        }   

    }

    return 0.0f;
}

AAIController* UGASQuickbarSlot::GetOwnerAIController()
{
    if (APawn* Owner = Cast<APawn>(GetOuter())) {
        if (AAIController* Controller = Cast<AAIController>(Owner->GetController())) {
            return Controller;
        }
    }
    return nullptr;
}
