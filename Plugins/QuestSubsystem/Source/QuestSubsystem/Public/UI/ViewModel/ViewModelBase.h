#pragma once

#include "UObject/Object.h"
#include "ViewModelBase.generated.h"

class UUserWidget;

UCLASS()
class QUESTSUBSYSTEM_API UViewModelBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(UUserWidget* RelatedWidget) {};
	virtual void Deinitialize() {};
};
