// ShootThemUp All Rights Reserved


#include "STUBasePlayerController.h"
#include "Components/STURespawnComponent.h"


ASTUBasePlayerController::ASTUBasePlayerController() 
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
	
}

void ASTUBasePlayerController::OnPossess(APawn* InPawn) 
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}