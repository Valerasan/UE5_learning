// ShootThemUp All Rights Reserved


#include "Components/STUCharacterMovementComponent.h"
#include "STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifire : MaxSpeed;
}
