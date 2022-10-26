// ShootThemUp All Rights Reserved


#include "Menu/UI/STUMenuHUD.h"
#include "UI/STUBaseWidget.h"

void ASTUMenuHUD::SetSettingsVisibility(bool Visible) 
{
	SettingsMenuWidget->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void ASTUMenuHUD::SetMainMenuVisibility(bool Visible) 
{
	MainMenuWidget->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void ASTUMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			MainMenuWidget->Show();
		}
	}
	if (SettingsWidgetClass)
	{
		SettingsMenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), SettingsWidgetClass);
		if (SettingsMenuWidget)
		{
			SettingsMenuWidget->AddToViewport();
			SettingsMenuWidget->Show();
			SetSettingsVisibility(false);
		}
	}
}
