// This is my training project.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "UObject/ConstructorHelpers.h"
// clang-format off
#include "RobotComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TRAININGPROJECT_API URobotComponent : public UActorComponent
{
  GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere)
  bool bGravityEnabled;

  virtual void Init();

protected:
  virtual void AddLink(FName LinkName, const TCHAR* LinkMesh, FVector Location, FRotator Rotation);
  virtual void AddJoint(FName JointName, FVector Location, FRotator Rotation, EAngularConstraintMotion X, EAngularConstraintMotion Y, EAngularConstraintMotion Z);

  UPROPERTY(VisibleAnywhere)
  TArray<UStaticMeshComponent*> Links;

  UPROPERTY(VisibleAnywhere)
  TArray<UPhysicsConstraintComponent*> Joints;

  UPROPERTY(VisibleAnywhere)
  AActor* Model;

public:
  // Sets default values for this component's properties
  URobotComponent();

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
