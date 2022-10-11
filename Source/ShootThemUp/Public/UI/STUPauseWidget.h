// ShootThemUp All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUPauseWidget.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

	   virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();
};