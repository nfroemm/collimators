#include "ParticleGun.hh"
#include "CollimatorConstruction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "PhaseSpace.hh"
#include <cmath>
#include <vector>
using namespace std;


ParticleGun::ParticleGun()
: G4VUserPrimaryGeneratorAction(),
  amu(0.),
  mmu(0.),
  gamma_magic(0.),
  beta_magic(0.),
  P_magic(0.),
  R_magic(0.),
  B_magic(0.),
  fParticleGun(0)
//outfile(0)
{
  // Create a new particle gun that shoots one particle at a time
  fParticleGun = new G4ParticleGun(1);

  // Set the default particle to mu+
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu+");
  fParticleGun->SetParticleDefinition(particle);

  // Calculate default kinematical quantities
  SetMagicParameters();

  // Open file for writing CollimatorGun phase space throughout run
//outfile = fopen("phsp_CollimatorGun.dat","w");
}


ParticleGun::~ParticleGun()
{
  delete fParticleGun;
//fclose(outfile);
}


void ParticleGun::SetMagicParameters()
{
  amu = 11659208.e-10;
  mmu = 105.6583715*MeV;
  gamma_magic = sqrt(1.+1./amu);
  beta_magic = tanh(acosh(gamma_magic));
  P_magic = beta_magic * gamma_magic * mmu;
  R_magic = 7.112*m;
  B_magic = ( P_magic/eV )/( R_magic/m * CLHEP::c_light/(m/s) );
}


void ParticleGun::GeneratePrimaries(G4Event* anEvent)
{
  // Get collimator geometry information
  G4double collRmin = CollimatorConstruction::Instance()->rmin;
  G4double collRmax = CollimatorConstruction::Instance()->rmax;
  G4double collDz   = CollimatorConstruction::Instance()->dz;
  std::vector<double> collRingAngle = CollimatorConstruction::Instance()->collRingAngle;

  // Randomly choose a collimator at which to shoot a muon
  // G4int collNum = G4RandFlat::shootInt( collRingAngle.size() ); // {0,1,2,3,4} for E989
  // G4double ringAngle = collRingAngle[collNum]*degree;

  // For now, first collimator only
  G4double ringAngle = collRingAngle[0]*degree;

  // Define some helper variables
  G4double xrand, xprand, yrand, yprand, trand, dpp_rand;
  xrand=xprand=yrand=yprand=trand=dpp_rand=0.;
  G4double etax = 7.112*m/( 1-0.182 );
  G4double dpp_max = collRmax/etax;


  G4int collGunType = 2;
  G4int pdistr = 1;


  // Choose how to shoot particles into the collimators
  if (collGunType==1) {
    // Vertical betatron only
    G4double sgn = G4RandFlat::shoot();
    if (sgn<0.5) {sgn=-1.;} else {sgn=1.;}
    yrand = sgn*( collRmin + ( collRmax-collRmin )*G4RandFlat::shoot() );
    // Generate a random momentum
    if (pdistr==1) { // Flat/Uniform
      dpp_rand = 2.*(G4RandFlat::shoot()-0.5) * dpp_max;
    } else if (pdistr==2) { // Gaussian
      dpp_rand = G4RandGauss::shoot() * 0.0015;
    }

  } else if (collGunType==2) {
    // Horizontal betatron only (outer edge of collimator)
    xrand  = collRmin + ( collRmax-collRmin )*G4RandFlat::shoot();
    // Generate a random momentum
    if (pdistr==1) { // Flat/Uniform
      dpp_rand = G4RandFlat::shoot() * dpp_max;
    } else if (pdistr==2) { // Gaussian
      dpp_rand = G4RandGauss::shoot() * 0.0015;
      if (dpp_rand<0.) dpp_rand*=-1.;
    }

  } else if (collGunType==3) {
    // Horizontal betatron only (inner edge of collimator)
    xrand  = -1.*( collRmin + ( collRmax-collRmin )*G4RandFlat::shoot() );
    // Generate a random momentum
    if (pdistr==1) { // Flat/Uniform
      dpp_rand = -G4RandFlat::shoot() * dpp_max;
    } else if (pdistr==2) { // Gaussian
      dpp_rand = G4RandGauss::shoot() * 0.0015;
      if (dpp_rand>0.) dpp_rand*=-1.;
    }

  } else {
    G4cout << "ERROR: Collimator Gun Type\n";
    throw;
  }



  // Transform position to global coordinates
  G4double x = xrand + 7.112*m;
  G4double y = yrand;
  G4double z = -(collDz+0.001*mm)/2.;
  G4ThreeVector pos(x,y,z);
  pos.rotateZ(90.*degree);
  pos.rotateY(90.*degree);
  pos.rotateZ(-ringAngle);

  // Transform momentum to global coordinates
  G4double px = xprand * P_magic;
  G4double py = yprand * P_magic;
  G4double pz = sqrt( pow(((1+dpp_rand)*P_magic),2) - px*px - py*py );
  G4ThreeVector mom(px,py,pz);
  mom.rotateZ(90.*degree);
  mom.rotateY(90.*degree);
  mom.rotateZ(-ringAngle);

  // Define the initial polarization
  G4ThreeVector pol = -mom/mom.mag();

  // Load the gun
  fParticleGun->SetParticleTime( trand );
  fParticleGun->SetParticlePosition( pos );
  fParticleGun->SetParticleMomentum( mom );
  fParticleGun->SetParticlePolarization( pol );

  // Fire
  fParticleGun->GeneratePrimaryVertex(anEvent);
}



#if 0
  // The transverse momentum acceptance of the ring is set by the quadrupole strength
  G4double ring_xpmax = (45./7112.)*sqrt(1-0.182);
  G4double ring_ypmax = (45./7112.)*sqrt( 0.182 );

  // Define semi-major axes of horizontal and vertical phase-space ellipses
  G4double rmin  = 40.*mm;
  G4double rmax  = 50.*mm;
  G4double xpmin = 0.9*ring_xpmax;
  G4double xpmax = 1.1*ring_xpmax;
  G4double ypmin = 0.9*ring_ypmax;
  G4double ypmax = 1.1*ring_ypmax;

  // Generate random transverse positions/momenta
  G4bool within_ellipses=false;
  do {
    // Define some random-number helpers
    G4double rand1 = G4RandFlat::shoot();
    G4double rand2 = G4RandFlat::shoot()*2*M_PI;
    G4double rand3 = G4RandFlat::shoot();
    G4double rand4 = G4RandFlat::shoot()*2*M_PI;
    // Calculate (x,y) from above random numbers
    xrand = ( rmin + ( rmax-rmin )*rand1 )*cos(rand2);
    yrand = ( rmin + ( rmax-rmin )*rand3 )*cos(rand4);
    // Make sure the random (x,y) are at the edge of the storage region (r=45mm)
    G4double rRand = sqrt(xrand*xrand + yrand*yrand);
    if (rRand>rmin) {
      if (rRand<rmax) {
        // Make sure the random (x',y') are at the edge of the ring acceptance
        xprand = ( xpmin + (xpmax-xpmin)*rand1 )*sin(rand2);
        yprand = ( ypmin + (ypmax-ypmin)*rand3 )*sin(rand4);
        within_ellipses = true;
      }
    }
  } while (within_ellipses==false);

  // Write the initial phase space to the output file
  //fprintf(outfile,"% 12.4e% 12.4e% 12.4e% 12.4e% 12.4e% 12.4e\n", xrand, 1000.*xprand, yrand, 1000.*yprand, time, dpp);
#endif



#if 0
  // The stuff below isn't important--it's just my playing around 
  // a bit, with an eye toward the future of g-2 simulations.

  // Backleg hole
  G4ThreeVector* phat = new G4ThreeVector(1,0,0);
  phat->rotateZ(1.25*degree);
  fParticleGun->SetParticleMomentumDirection(*phat);
  fParticleGun->SetParticlePosition(G4ThreeVector(-4.30000*m,7.14615*m,0));

  // General phase-space stuff
  int nmuons = 1e4;
  // Distribution functions
  double emitx_distr = 1;
  double emity_distr = 2;
  double p_distr     = 989;
  double t_distr     = 989;

  // Horizontal phase-space
  double emitx  = 40*mm*mrad;
  double betax  = 2.0*m;
  double alphax = 1.0;   // dimensionless
  double gammax = 0.0/m;
  double etax   = 1.0*m;
  double etapx  = 0.0;   // dimensionless
  double phix   = 0.0*rad;
  // Vertical phase-space
  double emity  = 40*mm*mrad;
  double betay  = 2.0*m;
  double alphay = 1.0;   // dimensionless
  double gammay = 0.0/m;
  double etay   = 1.0*m;
  double etapy  = 0.0;   // dimensionless
  double phiy   = 0.0*rad;
  // Longitudinal phase-space
  double p_width = 0.01; // dimensionless
  double t_width = 120*ns;

  // Setup the Twiss structs to pass to "PhaseSpace::CreatePhaseSpace()" subroutine
  // Horizontal phase-space
  twissx.emit_distr = emitx_distr;
  twissx.emit  = emitx;
  twissx.beta  = betax;
  twissx.alpha = alphax;
  twissx.gamma = gammax;
  twissx.eta   = etax;
  twissx.etap  = etapx;
  twissx.phi   = phix;
  // Vertical phase-space
  twissy.emit_distr = emity_distr;
  twissy.emit  = emity;
  twissy.beta  = betay;
  twissy.alpha = alphay;
  twissy.gamma = gammay;
  twissy.eta   = etay;
  twissy.etap  = etapy;
  twissy.phi   = phiy;
  // Longitudinal phase-space
  phsp.p_distr  = p_distr;
  phsp.p_width  = p_width;
  phsp.t_distr  = t_distr;
  phsp.t_width  = t_width;
  // Create the phase space based on user's inputs
  CreatePhaseSpace( nmuons, twissx, twissy, phsp );
#endif

