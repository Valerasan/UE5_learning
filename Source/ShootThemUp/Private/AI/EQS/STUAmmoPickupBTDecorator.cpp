// ShootThemUp All Rights Reserved


#include "AI/EQS/STUAmmoPickupBTDecorator.h"
#include "STUAmmoPickupBTDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUAmmoPickupBTDecorator::USTUAmmoPickupBTDecorator() 
{
	NodeName = "Weapon Pickup";
}

bool USTUAmmoPickupBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);
}
