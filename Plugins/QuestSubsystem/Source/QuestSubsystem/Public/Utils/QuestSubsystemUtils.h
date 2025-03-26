#pragma once

#include "UObject/Object.h"

class QuestSubsystemUtils
{
public:
	/**
	 * @brief Parses a JSON string and fills an object's properties by calling their respective setter functions.
	 *
	 * @param Object - The target UObject whose properties will be set.
	 * @param JSONParams - A JSON-formatted string containing key-value pairs that map to the object's properties.
	 *
	 * @note The function iterates through the properties of the provided object and attempts to match them with JSON fields.
	 * If a matching field is found:
	 * - It looks for a setter function with the name format "Set<PropertyName>".
	 * - If the setter exists, it calls it with the appropriate value extracted from the JSON.
	 * - For now supports int, float, string, bool, and int array properties.
	 */
	static void FillObjectParamsFromJSONString(UObject* Object, const FString& JSONParams);

	/**
	 * @brief Creates a JSON string representation of an object's properties that are marked with CPF_SaveGame.
	 *
	 * @param Object - The UObject containing properties to be serialized.
	 * @param ParentObject - (Optional) The parent object used for exporting property values.
	 * @return A JSON-formatted FString containing key-value pairs of the object's saveable properties.
	 *
	 * @note The function iterates through the properties of the given object and:
	 * - Checks if the property has the CPF_SaveGame flag.
	 * - Exports its value as a string and stores it in a JSON object.
	 * - Serializes the JSON object into a string and returns it.
	 */
	static FString CreateJSONStringForSaveGameParams(const UObject* Object, UObject* ParentObject = nullptr);
};

