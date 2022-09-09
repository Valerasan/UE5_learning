// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUHealthPercentBTDecorator.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUHealthPercentBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	USTUHealthPercentBTDecorator();

	protected:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float HealthPercent = 0.6f;

		virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
