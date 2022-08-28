// ShootThemUp All Rights Reserved

#include "AI/Services/STUChangeWeaponService.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "AIController.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	const auto Controller = OwnerComp.GetAIOwner();

	if (Controller)
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());

		if (WeaponComponent && Probaility > 0 && FMath::FRand() <= Probaility)
		{
			WeaponComponent->NextWeapon();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
