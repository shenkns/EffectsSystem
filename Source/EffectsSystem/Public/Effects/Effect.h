// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "UObject/Object.h"

#include "Effect.generated.h"

class UEffectData;
class UEffectsComponent;
class UEffect;
class APlayerState;

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
	UEffectData* Data;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (ExposeOnSpawn))
	APlayerState* Instigator;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (ExposeOnSpawn))
	UObject* Causer;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (ExposeOnSpawn))
	float Duration;

private:

	UPROPERTY()
	FTimerHandle EffectTimer;

	UPROPERTY(Replicated)
	float RemainingTime;

	UPROPERTY()
	FTimerHandle TickTimer;

public:

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
    
    virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	virtual UWorld* GetWorld() const override;

	void InitEffect();

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void Start();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
    void Stop();

	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ResetEffect(float EffectDuration);

	UFUNCTION(BlueprintCallable, Category = "Effect")
	void InitInstigateData(APlayerState* EffectInstigator, UObject* EffectCauser);

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	bool Stack(UEffect* StackableEffect);

	// Getters
	UFUNCTION(BlueprintPure, Category = "Effect")
	UEffectData* GetEffectData() const { return Data; }

	UFUNCTION(BlueprintPure, Category = "Effect")
	APlayerState* GetInstigator() const { return Instigator; }
	
	UFUNCTION(BlueprintPure, Category = "Effect")
	UObject* GetCauser() const { return Causer; }

	UFUNCTION(BlueprintPure, Category = "Effect")
	UEffectsComponent* GetEffectsComponent() const;

	UFUNCTION(BlueprintPure, Category = "Effect")
	AActor* GetOwner() const;

	UFUNCTION(BlueprintPure, Category = "Effect")
	float GetEffectRemainingTime() const;

	UFUNCTION(BlueprintPure, Category = "Effect")
	bool IsActive() const;

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void EffectTick(float DeltaTime);

private:

	void Tick();
};
