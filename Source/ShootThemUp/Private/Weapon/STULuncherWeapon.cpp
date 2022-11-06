// ShootThemUp All Rights Reserved

#include "Weapon/STULuncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "NiagaraComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStaticsTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogLauncher, All, All);

void ASTULuncherWeapon::StartFire()
{
	MakeShot();
}

ASTULuncherWeapon::ASTULuncherWeapon() : ASTUBaseWeapon()
{
	ProjectileSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Projcetile Spline"));
	ProjectileSpline->SetupAttachment(WeaponMesh, MuzzleSocketName);
}

void ASTULuncherWeapon::MakeShot()
{
	if (!GetWorld()) return;

	if (IsAmmoEmpty())
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		return;
	}

	FVector TraceStart, TraceEnd;

	if (!GetTraceDate(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
	// Set projectile params
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreasAmmo();
	SpawnMuzzleFX();

	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
}

void ASTULuncherWeapon::Zoom(bool Enable)
{
	FVector TraceStart, TraceEnd;

	if (!GetTraceDate(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	ProjectileSpline->ClearSplinePoints(true);
	FPredictProjectilePathParams Params = FPredictProjectilePathParams(10.0f,  //
		WeaponMesh->GetSocketLocation(MuzzleSocketName),					   //
		Direction * 3000.0f,												   //
		3.0f);

	Params.bTraceWithCollision = true;
	Params.bTraceComplex = true;
	Params.DrawDebugType = EDrawDebugTrace::ForDuration;
	Params.DrawDebugTime = 5.f;
	Params.SimFrequency = 10.0f;
	Params.TraceChannel = ECollisionChannel::ECC_Visibility;
	Params.OverrideGravityZ = 0;
	Params.bTraceWithChannel = true;
	FPredictProjectilePathResult PredictResult;

	UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictResult);

	UE_LOG(LogLauncher, Display, TEXT("Owner Name: %s"), *GetOwner()->GetName());
	for (int i = 0; i < PredictResult.PathData.Num(); i++)
	{
		ProjectileSpline->AddSplinePoint(PredictResult.PathData[i].Location, ESplineCoordinateSpace::World);
	}
}
