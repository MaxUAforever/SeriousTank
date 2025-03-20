#include "Components/QuestProvider.h"

#include "QuestSubsystem.h"

void UQuestProvider::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		UE_LOG(LogQuestSubsystem, Error, TEXT("UQuestProvider::BeginPlay: Owner is not valid!"));
		return;
	}

	UQuestSubsystem* QuestSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!IsValid(QuestSubsystem))
	{
		UE_LOG(LogQuestSubsystem, Warning, TEXT("UQuestProvider::BeginPlay: QuestSubsystem is not valid!"));
		return;
	}

	QuestSubsystem->RegisterQuestProvider(this);
}
