// ShootThemUp All Rights Reserved

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


ASTUAIController::ASTUAIController() 
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);

}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
	if (STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
	}
	Super::OnPossess(InPawn);
}

void ASTUAIController::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);

}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FoucsOnKeyName));

}
