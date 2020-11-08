#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NoiseFilterFactory.generated.h"

class UBaseNoiseFilter;
struct FNoiseSettings;

UCLASS()
class SOLARSYSTEM_API UNoiseFilterFactory : public UObject
{
	GENERATED_BODY()

public:
	UBaseNoiseFilter* GetNoiseFilter(const FNoiseSettings& settings);
	
};
