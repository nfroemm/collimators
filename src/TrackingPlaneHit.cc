#include "TrackingPlaneHit.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <iomanip>
using namespace std;


G4ThreadLocal G4Allocator<TrackingPlaneHit>* TrackingPlaneHitAllocator=0;

TrackingPlaneHit::TrackingPlaneHit()
 : G4VHit(),
  particleName("ERROR"),
  muonID(-999),
  trackID(-999),
  pdgID(-999),
  nplane(-999),
  nturn(-999.),
  rglobal(-999.),
  uglobal(-999.),
  rlocal (-999.),
  ulocal (-999.),
  time(-999.),
  pos(G4ThreeVector(-999.,-999.,-999.)),
  mom(G4ThreeVector(-999.,-999.,-999.)),
  pol(G4ThreeVector(-999.,-999.,-999.))
{}


TrackingPlaneHit::~TrackingPlaneHit()
{}


TrackingPlaneHit::TrackingPlaneHit(const TrackingPlaneHit& right)
 : G4VHit()
{
  particleName = right.particleName;
  muonID       = right.muonID;
  trackID      = right.trackID;
  pdgID        = right.pdgID;
  nplane       = right.nplane;
  nturn        = right.nturn;
  rglobal      = right.rglobal;
  uglobal      = right.uglobal;
  rlocal       = right.rlocal;
  ulocal       = right.ulocal;
  time         = right.time;
  pos          = right.pos;
  mom          = right.mom;
  pol          = right.pol;
}


const TrackingPlaneHit& TrackingPlaneHit::operator=(const TrackingPlaneHit& right)
{
  particleName = right.particleName;
  muonID       = right.muonID;
  trackID      = right.trackID;
  pdgID        = right.pdgID;
  nplane       = right.nplane;
  nturn        = right.nturn;
  rglobal      = right.rglobal;
  uglobal      = right.uglobal;
  rlocal       = right.rlocal;
  ulocal       = right.ulocal;
  time         = right.time;
  pos          = right.pos;
  mom          = right.mom;
  pol          = right.pol;

  return *this;
}


G4int TrackingPlaneHit::operator==(const TrackingPlaneHit& right) const
{
  return (this==&right) ? 1 : 0;
}


void TrackingPlaneHit::Draw()
{}


void TrackingPlaneHit::Print()
{
  G4cout 
    << "--------------------------------------------------" << G4endl
    << "TRACKING-PLANE HIT"                                 << G4endl
    << "--------------------------------------------------" << G4endl
    << "  particle: " << particleName   << G4endl
    << "    muonID: " << muonID         << G4endl
    << "   trackID: " << trackID        << G4endl
    << "     pdgID: " << pdgID          << G4endl
    << "    nplane: " << nplane         << G4endl
    << "     nturn: " << nturn          << G4endl
    << "   rglobal: " << rglobal/m      << " m"   << G4endl
    << "   uglobal: " << uglobal/degree << " deg" << G4endl
    << "    rlocal: " << rlocal/mm      << " mm"  << G4endl
    << "    ulocal: " << ulocal/degree  << " deg" << G4endl
    << "      time: " << time/ns        << " ns"  << G4endl
    << "       pos: " << pos/mm         << " mm"  << G4endl
    << "       mom: " << mom/rad        << " rad[muon] or MeV[electron]" << G4endl
    << "       pol: " << pol            << G4endl
    << G4endl;
}

