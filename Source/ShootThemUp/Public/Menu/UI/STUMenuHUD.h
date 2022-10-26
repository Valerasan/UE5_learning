// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUMenuHUD.generated.h"

class USTUBaseWidget;

UCLASS()
class SHOOTTHEMUP_API ASTUMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetSettingsVisibility(bool Visible);
	void SetMainMenuVisibility(bool Visible);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SettingsWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	USTUBaseWidget* MainMenuWidget = nullptr;

	UPROPERTY()
	USTUBaseWidget* SettingsMenuWidget = nullptr;
};