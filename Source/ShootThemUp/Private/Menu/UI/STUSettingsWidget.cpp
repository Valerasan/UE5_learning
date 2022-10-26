// ShootThemUp All Rights Reserved

#include "Menu/UI/STUSettingsWidget.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "Components/Button.h"
#include "Menu/UI/STUMenuHUD.h"
#include "Kismet/GameplayStatics.h"

void USTUSettingsWidget::NativeOnInitialized()
{
	if (ResolutionScaleSlider) ResolutionScaleSlider->OnValueChanged.AddDynamic(this, &USTUSettingsWidget::OnResolutionScaleChnaged);
	if (ViewDistanceSlider) ViewDistanceSlider->OnValueChanged.AddDynamic(this, &USTUSettingsWidget::OnViewDistanceChnaged);
	if (AntiAliasingSlider) AntiAliasingSlider->OnValueChanged.AddDynamic(this, &USTUSettingsWidget::OnAntiAliasingChnaged);
	if (PostProcessingSlider) PostProcessingSlider->OnValueChanged.AddDynamic(this, &USTUSettingsWidget::OnPostProcessingChnaged);
	if (ShadowSlider) ShadowSlider->OnValueChanged.AddDynamic(this, &USTUSettingsWidget::OnShadowChnaged);
	if (TexturesSettings) TexturesSettings->OnValueChanged.AddDynamic(this, &USTUSettingsWidget::OnTextureQualityChnaged);
	if (EfectsSlider) EfectsSlider->OnValueChanged.AddDynamic(this, &USTUSettingsWidget::OnEfectsChnaged);
	if (FoliageSlider) FoliageSlider->OnValueChanged.AddDynamic(this, &USTUSettingsWidget::OnFoliageChnaged);

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &USTUSettingsWidget::OnCloseSettings);
	}

	ResolutionScaleSlider->SetValue(GEngine->GameUserSettings->GetDefaultResolutionScale());
	ViewDistanceSlider->SetValue(GEngine->GameUserSettings->GetViewDistanceQuality());
	AntiAliasingSlider->SetValue(GEngine->GameUserSettings->GetAntiAliasingQuality());
	PostProcessingSlider->SetValue(GEngine->GameUserSettings->GetPostProcessingQuality());
	ShadowSlider->SetValue(GEngine->GameUserSettings->GetShadowQuality());
	TexturesSettings->SetValue(GEngine->GameUserSettings->GetTextureQuality());
	EfectsSlider->SetValue(GEngine->GameUserSettings->GetVisualEffectQuality());
	FoliageSlider->SetValue(GEngine->GameUserSettings->GetFoliageQuality());
}

void USTUSettingsWidget::OnResolutionScaleChnaged(float Value)
{
	GEngine->GameUserSettings->SetResolutionScaleValueEx(Value);
}

void USTUSettingsWidget::OnViewDistanceChnaged(float Value)
{
	GEngine->GameUserSettings->SetViewDistanceQuality(Value);
}

void USTUSettingsWidget::OnAntiAliasingChnaged(float Value)
{
	GEngine->GameUserSettings->SetAntiAliasingQuality(Value);
}

void USTUSettingsWidget::OnPostProcessingChnaged(float Value)
{
	GEngine->GameUserSettings->SetPostProcessingQuality(Value);
}

void USTUSettingsWidget::OnShadowChnaged(float Value)
{
	GEngine->GameUserSettings->SetShadowQuality(Value);
}

void USTUSettingsWidget::OnTextureQualityChnaged(float Value)
{
	GEngine->GameUserSettings->SetTextureQuality(Value);
	// UE_LOG(LogTemp, Warning, TEXT("Texture: %i"), GEngine->GameUserSettings->GetTextureQuality());
}

void USTUSettingsWidget::OnEfectsChnaged(float Value)
{
	GEngine->GameUserSettings->SetVisualEffectQuality(Value);
}

void USTUSettingsWidget::OnFoliageChnaged(float Value)
{
	GEngine->GameUserSettings->SetFoliageQuality(Value);
}

void USTUSettingsWidget::OnCloseSettings()
{
	GEngine->GameUserSettings->ApplySettings(false);

	auto HUD = Cast<ASTUMenuHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	// UE_LOG(LogTemp, Warning, TEXT("Texture: %i"), GEngine->GameUserSettings->GetTextureQuality());
	SetVisibility(ESlateVisibility::Hidden);
	if (HUD)
	{
		HUD->SetMainMenuVisibility(true);
	}
}