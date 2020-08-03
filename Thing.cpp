// This is my training project.

#include "Thing.h"

// Sets default values
AThing::AThing()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
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

  /* for (auto& Link : Links)
  {
    Link->SetEnableGravity(bGravityEnabled);
  }

  for (int32 i = 0; i < Joints.Num(); ++i)
  {
    Joints[i]->GetConstraintForce(LinearForce[i], AngularForce[i]);
    Joints[i]->UpdateConstraintFrames();
  } */

  Super::Tick(DeltaTime);
}