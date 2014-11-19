#include "StorageRegionHit.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <iomanip>
using namespace std;


G4ThreadLocal G4Allocator<StorageRegionHit>* StorageRegionHitAllocator=0;

StorageRegionHit::StorageRegionHit()
 : G4VHit(),
  particleName("ERROR"),
  muonID(-999),
  trackID(-999),
  pdgID(-999),
  nturn(-999),
  rglobal(-999.),
  uglobal(-999.),
  rlocal(-999.),
  ulocal(-999.),
  time(-999.),
  pos(G4ThreeVector(-999.,-999.,-999.)),
  mom(G4ThreeVector(-999.,-999.,-999.)),
  pol(G4ThreeVector(-999.,-999.,-999.))
{}


StorageRegionHit::~StorageRegionHit()
{}


StorageRegionHit::StorageRegionHit(const StorageRegionHit& right)
 : G4VHit()
{
  particleName = right.particleName;
  muonID       = right.muonID;
  trackID      = right.trackID;
  pdgID        = right.pdgID;
  nturn        = right.nturn;
  rglobal      = right.rglobal;
  uglobal      = right.rglobal;
  rlocal       = right.rlocal;
  ulocal       = right.rlocal;
  time         = right.time;
  pos          = right.pos;
  mom          = right.mom;
  pol          = right.pol;
}


const StorageRegionHit& StorageRegionHit::operator=(const StorageRegionHit& right)
{
  particleName = right.particleName;
  muonID       = right.muonID;
  trackID      = right.trackID;
  pdgID        = right.pdgID;
  nturn        = right.nturn;
  rglobal      = right.rglobal;
  uglobal      = right.rglobal;
  rlocal       = right.rlocal;
  ulocal       = right.rlocal;
  time         = right.time;
  pos          = right.pos;
  mom          = right.mom;
  pol          = right.pol;

  return *this;
}


G4int StorageRegionHit::operator==(const StorageRegionHit& right) const
{
  return (this==&right) ? 1 : 0;
}


void StorageRegionHit::Draw()
{}


void StorageRegionHit::Print()
{
  G4cout
    << "--------------------------------------------------" << G4endl
    << "STORAGE REGION HIT"                                 << G4endl
    << "--------------------------------------------------" << G4endl
    << "  particle: " << particleName   << G4endl
    << "    muonID: " << muonID         << G4endl
    << "   trackID: " << trackID        << G4endl
    << "     pdgID: " << pdgID          << G4endl
    << "     nturn: " << nturn          << G4endl
    << "   rglobal: " << rglobal/m      << " m"   << G4endl
    << "   uglobal: " << uglobal/degree << " deg" << G4endl
    << "    rlocal: " << rlocal/mm      << " mm"  << G4endl
    << "    ulocal: " << ulocal/degree  << " deg" << G4endl
    << "      time: " << time/ns        << " ns " << G4endl
    << "       pos: " << pos/mm         << " mm " << G4endl
    << "       mom: " << mom/rad        << " rad[muon] or MeV[electron]" << G4endl
    << "       pol: " << pol            << G4endl
    << G4endl;
}

