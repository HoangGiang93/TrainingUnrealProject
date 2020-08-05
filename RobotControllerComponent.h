// This is my training project.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "RobotComponent.h"
// clang-format off
#include "RobotControllerComponent.generated.h"
// clang-format on

USTRUCT()
struct FJointStates
{
  GENERATED_BODY()
public:
  UPROPERTY(VisibleAnywhere)
  int32 Num;

  UPROPERTY(EditAnywhere)
  TArray<float> Positions;

  UPROPERTY(VisibleAnywhere)
  TArray<float> Velocities;

  UPROPERTY(VisibleAnywhere)
  TArray<FVector> Torques;

  UPROPERTY(VisibleAnywhere)
  TArray<FVector> Forces;
};

USTRUCT()
struct FRobotControllerParameters
{
  GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere)
  TArray<float> K_p;

  UPROPERTY(EditAnywhere)
  TArray<float> K_d;

  UPROPERTY(VisibleAnywhere)
  TArray<float> ErrorPositions;

  UPROPERTY(VisibleAnywhere)
  TArray<float> ErrorVelocities;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) class TRAININGPROJECT_API URobotControllerComponent : public UActorComponent
{
  GENERATED_BODY()
public:
  virtual void Init();

  UPROPERTY(EditAnywhere)
  TArray<float> InJointTorques;

  UPROPERTY(EditAnywhere)
  FJointStates DesiredJointStates;

  UPROPERTY(EditAnywhere)
  bool bEnablePositionController;

protected:
  virtual void PositionController(const float& DeltaTime);
  virtual void UpdateJointForcesAndTorques();
  virtual void UpdateJointStates(const float& DeltaTime);

  UPROPERTY(VisibleAnywhere)
  URobotComponent* Robot;

  UPROPERTY(VisibleAnywhere)
  FJointStates JointStates;

  UPROPERTY(VisibleAnywhere)
  FRobotControllerParameters RobotControllerParameters;

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
