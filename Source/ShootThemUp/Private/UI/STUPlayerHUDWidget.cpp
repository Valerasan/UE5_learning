// ShootThemUp All Rights Reserved

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
		
	if (!HealthComponent) return 0.f;
	return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

	if (!WeaponComponent) return false;

	return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

	if (!WeaponComponent) return false;

	return WeaponComponent->GetAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Contriller = GetOwningPlayer();
	return Contriller && Contriller->GetStateName() == NAME_Spectating;
}

bool USTUPlayerHUDWidget::Initialize()
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
	}

	return Super::Initialize();
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDeltd)
{
	if (HealthDeltd < 0)
	{
		OnTakeDamage();
	}
}


//USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponetn() const
//{
//	const auto Player = GetOwningPlayerPawn();
//	if (!Player) return nullptr;
//
//	const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
//
//	const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
//	return WeaponComponent;
//}
//
//USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponent() const
//{
//	const auto Player = GetOwningPlayerPawn();
//	if (!Player) return nullptr;
//
//	const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
//
//	const auto HealthComponent = Cast<USTUHealthComponent>(Component);
//	return HealthComponent;
//}
