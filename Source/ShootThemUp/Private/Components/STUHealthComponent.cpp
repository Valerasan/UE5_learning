// ShootThemUp All Rights Reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsHealthFull() || IsDead()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	check(MaxHealth > 0);
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);
	}
}

void USTUHealthComponent::OnTakeAnyDamageHandle(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealDelayTimerHandle);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(
			HealDelayTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
	PlayCameraShake();
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(HealModifire + Health);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealDelayTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	const auto HealthDelta = NextHealth - Health;


	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

bool USTUHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}