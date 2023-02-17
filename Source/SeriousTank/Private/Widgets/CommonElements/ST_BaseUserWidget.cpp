#include "Widgets/CommonElements/ST_BaseUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/ST_BasePlayerController.h"
#include "Widgets/CommonElements/ST_MainButton.h"

void UST_BaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> Widgets;
	WidgetTree->GetChildWidgets(WidgetTree->RootWidget, Widgets);

	for (UWidget* Widget : Widgets)
	{
		if (UST_MainButton* Button = Cast<UST_MainButton>(Widget))
		{
			WidgetButtons.Add(Button);
			Button->OnHovered.AddDynamic(this, &ThisClass::OnButtonHovered);
		}
	}

	if (UWorld* World = GetWorld())
	{
		if (AST_BasePlayerController* PlayerController = Cast<AST_BasePlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
		{
			PlayerController->OnNavigationKeyPressed.AddUObject(this, &ThisClass::OnNavigationKeyPressed);
		}
	}
}

void UST_BaseUserWidget::OnNavigationKeyPressed()
{
	//UpdateButtonsState(false);
}

void UST_BaseUserWidget::OnButtonHovered()
{
	UpdateButtonsState(true);
}

void UST_BaseUserWidget::UpdateButtonsState(bool bIsMouseEvent)
{
	for (UST_MainButton* Button : WidgetButtons)
	{
		Button->UpdateState(bIsMouseEvent);
	}
}