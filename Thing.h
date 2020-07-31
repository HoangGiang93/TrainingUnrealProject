// This is my training project.

#pragma once

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "UObject/ConstructorHelpers.h"
// clang-format off
#include "Thing.generated.h"

UCLASS()
class TRAININGPROJECT_API AThing : public AActor
{
    GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FVector JointAcceleration;

    UPROPERTY(EditAnywhere)
    FVector Force;

protected:
	void AddAcceleration();

    UPROPERTY(VisibleAnywhere)
    UPhysicsConstraintComponent* Joint_1;
	
	/* UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Base; */

    UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Link_1;

    UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Link_2;

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
