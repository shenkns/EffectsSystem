// Copyright shenkns Effects System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "Components/ActorComponent.h"
#include "Effects/Effect.h"

#include "EffectsComponent.generated.h"

class AActor;
class AController;
class UEffect;
class UEffectData;

UCLASS(ClassGroup=(Effects), meta=(BlueprintSpawnableComponent))
class EFFECTSSYSTEM_API UEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UEffectsComponent();

public:

	UPROPERTY(BlueprintAssignable, Category = "Effects")
	FEffectEvent OnEffectAdded;
	
	UPROPERTY(BlueprintAssignable, Category = "Effects")
	FEffectEvent OnEffectRemoved;
	
protected:

	UPROPERTY(EditAnywhere, Instanced, Category = "Effects")
	TMap<UEffectData*, UEffect*> EffectsMap;

	UPROPERTY(Replicated)
	TArray<UEffect*> EffectsArray;

public:

	virtual void BeginPlay() override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Effects")
	void AddEffect(UEffect* Effect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Effects")
	void RemoveEffect(UEffect* Effect);

	// Getters
	UFUNCTION(BlueprintPure, Category = "Effects", meta = (CompactNodeTitle = "Effect", DeterminesOutputType = "Class"))
	UEffect* GetEffectByData(TSubclassOf<UEffect> Class, UEffectData* Data) const;

	template<typename T>
	T* GetEffectByData(UEffectData* Data) const;

private:

	UFUNCTION()
	void OnEffectEnded(UEffect* Effect);
};

template <typename T>
T* UEffectsComponent::GetEffectByData(UEffectData* Data) const
{
	const UEffect* Out = EffectsArray.FindByPredicate([&](const UEffect* Src)
	{
		return Src->GetEffectData() == Data;
	});

	return Out ? static_cast<T*>(*Out) : nullptr;
}
