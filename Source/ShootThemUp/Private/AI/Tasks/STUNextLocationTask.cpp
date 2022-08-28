// ShootThemUp All Rights Reserved

#include "AI/Tasks/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
{
	NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blakboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !Blakboard) return EBTNodeResult::Failed;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);

	FNavLocation NavLocation;
	auto Location = Pawn->GetActorLocation();
	if (!SelfCenter)
	{
		auto CentrActor = Cast<AActor>(Blakboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
		if (!CentrActor) return EBTNodeResult::Failed;
		Location = CentrActor->GetActorLocation();
	}


	const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
	if (!Found) return EBTNodeResult::Failed;

	Blakboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}
