// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupCouldeBeTaken.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API UEnvQueryTest_PickupCouldeBeTaken : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_PickupCouldeBeTaken(const FObjectInitializer& ObjectInitializer);
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
