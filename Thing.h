// This is my training project.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// clang-format off
#include "Thing.generated.h"

UCLASS()
class TRAININGPROJECT_API AThing : public AActor
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    bool bGravityEnabled = false;

public:
    // Sets default values for this actor's properties
    AThing();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
