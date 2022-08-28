// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreType.h"
#include "STUWeaponFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USTUWeaponFXComponent();

	void PlayInpactFX(const FHitResult& Hit);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FImpactData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};
