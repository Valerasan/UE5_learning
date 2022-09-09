// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()

public:
	ASTUBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	bool CouldBeTakenTest = true;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	virtual void Tick(float DeltaTime) override;
	bool CouldBeTaken() const;

private:

	float RotationYaw = 0.0f;
	FTimerHandle RespawnTimerHandle;

	virtual bool GivePikupTo(APawn* PlayrPawn);
	void PickupWastaken();
	void Respawn();

	void GenerateRotationYaw();
};
