// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULuncherWeapon.generated.h"

class ASTUProjectile;
class USplineComponent;
class UNiagaraComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTULuncherWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void Zoom(bool Enable);

	ASTULuncherWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASTUProjectile> ProjectileClass;

	 UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	 USplineComponent* ProjectileSpline;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	// UNiagaraComponent* Niagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;

	virtual void MakeShot() override;
};
