// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "UObject/Object.h"
#include "Effect.generated.h"

class UEffectData;
class UEffectsComponent;
class UEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectEvent, UEffect*, Effect);

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class EFFECTSSYSTEM_API UEffect : public UObject
{
	GENERATED_BODY()

public:

	UEffect();

public:

	UPROPERTY(BlueprintAssignable, Category = "Effect")
	FEffectEvent OnEffectStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "Effect")
	FEffectEvent OnEffectEnded;

protected:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (ExposeOnSpawn))
	UEffectData* EffectData;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (ExposeOnSpawn))
	float Duration;

private:

	UPROPERTY()
	FTimerHandle EffectTimer;

public:

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UWorld* GetWorld() const override;

	void InitEffect();
	void InitEffectWithData(UEffectData* Data);

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void Start();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
    void Stop();

	// Getters
	UFUNCTION(BlueprintPure, Category = "Effect")
	UEffectData* GetEffectData() const { return EffectData; }

	UFUNCTION(BlueprintPure, Category = "Effect")
	UEffectsComponent* GetEffectsComponent() const;

	UFUNCTION(BlueprintPure, Category = "Effect")
	AActor* GetOwner() const;

	UFUNCTION(BlueprintPure, Category = "Effect")
	float GetEffectRemainingTime() const;

	UFUNCTION(BlueprintPure, Category = "Effect")
	bool IsActive() const;
};
