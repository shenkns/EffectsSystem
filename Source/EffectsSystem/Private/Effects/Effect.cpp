// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Effects/Effect.h"

#include "Module/EffectsSystemModule.h"
#include "Components/EffectsComponent.h"
#include "LogSystem.h"
#include "Data/EffectData.h"
#include "Net/UnrealNetwork.h"

UEffect::UEffect()
{
	Duration = 5.f;
}

void UEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEffect, EffectData)

	DOREPLIFETIME(UEffect, Duration)
	
	if(const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

UWorld* UEffect::GetWorld() const
{
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}

void UEffect::InitEffect(UEffectData* Data)
{
	EffectData = Data;

	LOG(LogEffectsSystem, "%s Initialized With %s", *GetName(), *EffectData->GetName())

	GetWorld()->GetTimerManager().SetTimer(EffectTimer, this, &UEffect::Stop, Duration);
	Start();
}

UEffectsComponent* UEffect::GetEffectsComponent() const
{
	return GetTypedOuter<UEffectsComponent>();
}

AActor* UEffect::GetOwner() const
{
	return GetEffectsComponent() ? GetEffectsComponent()->GetOwner() : nullptr;
}

float UEffect::GetEffectRemainingTime() const
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(EffectTimer);
}

bool UEffect::IsActive() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(EffectTimer);
}

void UEffect::Start_Implementation()
{
	OnEffectStarted.Broadcast(this);
	
	LOG(LogEffectsSystem, "%s Effect Started On %s", *GetName(), *GetOwner()->GetName())
}

void UEffect::Stop_Implementation()
{
	OnEffectEnded.Broadcast(this);
}
