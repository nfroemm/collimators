#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class G4VPhysicsConstructor;

class PhysicsList : public G4VModularPhysicsList
{
  public:
    PhysicsList();
    virtual ~PhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

    void SetCuts();
};

#endif
