#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLARSYSTEM_API UGravityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGravityComponent();

	UPROPERTY(EditAnywhere)
	float Mass;
	UPROPERTY(EditAnywhere)
	FVector InitialVelocity;
	UPROPERTY(EditAnywhere)
	bool ShouldDrawDebug;

	void UpdateVelocity(TArray<UGravityComponent*> allBodies, float deltaTime);
	void UpdatePosition(float deltaTime);

protected:
	virtual void BeginPlay() override;

private:
	FVector CurrentVelocity;
	float G;
	void DrawDebug();
};
