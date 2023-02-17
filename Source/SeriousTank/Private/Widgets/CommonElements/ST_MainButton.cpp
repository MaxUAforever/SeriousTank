#include "Widgets/CommonElements/ST_MainButton.h"

#include "Kismet/GameplayStatics.h"
#include "Styling/SlateTypes.h"

#include "GameFramework/ST_BasePlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(ST_MainButton, Warning, All)

void UST_MainButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (UWorld* World = GetWorld())
	{
		if (AST_BasePlayerController* PlayerController = Cast<AST_BasePlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
		{
			PlayerController->OnNavigationKeyPressed.AddUObject(this, &ThisClass::OnNavigationKeyPressed);
		}
	}
}

void UST_MainButton::OnNavigationKeyPressed()
{
	UpdateState(false);
}

void UST_MainButton::UpdateState(bool bIsMouseEvent)
{
	if (bIsMouseEvent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mouse event."))
		if (IsHovered())
		{
			UE_LOG(LogTemp, Warning, TEXT("Button is hovered."))
			SetKeyboardFocus();
			SetStyle(SelectedButtonStyle);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Button is NOT hovered."))
			SetStyle(NotSelectedButtonStyle);
		}
	}
	else
	{
		if (HasKeyboardFocus())
		{
			UE_LOG(LogTemp, Warning, TEXT("Button has keyboard focus."))
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Button has NOT keyboard focus."))
		}
		SetStyle(HasKeyboardFocus() ? SelectedButtonStyle : NotSelectedButtonStyle);
	}
}
