// ShootThemUp All Rights Reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Component/STUWeaponFXComponent.h"

ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SpehreComponent");
	CollisionComponent->InitSphereRadius(5.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 2000.f;

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
	SetLifeSpan(LifeSeconds);
}

void ASTUProjectile::OnProjectileHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	// make damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(),	 //
		DamageAmount,								 //
		GetActorLocation(),							 //
		DamageRadius,								 //
		UDamageType::StaticClass(),					 //
		{GetOwner()},								 //
		this,										 //
		nullptr,									 //
		DoFullDamage);

	// DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.f);

	WeaponFXComponent->PlayInpactFX(Hit);

	// Destroy();
	// SetLifeSpan(0.2f);

	SetLifeSpan(0.8f);
}

AController* ASTUProjectile::GetControllerChracter() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
