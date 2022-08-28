// ShootThemUp All Rights Reserved

#include "Weapon/STULuncherWeapon.h"
#include "Weapon/STUProjectile.h"


void ASTULuncherWeapon::StartFire()
{
	MakeShot();
}

void ASTULuncherWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty()) return;

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
}
