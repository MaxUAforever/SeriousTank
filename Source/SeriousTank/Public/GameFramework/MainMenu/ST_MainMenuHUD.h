#pragma once

#include "GameFramework/ST_BaseHUD.h"
#include "ST_MainMenuHUD.generated.h"

UCLASS()
class SERIOUSTANK_API AST_MainMenuHUD : public AST_BaseHUD
{
	GENERATED_BODY()
	
public:
	virtual void SetSettingsWidgetVisible(bool bShowSettings) override;
};
