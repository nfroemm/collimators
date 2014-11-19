#ifndef ParticleGun_h
#define ParticleGun_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "PhaseSpace.hh"


class G4Event;
class G4ParticleGun;


class ParticleGun : public G4VUserPrimaryGeneratorAction
{
  public:
    ParticleGun();    
    virtual ~ParticleGun();

    void SetMagicParameters();
    virtual void GeneratePrimaries(G4Event*);         

    Twiss twissx; // user's inputs
    Twiss twissy; // user's inputs
    PhaseSpace phsp; // phase-space created

  private:
    G4double amu;
    G4double mmu;
    G4double gamma_magic;
    G4double beta_magic;
    G4double P_magic;
    G4double R_magic;
    G4double B_magic;
    G4ParticleGun* fParticleGun;
//  FILE* outfile;
};

#endif
