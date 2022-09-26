// ShootThemUp All Rights Reserved

#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIWeaponComponent, All, All);

void USTUAIWeaponComponent::StartFire()
{
	if (!CanFire())
	{
		
		return;
	}

	if (CurrentWeapon->IsAmmoEmpty())
	{
		NextWeapon();
	}
	else
	{
		CurrentWeapon->StartFire();
	}
}

void USTUAIWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while (NextIndex != CurrentWeaponIndex)
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
		NextIndex = (NextIndex + 1) % Weapons.Num();
	}

	if (CurrentWeaponIndex != NextIndex)
	{
		CurrentWeaponIndex = NextIndex;
		ReloadAnimInProgress = false;
		EquipWeapon(CurrentWeaponIndex);
	}
}
