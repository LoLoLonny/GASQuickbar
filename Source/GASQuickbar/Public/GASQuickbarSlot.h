// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "GASQuickbarSlot.generated.h"

/**
 * 
 */

class UAbilitySystemComponent;
class UGameplayAbility;
class AAIController;

struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityStateUpdate, bool, bBlocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownUpdate, const float, Cooldown);

UCLASS()
class GASQUICKBAR_API UGASQuickbarSlot : public UObject
{
	GENERATED_BODY()
	
public:
	/** The Definition for this ability. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Appearance)
	TSubclassOf<UGameplayAbility> SlotAbility;

	FTimerHandle CooldownTimerHandle;

	UPROPERTY(BlueprintReadOnly)
	bool bIsBlocked = false;

	UPROPERTY(BlueprintReadOnly)
	bool bHasCooldown = false;

	UPROPERTY(BlueprintReadOnly)
	float CooldownDuration = 0.0f;
	
	UFUNCTION()
	virtual void InitAbilityInstance(TSubclassOf<UGameplayAbility>& Ability, UAbilitySystemComponent* ASC);

	UFUNCTION()
	bool IsValid() const;

	virtual void OnCooldownChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void OnCooldownTimerUpdate();

	const float GetCooldownTimeRemaining() const;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityStateUpdate OnAbilityStateUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnCooldownUpdate OnCooldownUpdate;

	UFUNCTION(BlueprintCallable)
	AAIController* GetOwnerAIController();

private:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
};
