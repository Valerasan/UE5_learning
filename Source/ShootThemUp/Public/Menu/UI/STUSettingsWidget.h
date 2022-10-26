// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUSettingsWidget.generated.h"

class UButton;
class USlider;

UCLASS()
class SHOOTTHEMUP_API USTUSettingsWidget : public USTUBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	USlider* ResolutionScaleSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* ViewDistanceSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* AntiAliasingSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* PostProcessingSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* ShadowSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* TexturesSettings;

	UPROPERTY(meta = (BindWidget))
	USlider* EfectsSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* FoliageSlider;

private:
	UFUNCTION()
	void OnCloseSettings();

	UFUNCTION()
	void OnResolutionScaleChnaged(float Value);

	UFUNCTION()
	void OnViewDistanceChnaged(float Value);

	UFUNCTION()
	void OnAntiAliasingChnaged(float Value);

	UFUNCTION()
	void OnPostProcessingChnaged(float Value);

	UFUNCTION()
	void OnShadowChnaged(float Value);

	UFUNCTION()
	void OnTextureQualityChnaged(float Value);

	UFUNCTION()
	void OnEfectsChnaged(float Value);

	UFUNCTION()
	void OnFoliageChnaged(float Value);
};
