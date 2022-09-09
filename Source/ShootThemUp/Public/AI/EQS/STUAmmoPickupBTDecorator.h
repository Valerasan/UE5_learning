// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUAmmoPickupBTDecorator.generated.h"

class ASTUBaseWeapon;

UCLASS()
class SHOOTTHEMUP_API USTUAmmoPickupBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	USTUAmmoPickupBTDecorator();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<ASTUBaseWeapon> WeaponType;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
