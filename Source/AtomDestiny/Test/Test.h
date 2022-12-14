#pragma once

#include "CoreMinimal.h"
#include "Test.generated.h"

USTRUCT(BlueprintType)
struct FMyStruct0
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Vars")
	int myPropertyInt;

	FMyStruct0()
	{
		myPropertyInt = 4;
	}
};