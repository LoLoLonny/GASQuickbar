# GASQuickbar

This is a simple quickbar component and a wrapper class for abilities that tracks cooldowns and can execute the related ability. It's meant to be used RTS-style to invoke abilities on AI characters.

Usage:

Add the quickbar component to your RTS pawn.

You need to instantiate the AbilitySlots and pass them to the quickbar as an array. Where and how depends on your setup.

```c++
TArray<TObjectPtr<UGASQuickbarSlot>> Slots;
```

Here is how we do it for our units. We have a DataAsset that holds the ability set for a given unit:

```c++
void UEntityComponent::GiveAbilities(UAbilitySystemComponent* ASC)
{
	if(!AbilitySet)
		return;
	// If the server has the authority to grant abilities and there is a valid ASC
	if (GetOwner()->HasAuthority() && ASC)
	{
		// Foreach ability in DefaultAbilities, grant the ability
		for (TSubclassOf<UGameplayAbility>& AbilityToGrant : AbilitySet->Abilities)
		{
			if(!AbilityToGrant)
				continue;
				
				// `1` below is the level of the ability, which could later be used to allow abilities to scale with player level
				FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(
					FGameplayAbilitySpec(AbilityToGrant, 1, 0));
				
				//Create an UGASQuickbarSlot to track the status of an ability
				UGASQuickbarSlot* Instance = NewObject<UGASQuickbarSlot>(GetOwner());
				Instance->InitAbilityInstance(AbilityToGrant, ASC);
				Abilities.Add(Instance);

		}
	}
}
```
The units hold the instances. We pass the instances to the quickbar of the RTS pawn when a unit is selected:

```c++
if(UAbilityQuickbarComponent* QC = UAbilityQuickbarComponent::FindAbilityQuickbarComponent(this)){
				QC->UpdateSlots(Slots);
			}
```

You can also pass the slot instance as the source object when you grant the ability. This provides an easy way to supply all kinds of data to the ability. Additionally, you can subclass the `UGASQuickbarSlot` class to provide callbacks that can be used within the ability when the slot is the source object.

```c++
AbilitySpec.SourceObject = Instance;
```

Here's the corrected version with markdown preserved:

"The Slots and The Component provide events to update your UI. `OnSlotUpdate` is called when a new set of abilities is provided. `OnAbilityStateUpdate` is called when a slot is blocked or freed, and `OnCooldownUpdate` is called while there are active cooldowns.

You can execute a slot by calling `TryExecuteSlot` or `TryExecuteSlotByEvent` on the QuickbarComponent.

This plugin is used in the RTT game *Schleichzombie*. We use a subclass of QuickbarSlot that holds more information, like ability icons for the UI.

<img src="https://github.com/user-attachments/assets/d5efbd41-bfcd-4ba3-94d1-1c066aadcbe6" width="700"/>


