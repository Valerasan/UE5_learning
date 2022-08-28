// ShootThemUp All Rights Reserved


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePikupTo(APawn* PlayrPawn)
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayrPawn);
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayrPawn);
	if (!WeaponComponent) return false;


	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
