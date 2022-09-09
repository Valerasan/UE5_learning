// ShootThemUp All Rights Reserved


#include "AI/EQS/EnvQueryTest_PickupCouldeBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/STUBasePickup.h"

UEnvQueryTest_PickupCouldeBeTaken::UEnvQueryTest_PickupCouldeBeTaken(const FObjectInitializer& ObjectInitializer) 
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldeBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const 
{
	const auto DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	const bool WantsBeTakable = BoolValue.GetValue();

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
		if (!PickupActor) continue;

		const auto CouldBeTaken = PickupActor->CouldBeTaken();
		It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable);
	}
}
