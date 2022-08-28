// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1", ClampMax = "100"))
	float HealthAmount = 50.f;
	
private:
	virtual bool GivePikupTo(APawn* PlayrPawn) override;
};
