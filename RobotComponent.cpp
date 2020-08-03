// This is my training project.

#include "RobotComponent.h"

// Sets default values for this component's properties
URobotComponent::URobotComponent()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  Model = GetOwner();
  Init();
}

// Called when the game starts
void URobotComponent::BeginPlay()
{
  SetFixBase();
  Super::BeginPlay();
}

// Called every frame
void URobotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  SetFixBase();
  for (auto& Link : Links)
  {
    Link->SetEnableGravity(bEnableGravity);
  }
  
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URobotComponent::Init()
{
  AddLink(FName(TEXT("Base")), TEXT("StaticMesh'/Game/Assets/SM_Base.SM_Base'"), FVector(0.f), FRotator(0.f));
  AddLink(FName(TEXT("Link_1")), TEXT("StaticMesh'/Game/Assets/SM_Link_1.SM_Link_1'"), FVector(0.f, 0.f, 8.9159f), FRotator(0.f));
  AddJoint(FName(TEXT("Joint_1")), FVector(0.f), FRotator(0.f), EAngularConstraintMotion::ACM_Locked, EAngularConstraintMotion::ACM_Locked, EAngularConstraintMotion::ACM_Free);
  AddLink(FName(TEXT("Link_2")), TEXT("StaticMesh'/Game/Assets/SM_Link_2.SM_Link_2'"), FVector(0.f, -13.585f, 0.f), FRotator(-90.f, 0.f, 0.f));
  AddJoint(FName(TEXT("Joint_2")), FVector(0.f), FRotator(0.f), EAngularConstraintMotion::ACM_Locked, EAngularConstraintMotion::ACM_Free, EAngularConstraintMotion::ACM_Locked);
  AddLink(FName(TEXT("Link_3")), TEXT("StaticMesh'/Game/Assets/SM_Link_3.SM_Link_3'"), FVector(0.f, 11.97f, 42.500f), FRotator(0.f));
  AddJoint(FName(TEXT("Joint_3")), FVector(0.f), FRotator(0.f), EAngularConstraintMotion::ACM_Locked, EAngularConstraintMotion::ACM_Free, EAngularConstraintMotion::ACM_Locked);
  AddLink(FName(TEXT("Link_4")), TEXT("StaticMesh'/Game/Assets/SM_Link_4.SM_Link_4'"), FVector(0.f, 0.f, 39.225f), FRotator(-90.f, 0.f, 0.f));
  AddJoint(FName(TEXT("Joint_4")), FVector(0.f), FRotator(0.f), EAngularConstraintMotion::ACM_Locked, EAngularConstraintMotion::ACM_Free, EAngularConstraintMotion::ACM_Locked);
  AddLink(FName(TEXT("Link_5")), TEXT("StaticMesh'/Game/Assets/SM_Link_5.SM_Link_5'"), FVector(0.f, -10.915f - 11.97f + 13.585f, 0.f), FRotator(0.f, 0.f, 0.f));
  AddJoint(FName(TEXT("Joint_5")), FVector(0.f), FRotator(0.f), EAngularConstraintMotion::ACM_Locked, EAngularConstraintMotion::ACM_Locked, EAngularConstraintMotion::ACM_Free);
  AddLink(FName(TEXT("Link_6")), TEXT("StaticMesh'/Game/Assets/SM_Link_6.SM_Link_6'"), FVector(0.f, 0.f, 9.465f), FRotator(0.f, 0.f, 0.f));
  AddJoint(FName(TEXT("Joint_6")), FVector(0.f), FRotator(0.f), EAngularConstraintMotion::ACM_Locked, EAngularConstraintMotion::ACM_Free, EAngularConstraintMotion::ACM_Locked);

  bEnableGravity = true;
  bFixBase = true;

  UE_LOG(LogTemp, Warning, TEXT("Finish initializing Robot."))
}

void URobotComponent::AddJoint(FName JointName, FVector Location, FRotator Rotation, EAngularConstraintMotion X, EAngularConstraintMotion Y, EAngularConstraintMotion Z)
{
  if (!Links.Num())
  {
    UE_LOG(LogTemp, Error, TEXT("Links not found"))
    return;
  }

  UStaticMeshComponent* LinkParent = Links.Last(1);
  UStaticMeshComponent* LinkChild = Links.Last();

  UPhysicsConstraintComponent* Joint = CreateDefaultSubobject<UPhysicsConstraintComponent>(JointName);
  Joint->SetupAttachment(LinkChild);
  Joint->SetConstrainedComponents(LinkParent, FName(*LinkParent->GetName()), LinkChild, FName(*LinkChild->GetName()));
  Joint->SetRelativeLocationAndRotation(Location, Rotation);
  Joint->ConstraintInstance.SetDisableCollision(true);
  Joint->ConstraintInstance.SetLinearXMotion(ELinearConstraintMotion::LCM_Locked);
  Joint->ConstraintInstance.SetLinearYMotion(ELinearConstraintMotion::LCM_Locked);
  Joint->ConstraintInstance.SetLinearZMotion(ELinearConstraintMotion::LCM_Locked);
  Joint->ConstraintInstance.SetAngularSwing1Motion(Z);
  Joint->ConstraintInstance.SetAngularSwing2Motion(Y);
  Joint->ConstraintInstance.SetAngularTwistMotion(X);

  Joints.Add(Joint);
}

void URobotComponent::AddLink(FName LinkName, const TCHAR* LinkMesh, FVector Location, FRotator Rotation)
{
  UStaticMeshComponent* Link = CreateDefaultSubobject<UStaticMeshComponent>(LinkName);
  if (Links.Num())
  {
    Link->SetupAttachment(Links.Last());
  }
  else
  {
    if (Model)
    {
      Model->SetRootComponent(Link);
    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT("Model Thing not found"))
      return;
    }
  }

  ConstructorHelpers::FObjectFinder<UStaticMesh> LinkAsset(LinkMesh);
  if (LinkAsset.Succeeded())
  {
    Link->SetStaticMesh(LinkAsset.Object);
    Link->SetRelativeLocationAndRotation(Location, Rotation);
    Link->SetSimulatePhysics(true);
    Link->SetLinearDamping(0.f);
    Link->SetAngularDamping(0.f);
    Link->SetEnableGravity(bEnableGravity);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Static Mesh %s not found."), *Link->GetName())
    return;
  }

  Links.Add(Link);
}

void URobotComponent::SetFixBase()
{
  if (Links.Num() && Links[0])
  {
    Links[0]->SetSimulatePhysics(!bFixBase);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Links or Links[0] not found."))
    return;
  }
}