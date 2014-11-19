#include "CollimatorHit.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <iomanip>
using namespace std;


G4ThreadLocal G4Allocator<CollimatorHit>* CollimatorHitAllocator=0;

CollimatorHit::CollimatorHit()
 : G4VHit(),
  particleName("ERROR"),
  muonID(-999),
  trackID(-999),
  pdgID(-999),
  ncoll(-999),
  nturn(-999),
  //
  t0(-999.),
  x0(G4ThreeVector(-999.,-999.,-999.)),
  p0(G4ThreeVector(-999.,-999.,-999.)),
  p0mag(-999.),
  r0global(-999.),
  u0global(-999.),
  r0local(-999.),
  u0local(-999.),
  //
  t1(-999.),
  x1(G4ThreeVector(-999.,-999.,-999.)),
  p1(G4ThreeVector(-999.,-999.,-999.)),
  p1mag(-999.),
  r1global(-999.),
  u1global(-999.),
  r1local(-999.),
  u1local(-999.)
{}


CollimatorHit::~CollimatorHit()
{}


CollimatorHit::CollimatorHit(const CollimatorHit& right)
 : G4VHit()
{
  particleName = right.particleName;
  muonID   = right.muonID;
  trackID  = right.trackID;
  pdgID    = right.pdgID;
  nturn    = right.nturn;
  ncoll    = right.ncoll;
  t0       = right.t0;
  x0       = right.x0;
  p0       = right.p0;
  p0mag    = right.p0mag;
  r0global = right.r0global;
  u0global = right.u0global;
  r0local  = right.r0local;
  u0local  = right.u0local;
  t1       = right.t1;
  x1       = right.x1;
  p1       = right.p1;
  p1mag    = right.p1mag;
  r1global = right.r1global;
  u1global = right.u1global;
  r1local  = right.r1local;
  u1local  = right.u1local;
}


const CollimatorHit& CollimatorHit::operator=(const CollimatorHit& right)
{
  particleName = right.particleName;
  muonID   = right.muonID;
  trackID  = right.trackID;
  pdgID    = right.pdgID;
  nturn    = right.nturn;
  ncoll    = right.ncoll;
  t0       = right.t0;
  x0       = right.x0;
  p0       = right.p0;
  p0mag    = right.p0mag;
  r0global = right.r0global;
  u0global = right.u0global;
  r0local  = right.r0local;
  u0local  = right.u0local;
  t1       = right.t1;
  x1       = right.x1;
  p1       = right.p1;
  p1mag    = right.p1mag;
  r1global = right.r1global;
  u1global = right.u1global;
  r1local  = right.r1local;
  u1local  = right.u1local;

  return *this;
}


G4int CollimatorHit::operator==(const CollimatorHit& right) const
{
  return (this==&right) ? 1 : 0;
}


void CollimatorHit::Draw()
{}


void CollimatorHit::Print()
{
  G4cout
    << "==================================================" << G4endl
    << "COLLIMATOR HIT"                                     << G4endl
    << "==================================================" << G4endl
    << " particle: " << particleName    << G4endl
    << "   muonID: " << muonID          << G4endl
    << "  trackID: " << trackID         << G4endl
    << "    pdgID: " << pdgID           << G4endl
    << "    nturn: " << nturn           << G4endl 
    << "    ncoll: " << ncoll           << G4endl
    << "       t0: " << t0/ns           << " ns"  << G4endl
    << "       x0: " << x0/mm           << " mm"  << G4endl
    << "       p0: " << p0              << ""     << G4endl
    << "     |p0|: " << p0mag           << " MeV" << G4endl
    << " r0global: " << r0global/mm     << " mm"  << G4endl
    << " u0global: " << u0global/degree << " deg" << G4endl
    << "  r0local: " << r0local/mm      << " mm"  << G4endl
    << "  u0local: " << u0local/degree  << " deg" << G4endl
    << "       t1: " << t1/ns           << " ns"  << G4endl
    << "       x1: " << x1/mm           << " mm"  << G4endl
    << "       p1: " << p1              << ""     << G4endl
    << "     |p1|: " << p1mag/MeV       << " MeV" << G4endl
    << " r1global: " << r1global/mm     << " mm"  << G4endl
    << " u1global: " << u1global/degree << " deg" << G4endl
    << "  r1local: " << r1local/mm      << " mm"  << G4endl
    << "  u1local: " << u1local/degree  << " deg" << G4endl
    << G4endl;
}

