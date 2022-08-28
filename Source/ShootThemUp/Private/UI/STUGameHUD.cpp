// ShootThemUp All Rights Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();

	//DrawCrossHair();
}

void ASTUGameHUD::BeginPlay() 
{
	Super::BeginPlay();

	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidegetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}

void ASTUGameHUD::DrawCrossHair()
{
	const TInterval<float> Centr(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.f;
	const float LineThicknes = 2.f;
	const FLinearColor LineralColor = FLinearColor::Green;


	DrawLine(Centr.Min - HalfLineSize, Centr.Max, Centr.Min + HalfLineSize, Centr.Max, LineralColor, LineThicknes);
	DrawLine(Centr.Min, Centr.Max - HalfLineSize, Centr.Min, Centr.Max + HalfLineSize, LineralColor, LineThicknes);
}
