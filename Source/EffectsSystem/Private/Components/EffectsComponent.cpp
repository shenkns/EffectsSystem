// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Components/EffectsComponent.h"

#include "Module/EffectsSystemModule.h"
#include "LogSystem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/PlayerState.h"

UEffectsComponent::UEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
}

void UEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		for(UEffect* Effect : StartEffects)
		{
			Effect->InitInstigateData(GetOwner()->GetInstigatorController() ? GetOwner()->GetInstigatorController()->GetPlayerState<APlayerState>() : nullptr,
				GetOwner()
			);
			
			AddEffect(Effect);
		}

		LOG(LogEffectsSystem, "%d Effects Initialized", EffectsArray.Num())
	}
}

bool UEffectsComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for(UEffect* Effect : EffectsArray)
	{
		if(Effect)
		{
			bWroteSomething |= Channel->ReplicateSubobject(Effect, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UEffectsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UEffectsComponent, EffectsArray)
}

UEffect* UEffectsComponent::GetEffectByData(TSubclassOf<UEffect> Class, UEffectData* Data) const
{
	UEffect* const* Out = EffectsArray.FindByPredicate([&](const UEffect* Src)
	{
		return Src->GetEffectData() == Data;
	});
	
	return Out ? *Out : nullptr;
}

void UEffectsComponent::OnEffectEnded(UEffect* Effect)
{
	RemoveEffect(Effect);
}

void UEffectsComponent::MulticastEffectRemoved_Implementation(UEffect* RemovedEffect)
{
	if(!GetOwner()->HasAuthority())
	{
		OnEffectRemoved.Broadcast(RemovedEffect);
	}
}

void UEffectsComponent::MulticastEffectAdded_Implementation(UEffect* AddedEffect)
{
	if(!GetOwner()->HasAuthority())
	{
		OnEffectAdded.Broadcast(AddedEffect);
	}
}

void UEffectsComponent::AddEffect(UEffect* Effect)
{
	if(UEffect* StackableEffect = GetEffectByData<UEffect>(Effect->GetEffectData()))
	{
		if(StackableEffect->Stack(Effect))
		{
			return;
		}
	}
	
	Effect->Rename(nullptr, this);
	
	EffectsArray.Add(Effect);
	Effect->OnEffectEnded.AddUniqueDynamic(this, &UEffectsComponent::OnEffectEnded);

	Effect->InitEffect();

	GetOwner()->ForceNetUpdate();

	OnEffectAdded.Broadcast(Effect);
	MulticastEffectAdded(Effect);
}

void UEffectsComponent::RemoveEffect(UEffect* Effect)
{
	EffectsArray.Remove(Effect);
	Effect->OnEffectEnded.RemoveDynamic(this, &UEffectsComponent::OnEffectEnded);

	Effect->Stop();

	OnEffectRemoved.Broadcast(Effect);
	MulticastEffectRemoved(Effect);
}
