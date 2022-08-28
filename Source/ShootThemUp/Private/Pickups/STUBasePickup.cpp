// ShootThemUp All Rights Reserved

#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

ASTUBasePickup::ASTUBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisisonComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	check(CollisionComponent);

	GenerateRotationYaw();
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto PAwn = Cast<APawn>(OtherActor);
	//UE_LOG(LogBasePickup, Display, TEXT("Pickup was taken"));
	if (GivePikupTo(PAwn))
	{
		PickupWastaken();
	}

	// Destroy();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, RotationYaw, 0.f));
}

bool ASTUBasePickup::GivePikupTo(APawn* PlayrPawn)
{
	return false;
}

void ASTUBasePickup::PickupWastaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	FTimerHandle RespawnTimerHandle;

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
	GenerateRotationYaw(); 
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
}


void ASTUBasePickup::GenerateRotationYaw() 
{
	const auto Direction = FMath::RandBool() ? 1.f : -1.f;
	RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}