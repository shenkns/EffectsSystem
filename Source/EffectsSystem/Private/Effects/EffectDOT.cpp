// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Effects/EffectDOT.h"

#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"

void UEffectDOT::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEffectDOT, Damage)
	DOREPLIFETIME(UEffectDOT, Period)
}

void UEffectDOT::Start_Implementation()
{
	Super::Start_Implementation();

	GetWorld()->GetTimerManager().SetTimer(DOTTimer, this, &UEffectDOT::DealDamage, Period, true);

	if(bInstantStart)
	{
		DealDamage();
	}
}

void UEffectDOT::Stop_Implementation()
{
	Super::Stop_Implementation();

	GetWorld()->GetTimerManager().ClearTimer(DOTTimer);
}

void UEffectDOT::DealDamage_Implementation()
{
	UHealthComponent::Damage(GetOwner(), Damage, nullptr, nullptr);
}
