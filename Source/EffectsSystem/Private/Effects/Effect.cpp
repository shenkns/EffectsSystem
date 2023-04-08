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

	DOREPLIFETIME(UEffect, Data)
	DOREPLIFETIME(UEffect, Instigator)
	DOREPLIFETIME(UEffect, Causer)

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

void UEffect::InitEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(EffectTimer);
	GetWorld()->GetTimerManager().SetTimer(EffectTimer, this, &UEffect::Stop, Duration);
	
	Start();

	if(Data)
	{
		LOG(LogEffectsSystem, "%s Initialized With %s", *GetName(), *Data->GetName())
	}
}

void UEffect::ResetEffect(float EffectDuration)
{
	Duration = EffectDuration;

	InitEffect();
}

void UEffect::InitInstigateData(APlayerState* EffectInstigator, UObject* EffectCauser)
{
	Instigator = EffectInstigator;
	Causer = EffectCauser;
}

bool UEffect::Stack_Implementation(UEffect* StackableEffect)
{
	if(Data->bStackable)
	{
		ResetEffect(GetEffectRemainingTime() + StackableEffect->GetEffectRemainingTime());

		return true;
	}

	return false;
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
	if(IsActive())
	{
		return GetWorld()->GetTimerManager().GetTimerRemaining(EffectTimer);
	}

	return Duration;
}

bool UEffect::IsActive() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(EffectTimer);
}

void UEffect::Start_Implementation()
{
	GetWorld()->GetTimerManager().UnPauseTimer(EffectTimer);
	OnEffectStarted.Broadcast(this);
	
	LOG(LogEffectsSystem, "%s Effect Started On %s", *GetName(), *GetOwner()->GetName())
}

void UEffect::Stop_Implementation()
{
	GetWorld()->GetTimerManager().PauseTimer(EffectTimer);
	Duration = GetWorld()->GetTimerManager().GetTimerRemaining(EffectTimer);
	
	OnEffectEnded.Broadcast(this);
}
