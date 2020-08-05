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
  if (bEnablePositionController)
  {
    PositionController();
  }
  UpdateJointForcesAndTorques();
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void URobotControllerComponent::Init()
{
  AActor* Model = GetOwner();
  if (Model)
  {
    Robot = Model->FindComponentByClass<URobotComponent>();
    for (int32 i = 0; i < Robot->GetNumberOfJoints(); ++i)
    {
      UStaticMeshComponent* Link = Robot->GetLink(i + 1);
      FVector CenterOfMassOffset = Link->GetComponentRotation().UnrotateVector(Link->GetCenterOfMass() - Link->GetComponentLocation());
      Link->SetCenterOfMass(-CenterOfMassOffset, NAME_None);
    }

    JointStates.Init(0.f, Robot->GetNumberOfJoints());
    JointStatesOffset.Init(0.f, Robot->GetNumberOfJoints());
    UpdateJointStates();
    //JointStatesOffset = JointStates;
    JointStates.Init(0.f, Robot->GetNumberOfJoints());
    DesiredJointStates.Init(0.f, Robot->GetNumberOfJoints());
    InJointTorques.Init(0.f, Robot->GetNumberOfJoints());
    OutJointTorques.Init(FVector(0.f), Robot->GetNumberOfJoints());
    OutJointForces.Init(FVector(0.f), Robot->GetNumberOfJoints());
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
    FRotator LinkParentRotation = Robot->GetLink(i)->GetComponentRotation();
    FRotator LinkChildRotation = Robot->GetLink(i + 1)->GetComponentRotation();
    FRotator LinkChildOriginRotation = UKismetMathLibrary::ComposeRotators(LinkChildRotation, Robot->GetLinkRotationOffset(i).GetInverse());
    FRotator LinkDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(LinkChildOriginRotation, LinkParentRotation).;
    Angle = LinkChildOriginRotation.GetManhattanDistance(LinkParentRotation);
    /* Robot->GetLink(i)->GetComponentRotation().UnrotateVector(Robot->GetLink(i + 1)->GetComponentRotation().Vector())
     ->GetRelativeTransform().GetRotation().ToAxisAndAngle(Axis, Angle); */

    /* if (Axis.X + Axis.Y + Axis.Z < 0.f)
    {
      Angle = -Angle;
    } */
    //JointStates[i] = Angle + JointStatesOffset[i];
    UE_LOG(LogTemp, Log, TEXT("JointStates[%d]: %f"), i, Angle)
    UE_LOG(LogTemp, Log, TEXT("Axis[%d]: %s"), i, *LinkDeltaRotation.ToString())
    JointStates[i] = Angle;
  }
}

void URobotControllerComponent::UpdateJointForcesAndTorques()
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
    FVector InJointTorque = Link->GetComponentRotation().RotateVector(AxisOrientation[i] * InJointTorques[i]);
    /* UE_LOG(LogTemp, Log, TEXT("InJointTorques[%d]: %s"), i, *InJointTorque.ToString())
    UE_LOG(LogTemp, Log, TEXT("CenterOfMass [%d]: %s"), i, *Link->GetCenterOfMass().ToString())
    UE_LOG(LogTemp, Log, TEXT("JointComponentLocation [%d]: %s"), i, *Link->GetComponentLocation().ToString())
    FVector CenterOfMassOffset = Link->GetCenterOfMass() - Link->GetComponentLocation();
    UE_LOG(LogTemp, Log, TEXT("CenterOfMassOffset [%d]: %s"), i, *CenterOfMassOffset.ToString()) */
    Link->AddTorqueInRadians(InJointTorque, NAME_None, true);
    Robot->GetJoint(i)->GetConstraintForce(OutJointForces[i], OutJointTorques[i]);
    DrawDebugLine(
        GetWorld(),
        Link->GetCenterOfMass(),
        Link->GetCenterOfMass() + FMath::Sqrt(FMath::Abs(InJointTorques[i])) * Link->GetComponentRotation().RotateVector(AxisOrientation[i]),
        FColor(0, 255, 0),
        false,
        0.f,
        0,
        1.f);
  }
}

void URobotControllerComponent::PositionController()
{
  for (int32 i = 0; i < JointStates.Num(); ++i)
  {
    InJointTorques[i] = (DesiredJointStates[i] - JointStates[i]) * 100;
  }
}