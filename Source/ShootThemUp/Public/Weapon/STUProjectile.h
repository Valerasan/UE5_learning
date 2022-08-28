// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()

public:
	ASTUProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool DoFullDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float LifeSeconds = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	USTUWeaponFXComponent* WeaponFXComponent;

	virtual void BeginPlay() override;

private:
	FVector ShotDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	AController* GetControllerChracter() const;
};
