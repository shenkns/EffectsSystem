// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "Effects/Effect.h"

#include "EffectDOT.generated.h"

UCLASS()
class EFFECTSSYSTEM_API UEffectDOT : public UEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Effect|DOT", meta = (ExposeOnSpawn))
	float Damage;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Effect|DOT", meta = (ExposeOnSpawn))
	float Period = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|DOT", meta = (ExposeOnSpawn))
	bool bInstantStart;

private:

	UPROPERTY()
	FTimerHandle DOTTimer;

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Effect|DOT")
	void DealDamage();
};
