// This is my training project.

#include "Thing.h"

// Sets default values
AThing::AThing()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create Base and attach to Root
    Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
    Base->SetupAttachment(RootComponent);
    Base->SetSimulatePhysics(true);
    Base->SetLinearDamping(0.f);
    Base->SetAngularDamping(0.f);

    // Create physical Base
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
    if (BaseAsset.Succeeded())
    {
        Base->SetStaticMesh(BaseAsset.Object);
        Base->SetRelativeScale3D(FVector(0.1f, 0.1f, 1.f));
        Base->SetRelativeLocation(FVector(0.f));
    }
}

// Called when the game starts or when spawned
void AThing::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AThing::Tick(float DeltaTime)
{
    UE_LOG(LogTemp, Warning, TEXT("At: %f"), GetGameTimeSinceCreation());
    //JointAccelerationOut = (Base->GetComponentRotation().RotateVector(Base->GetPhysicsAngularVelocityInDegrees()) - JointVelocity) / DeltaTime;
    AddAcceleration();
    Super::Tick(DeltaTime);
}

void AThing::AddAcceleration()
{
    Base->AddTorqueInDegrees(Base->GetComponentRotation().RotateVector(JointAcceleration * Base->GetInertiaTensor()));

    UE_LOG(LogTemp, Log, TEXT("AccIn: %s"), *JointAcceleration.ToString());
}