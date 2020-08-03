// This is my training project.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotComponent.h"
// clang-format off
#include "RobotControllerComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TRAININGPROJECT_API URobotControllerComponent : public UActorComponent
{
  GENERATED_BODY()
public:
  virtual void Init();

  UPROPERTY(EditAnywhere)
  TArray<float> JointTorques;

  virtual void AddJointTorques();

protected:
  virtual void UpdateJointStates();

  UPROPERTY(VisibleAnywhere)
  URobotComponent* Robot;

  UPROPERTY(VisibleAnywhere)
  TArray<float> JointStates;

public:
  // Sets default values for this component's properties
  URobotControllerComponent();

protected:
  // Called when the game starts
  virtual void
  BeginPlay() override;

public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
