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
  UpdateJointStates(DeltaTime);
  if (bEnablePositionController)
  {
    PositionController(DeltaTime);
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
    JointStates.Num = Robot->GetNumberOfJoints();
    for (int32 i = 0; i < JointStates.Num; ++i)
    {
      UStaticMeshComponent* Link = Robot->GetLink(i + 1);
      FVector CenterOfMassOffset = Link->GetComponentRotation().UnrotateVector(Link->GetCenterOfMass() - Link->GetComponentLocation());
      Link->SetCenterOfMass(-CenterOfMassOffset, NAME_None);
      Link->Set
    }

    JointStates.Positions.Init(0.f, JointStates.Num);
    JointStates.Velocities.Init(0.f, JointStates.Num);
    JointStates.Torques.Init(FVector::ZeroVector, JointStates.Num);
    JointStates.Forces.Init(FVector::ZeroVector, JointStates.Num);

    bEnablePositionController = true;

    DesiredJointStates = JointStates;
    InJointTorques.Init(0.f, JointStates.Num);

    RobotControllerParameters.K_p.Init(1.f, JointStates.Num);
    RobotControllerParameters.K_d.Init(1.f, JointStates.Num);
    RobotControllerParameters.ErrorPositions.Init(0.f, JointStates.Num);
    RobotControllerParameters.ErrorVelocities.Init(0.f, JointStates.Num);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Model not found."))
    return;
  }
}

void URobotControllerComponent::UpdateJointStates(const float& DeltaTime)
{
  float Angle;
  TArray<int32> JointTypes;
  JointTypes = {1, 2, 2, 2, 1, 2};
  for (int32 i = 0; i < JointStates.Num; ++i)
  {
    /* UE_LOG(LogTemp, Log, TEXT("JointStates[%d]Twist: %f"), i, Robot->GetJoint(i)->ConstraintInstance.GetCurrentTwist())
    UE_LOG(LogTemp, Log, TEXT("JointStates[%d]Swing1: %f"), i, Robot->GetJoint(i)->ConstraintInstance.GetCurrentSwing1())
    UE_LOG(LogTemp, Log, TEXT("JointStates[%d]Swing2: %f"), i, Robot->GetJoint(i)->ConstraintInstance.GetCurrentSwing2()) */
    switch (JointTypes[i])
    {
    case 0:
      Angle = FMath::RadiansToDegrees(Robot->GetJoint(i)->ConstraintInstance.GetCurrentTwist());
      break;
    case 1:
      Angle = FMath::RadiansToDegrees(Robot->GetJoint(i)->ConstraintInstance.GetCurrentSwing1());
      break;
    case 2:
      Angle = FMath::RadiansToDegrees(Robot->GetJoint(i)->ConstraintInstance.GetCurrentSwing2());
      break;
    default:
      Angle = 0.f;
      break;
    }
    if (FMath::Abs(Angle - JointStates.Positions[i]) > 180.f)
    {
      Angle = Angle + (Angle < 0.f ? 360.f : -360.f);
    }
    
    JointStates.Velocities[i] = (Angle - JointStates.Positions[i])/DeltaTime;
    JointStates.Positions[i] = Angle;
  }
}

void URobotControllerComponent::UpdateJointForcesAndTorques()
{
  // TODO: Remove hardcode
  TArray<FVector> AxisOrientation;
  AxisOrientation.Emplace(0.f, 0.f, 1.f);
  AxisOrientation.Emplace(0.f, 1.f, 0.f);
  AxisOrientation.Emplace(0.f, 1.f, 0.f);
  AxisOrientation.Emplace(0.f, 1.f, 0.f);
  AxisOrientation.Emplace(0.f, 0.f, 1.f);
  AxisOrientation.Emplace(0.f, 1.f, 0.f);
  for (int32 i = 0; i < JointStates.Num; ++i)
  {
    UStaticMeshComponent* Link = Robot->GetLink(i + 1);
    FVector InJointTorque = Link->GetComponentRotation().RotateVector(AxisOrientation[i] * InJointTorques[i]);
    /* UE_LOG(LogTemp, Log, TEXT("InJointTorques[%d]: %s"), i, *InJointTorque.ToString())
    UE_LOG(LogTemp, Log, TEXT("CenterOfMass [%d]: %s"), i, *Link->GetCenterOfMass().ToString())
    UE_LOG(LogTemp, Log, TEXT("JointComponentLocation [%d]: %s"), i, *Link->GetComponentLocation().ToString())
    FVector CenterOfMassOffset = Link->GetCenterOfMass() - Link->GetComponentLocation();
    UE_LOG(LogTemp, Log, TEXT("CenterOfMassOffset [%d]: %s"), i, *CenterOfMassOffset.ToString()) */
    Link->AddTorqueInRadians(InJointTorque, NAME_None, true);
    Robot->GetJoint(i)->GetConstraintForce(JointStates.Forces[i], JointStates.Torques[i]);
    DrawDebugLine(
        GetWorld(),
        Link->GetCenterOfMass(),
        Link->GetCenterOfMass() + InJointTorques[i] * Link->GetComponentRotation().RotateVector(AxisOrientation[i]),
        FColor(0, 255, 0),
        false,
        0.f,
        0,
        1.f);
  }
}

void URobotControllerComponent::PositionController(const float& DeltaTime)
{
  for (int32 i = 0; i < JointStates.Num; ++i)
  {
    RobotControllerParameters.ErrorPositions[i] = JointStates.Positions[i] - DesiredJointStates.Positions[i];
    RobotControllerParameters.ErrorVelocities[i] = JointStates.Velocities[i] - DesiredJointStates.Velocities[i];
    InJointTorques[i] = RobotControllerParameters.ErrorPositions[i] * RobotControllerParameters.K_p[i] + RobotControllerParameters.ErrorVelocities[i] * RobotControllerParameters.K_d[i];
  }
}