// This is my training project.

#include "Thing.h"

// Sets default values
AThing::AThing()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create Link_1 and attach to Root
    Link_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Link_1"));
    SetRootComponent(Link_1);

    // Create Link_2 and attach to Link_1
    Link_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Link_2"));
    Link_2->SetupAttachment(Link_1);

    // Create physical Link_1
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Link_1Asset(TEXT("StaticMesh'/Game/Assets/SM_Link_1.SM_Link_1'"));
    if (Link_1Asset.Succeeded())
    {
        Link_1->SetStaticMesh(Link_1Asset.Object);
        Link_1->SetRelativeLocationAndRotation(FVector(0.f), FRotator(0.f));
        Link_1->SetSimulatePhysics(true);
        Link_1->SetLinearDamping(0.f);
        Link_1->SetAngularDamping(0.f);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Static Mesh %s not found."), *Link_1->GetName())
        return;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Link_2Asset(TEXT("StaticMesh'/Game/Assets/SM_Link_2.SM_Link_2'"));
    if (Link_2Asset.Succeeded())
    {
        Link_2->SetStaticMesh(Link_2Asset.Object);
        Link_2->SetRelativeLocationAndRotation(FVector(0.f, -13.585f, 0.f), FRotator(0.f, 0.f, 0.f));
        Link_2->SetSimulatePhysics(true);
        Link_2->SetLinearDamping(0.f);
        Link_2->SetAngularDamping(0.f);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Static Mesh %s not found."), *Link_2->GetName())
        return;
    }

    Joint_1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Joint_1"));
    Joint_1->SetupAttachment(Link_2);
    Joint_1->SetConstrainedComponents(Link_1, FName(TEXT("Link_1")), Link_2, FName(TEXT("Link_2")));
    Joint_1->SetRelativeLocationAndRotation(FVector(0.f), FRotator(0.f));
    Joint_1->ConstraintInstance.SetDisableCollision(true);
    Joint_1->ConstraintInstance.SetLinearXMotion(ELinearConstraintMotion::LCM_Locked);
    Joint_1->ConstraintInstance.SetLinearYMotion(ELinearConstraintMotion::LCM_Locked);
    Joint_1->ConstraintInstance.SetLinearZMotion(ELinearConstraintMotion::LCM_Locked);
    Joint_1->ConstraintInstance.SetAngularSwing1Motion(EAngularConstraintMotion::ACM_Locked);
    Joint_1->ConstraintInstance.SetAngularSwing2Motion(EAngularConstraintMotion::ACM_Free);
    Joint_1->ConstraintInstance.SetAngularTwistMotion(EAngularConstraintMotion::ACM_Locked);
}

// Called when the game starts or when spawned
void AThing::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AThing::Tick(float DeltaTime)
{

    UE_LOG(LogTemp, Warning, TEXT("At: %f"), GetGameTimeSinceCreation())
    Link_1->AddForce(FVector(0.f, 0.f, 10.f));

    if (Joint_1->IsBroken())
    {
        UE_LOG(LogTemp, Error, TEXT("Constraint is broken!!!"));
    }

    //AddAcceleration();
    Super::Tick(DeltaTime);
}

void AThing::AddAcceleration()
{
    Link_1->AddTorqueInDegrees(Link_1->GetComponentRotation().RotateVector(JointAcceleration * Link_1->GetInertiaTensor()));
    UE_LOG(LogTemp, Log, TEXT("AccIn: %s"), *JointAcceleration.ToString())
}