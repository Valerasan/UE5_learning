// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreType.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTUWeaponComponent();

	virtual void StartFire();
	void StopFire();
	bool IsFiring() const;

	virtual void NextWeapon();
	void Reload();

	bool GetWeaponUIData(FWeaponUIData& UIData) const;
	bool GetAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);
	bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	int32 CurrentWeaponIndex = 0;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool CanFire() const;
	bool CanEquip() const;
	void EquipWeapon(int32 WeaponIndex);

private:
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	void InitAnimations();
	void SpawnWeapons();

	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	void PlayAnimMontage(UAnimMontage* Animation);

	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);

	bool CanReload() const;

	void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();
};
