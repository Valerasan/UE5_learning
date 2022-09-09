// ShootThemUp All Rights Reserved


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePikupTo(APawn* PlayrPawn)
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayrPawn);
	if (!HealthComponent) return false;

	return HealthComponent->TryToAddHealth(HealthAmount);
}
