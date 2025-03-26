#include "Utils/QuestSubsystemUtils.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Json.h"
#include "JsonUtilities.h"

void QuestSubsystemUtils::FillObjectParamsFromJSONString(UObject* Object, const FString& JSONParams)
{
	if (!Object)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystemUtils::FillObjectParamsFromJSONString: Object is null"));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONParams);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystemUtils::FillObjectParamsFromJSONString: Failed to parse JSON: %s"), *JSONParams);
		return;
	}

	for (TFieldIterator<FProperty> PropIt(Object->GetClass()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		FString PropertyName = Property->GetName();

		if (!JsonObject->HasField(PropertyName))
		{
			continue;
		}

		FString SetterFunctionName = "Set" + PropertyName;
		UFunction* SetterFunction = Object->FindFunction(FName(SetterFunctionName));
		if (!SetterFunction)
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestSubsystemUtils::FillObjectParamsFromJSONString: Setter function %s not found on %s"), *SetterFunctionName, *Object->GetName());
			return;
		}

		if (FIntProperty* IntProp = CastField<FIntProperty>(Property))
		{
			struct { int32 Value; } ParamsStruct;
			ParamsStruct.Value = JsonObject->GetIntegerField(PropertyName);
			Object->ProcessEvent(SetterFunction, &ParamsStruct);
		}
		else if (FStrProperty* StrProp = CastField<FStrProperty>(Property))
		{
			struct { FString Value; } ParamsStruct;
			ParamsStruct.Value = JsonObject->GetStringField(PropertyName);
			Object->ProcessEvent(SetterFunction, &ParamsStruct);
		}
		else if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Property))
		{
			struct { float Value; } ParamsStruct;
			ParamsStruct.Value = JsonObject->GetNumberField(PropertyName);
			Object->ProcessEvent(SetterFunction, &ParamsStruct);
		}
		else if (FBoolProperty* BoolProp = CastField<FBoolProperty>(Property))
		{
			struct { float Value; } ParamsStruct;
			ParamsStruct.Value = JsonObject->GetBoolField(PropertyName);
			Object->ProcessEvent(SetterFunction, &ParamsStruct);
		}
		else if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(Property))
		{
			const TArray<TSharedPtr<FJsonValue>>* JsonArray = nullptr;
			bool bResult = JsonObject->TryGetArrayField(PropertyName, JsonArray);
			if (!bResult)
			{
				UE_LOG(LogTemp, Warning, TEXT("QuestSubsystemUtils::FillObjectParamsFromJSONString: Failed to parse JSON array: %s"), *PropertyName);
				continue;
			}

			if (FIntProperty* InnerIntProp = CastField<FIntProperty>(ArrayProp->Inner))
			{
				struct { TArray<int32> ValueArray; } ParamsStruct;

				FScriptArrayHelper ArrayHelper(ArrayProp, ArrayProp->ContainerPtrToValuePtr<void>(Object));
				ArrayHelper.Resize(JsonArray->Num());
				for (int32 i = 0; i < JsonArray->Num(); i++)
				{
					ParamsStruct.ValueArray.Add(static_cast<int32>((*JsonArray)[i]->AsNumber()));
				}

				Object->ProcessEvent(SetterFunction, &ParamsStruct);
			}
		}
	}
}

FString QuestSubsystemUtils::CreateJSONStringForSaveGameParams(const UObject* Object, UObject* ParentObject)
{
	FString ResultString;

	if (!IsValid(Object))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestSubsystemUtils::CreateJSONStringForSaveGameParams: Object is null!"));
		return ResultString;
	}

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	for (TFieldIterator<FProperty> PropIt(Object->GetClass()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		if (!Property->HasAnyPropertyFlags(CPF_SaveGame))
		{
			continue;
		}
			
		FString PropertyValue;
		Property->ExportTextItem_Direct(PropertyValue, Property->ContainerPtrToValuePtr<void>(Object), nullptr, ParentObject, PPF_None);

		JsonObject->SetStringField(Property->GetName(), PropertyValue);
	}
	
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResultString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return ResultString;
}