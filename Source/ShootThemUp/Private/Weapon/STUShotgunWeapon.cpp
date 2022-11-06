// ShootThemUp All Rights Reserved


#include "Weapon/STUShotgunWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Component/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ASTUShotgunWeapon::ASTUShotgunWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTUShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void ASTUShotgunWeapon::StartFire()
{
	InitFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandl, this, &ASTUShotgunWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTUShotgunWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandl);
	SetFXActive(false);
}

void ASTUShotgunWeapon::Zoom(bool Enabled)
{
	const auto Controller = Cast<APlayerController>(GetControllerChracter());
	if (!Controller || !Controller->PlayerCameraManager) return;

	if (Enabled)
	{
		DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
	}

	Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}

void ASTUShotgunWeapon::MakeShot()
{
	// UE_LOG(LogTemp, Display, TEXT("Make Shot"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;


	for (int i = 0; i < BuckshotAmount; i++)
	{
		if (!GetTraceDate(TraceStart, TraceEnd))
		{
			StopFire();
			return;
		}
		FHitResult HitResult;
		MakeHit(HitResult, TraceStart, TraceEnd);

		FVector TraceFXend = TraceEnd;

		if (HitResult.bBlockingHit)
		{
			TraceFXend = HitResult.ImpactPoint;
			MakeDamage(HitResult);
			WeaponFXComponent->PlayInpactFX(HitResult);
		}
		SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXend);
	}
	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	DecreasAmmo();
}

bool ASTUShotgunWeapon::GetTraceDate(FVector& TraceStart, FVector& TraceEnd) const
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

void ASTUShotgunWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if (!DamageActor) return;


	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
	DamageActor->TakeDamage(DamageAmount, PointDamageEvent, GetControllerChracter(), this);
}

void ASTUShotgunWeapon::InitFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetFXActive(true);
}

void ASTUShotgunWeapon::SetFXActive(bool IsActive)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive, true);
	}
}

void ASTUShotgunWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

AController* ASTUShotgunWeapon::GetControllerChracter() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}