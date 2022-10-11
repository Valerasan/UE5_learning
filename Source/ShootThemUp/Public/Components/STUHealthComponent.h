// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreType.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USTUHealthComponent();

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); };

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const { return Health / MaxHealth; }

	float GetHealt() const { return Health; }

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "5"))
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealModifire = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Health = 0.f;

	UFUNCTION()
	void OnTakeAnyDamageHandle(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FTimerHandle HealDelayTimerHandle;

	void HealUpdate();

	void SetHealth(float NewHealth);

	void PlayCameraShake();

	void Killed(AController* KillerController);
};
