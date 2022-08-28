// ShootThemUp All Rights Reserved

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Component/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASTURifleWeapon::ASTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandl, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandl);
	SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot()
{
	UE_LOG(LogTemp, Display, TEXT("Make Shot"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;

	if (!GetTraceDate(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	///////////////////////////
	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	const FVector ActualTraceEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
	const FVector HitDirectionFromMuzzle = (ActualTraceEnd - SocketTransform.GetLocation()).GetSafeNormal();
	const float Degrees =
		FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SocketTransform.GetRotation().GetForwardVector(), HitDirectionFromMuzzle)));
	// UE_LOG(LogBaseWeapon, Display, TEXT("Andle %f"), Degrees);
	/////////////////////////

	FVector TraceFXend = TraceEnd;

	if (HitResult.bBlockingHit)
	{
		TraceFXend = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayInpactFX(HitResult);
	}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXend);
	DecreasAmmo();
}

bool ASTURifleWeapon::GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotaion;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotaion)) return false;

	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotaion.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if (!DamageActor) return;

	DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}

void ASTURifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible, true);
	}
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd) 
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}

}
