// ShootThemUp All Rights Reserved

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASTUAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
	}
}

void ASTUAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto STUController = Cast<AAIController>(Controller);
	if (STUController && STUController->BrainComponent)
	{
		STUController->BrainComponent->Cleanup();
	}
}
