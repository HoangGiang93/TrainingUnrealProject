// This is my training project.

#include "RobotControllerComponent.h"

// Sets default values for this component's properties
URobotControllerComponent::URobotControllerComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  Init();
}

// Called when the game starts
void URobotControllerComponent::BeginPlay()
{
  Super::BeginPlay();

  // ...
}

// Called every frame
void URobotControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  UpdateJointStates();
  AddJointTorques();
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void URobotControllerComponent::Init()
{
  AActor* Model = GetOwner();
  if (Model)
  {
    Robot = Model->FindComponentByClass<URobotComponent>();
    JointStates.Init(0.f, Robot->GetNumberOfJoints());
    JointTorques.Init(0.f, Robot->GetNumberOfJoints());
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Model not found."))
    return;
  }
}

void URobotControllerComponent::UpdateJointStates()
{
  FVector Axis;
  float Angle;

  for (int32 i = 0; i < JointStates.Num(); ++i)
  {
    Cast<USceneComponent>(Robot->GetLink(i + 1))->GetComponentRotation().Quaternion().ToAxisAndAngle(Axis, Angle);
    if (Axis.X + Axis.Y + Axis.Z < 0.f)
    {
      Angle = -Angle;
    }
    JointStates[i] = Angle;
  }
}

void URobotControllerComponent::AddJointTorques()
{
  TArray<FVector> AxisOrientation;
  AxisOrientation.Emplace(0.f, 0.f, 1.f);
  AxisOrientation.Emplace(0.f, 1.f, 0.f);
  AxisOrientation.Emplace(0.f, 1.f, 0.f);
  AxisOrientation.Emplace(0.f, 1.f, 0.f);
  AxisOrientation.Emplace(0.f, 0.f, 1.f);
  AxisOrientation.Emplace(0.f, 1.f, 0.f);
  for (int32 i = 0; i < JointStates.Num(); ++i)
  {
    UStaticMeshComponent* Link = Robot->GetLink(i + 1);
    Link->AddTorqueInDegrees(Link->GetComponentRotation().RotateVector(AxisOrientation[i] * JointTorques[i]));
  }
}