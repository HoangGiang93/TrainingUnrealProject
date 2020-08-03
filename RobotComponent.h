// This is my training project.

#pragma once

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
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
  bool bEnableGravity;

  UPROPERTY(EditAnywhere)
  bool bFixBase;

  FORCEINLINE virtual UStaticMeshComponent* GetLink(const int32& i) const { return Links[i]; };
  FORCEINLINE virtual UPhysicsConstraintComponent* GetJoint(const int32& i) const { return Joints[i]; };
  FORCEINLINE virtual int32 GetNumberOfJoints() const { return Joints.Num(); };

  virtual void Init();
  virtual void SetFixBase();

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
