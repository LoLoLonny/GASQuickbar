// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "GASQuickbarComponent.generated.h"

class UQuickbarSlot;
class UAbilitySystemComponent;
class UGameplayAbility;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Command_Activate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUpdate, const TArray<UGASQuickbarSlot*>&, Slots);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASQUICKBAR_API UGASQuickbarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGASQuickbarComponent();

	UFUNCTION()
	void UpdateSlots(TArray<UGASQuickbarSlot*> Abilities);

	UFUNCTION(BlueprintCallable)
	void LoadSlots() const;

	/** Returns the quickbar component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Abilities|Quickbar")
	static UGASQuickbarComponent* FindAbilityQuickbarComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGASQuickbarComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable)
	virtual void TryExecuteSlot(int SlotIndex);

	UFUNCTION(BlueprintCallable)
	virtual void TryExecuteSlotWithEvent(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayAbility> AbilityClass, UGASQuickbarSlot* Slot);

private:
	UPROPERTY(AdvancedDisplay)
	TArray<TObjectPtr<UGASQuickbarSlot>> Slots;

	UPROPERTY(BlueprintAssignable)
	FOnSlotUpdate OnSlotUpdate;
};
