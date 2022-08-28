// ShootThemUp All Rights Reserved

#include "Weapon/Component/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void USTUWeaponFXComponent::PlayInpactFX(const FHitResult& Hit)
{
	auto ImpactData = DefaultImpactData;

	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMat))
		{
			ImpactData = ImpactDataMap[PhysMat];
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),	//
		ImpactData.NiagaraSystem,								//
		Hit.ImpactPoint,										//
		Hit.ImpactNormal.Rotation());

	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),  //
		ImpactData.DecalData.Material,										  //
		ImpactData.DecalData.Size,											  //
		Hit.ImpactPoint,													  //
		Hit.ImpactNormal.Rotation());

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime,	//
			ImpactData.DecalData.FadeOutTime);
	}
}
