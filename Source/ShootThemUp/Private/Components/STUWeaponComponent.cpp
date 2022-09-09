// ShootThemUp All Rights Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our chracter can hold only %i weapon items"), WeaponNum);

	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || !GetWorld()) return;

	for (auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClasses);
		if (!Weapon) return;

		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Chracter);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Chracter->GetMesh(), WeaponArmorySocketName);
	}
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
		return;
	}

	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Chracter->GetMesh(), WeaponArmorySocketName);
	}
	CurrentWeapon = Weapons[WeaponIndex];
	// CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrenWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {	 //
		return Data.WeaponClasses == CurrentWeapon->GetClass();								 //
	});
	CurrentReloadAnimMontage = CurrenWeaponData ? CurrenWeaponData->ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Chracter->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

bool USTUWeaponComponent::IsFiring() const
{
	return CurrentWeapon && CurrentWeapon->IsFiring();
}

void USTUWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::Reload()
{
	ChangeClip();
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::GetAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for (const auto Weapon: Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter) return;

	Chracter->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{

	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify) 
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
			checkNoEntry();
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || Chracter->GetMesh() != MeshComp) return;

	EquipAnimInProgress = false;
	// UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Chracter = Cast<ACharacter>(GetOwner());
	if (!Chracter || Chracter->GetMesh() != MeshComp) return;

	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = false;
	// UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
}

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}



bool USTUWeaponComponent::CanReload() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon) return;
	if (CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();

	}
	else
	{
		for (const auto Weapon : Weapons)
		{
			if (Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

void USTUWeaponComponent::ChangeClip() 
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}