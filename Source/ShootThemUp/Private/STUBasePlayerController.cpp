// ShootThemUp All Rights Reserved


#include "STUBasePlayerController.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"
#include "STUGameInstance.h"

ASTUBasePlayerController::ASTUBasePlayerController() 
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
	
}

void ASTUBasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUBasePlayerController::OnMatchStateChanged);
		}
	}
}

void ASTUBasePlayerController::OnMatchStateChanged(ESTUMatchState State)
{
	if (State == ESTUMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void ASTUBasePlayerController::OnMuteSound() 
{
	if (!GetWorld()) return;

	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if (!STUGameInstance) return;

	STUGameInstance->ToggleVolume();

}

void ASTUBasePlayerController::OnPossess(APawn* InPawn) 
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}

void ASTUBasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUBasePlayerController::OnPauseGame);
	InputComponent->BindAction("Mute", IE_Pressed, this, &ASTUBasePlayerController::OnMuteSound);
}

void ASTUBasePlayerController::OnPauseGame() 
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);
}