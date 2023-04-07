// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Components/EffectsComponent.h"

#include "Module/EffectsSystemModule.h"
#include "LogSystem.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

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
		for(const TTuple<UEffectData*, UEffect*>& Pair : EffectsMap)
		{
			Pair.Value->Rename(nullptr, this);
			Pair.Value->InitEffectWithData(Pair.Key);

			EffectsArray.Add(Pair.Value);
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

void UEffectsComponent::AddEffect(UEffect* Effect)
{
	Effect->Rename(nullptr, this);
	
	EffectsArray.Add(Effect);
	Effect->OnEffectEnded.AddUniqueDynamic(this, &UEffectsComponent::OnEffectEnded);

	Effect->InitEffect();

	OnEffectAdded.Broadcast(Effect);
}

void UEffectsComponent::RemoveEffect(UEffect* Effect)
{
	EffectsArray.Remove(Effect);

	Effect->Stop();

	OnEffectRemoved.Broadcast(Effect);
}
