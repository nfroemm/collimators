#include "RootManager.hh"
#include "RootMessenger.hh"
#include <TFile.h>
#include <TTree.h>

#include "globals.hh"


RootManager::RootManager()
: rootMsgr_(0),
  outfile("x.root"),
  rootfile(0),
  tphits(0),
  srhits(0),
  colhits(0),
  //
  tp_muonID(0),
  tp_trackID(0),
  tp_pdgID(0),
  tp_nplane(0),
  tp_nturn(0),
  tp_rglobal(0.),
  tp_uglobal(0.),
  tp_rlocal(0.),
  tp_ulocal(0.),
  tp_t(0.),
  tp_x(0.),
  tp_xp(0.),
  tp_y(0.),
  tp_yp(0.),
  tp_z(0.),
  tp_dpp(0.),
  tp_sx(0.),
  tp_sy(0.),
  tp_sz(0.),
  //
  sr_muonID(0),
  sr_trackID(0),
  sr_pdgID(0),
  sr_nturn(0),
  sr_rglobal(0.),
  sr_uglobal(0.),
  sr_rlocal(0.),
  sr_ulocal(0.),
  sr_t(0.),
  sr_x(0.),
  sr_xp(0.),
  sr_y(0.),
  sr_yp(0.),
  sr_z(0.),
  sr_dpp(0.),
  sr_sx(0.),
  sr_sy(0.),
  sr_sz(0.),
  //
  // Collimator hits
  //
  col_muonID(0),
  col_trackID(0),
  col_pdgID(0),
  col_ncol(0),
  col_nturn(0),
  //
  col_r0g(0.),
  col_u0g(0.),
  col_r0l(0.),
  col_u0l(0.),
  col_t0(0.),
  col_x0(0.),
  col_xp0(0.),
  col_y0(0.),
  col_yp0(0.),
  col_z0(0.),
  col_dpp0(0.),
  col_p0mag(0.),
  //
  col_r1g(0.),
  col_u1g(0.),
  col_r1l(0.),
  col_u1l(0.),
  col_t1(0.),
  col_x1(0.),
  col_xp1(0.),
  col_y1(0.),
  col_yp1(0.),
  col_z1(0.),
  col_dpp1(0.),
  col_p1mag(0.)
{
  rootMsgr_ = new RootMessenger(this);
}


RootManager::~RootManager()
{
  if (rootfile) delete rootfile;
  if (rootMsgr_) delete rootMsgr_;
}


void RootManager::SetOutputFile(G4String str)
{
  outfile = str;
}


void RootManager::Book()
{
  rootfile = new TFile(outfile,"RECREATE");
  if (!rootfile) {
    G4cout << "PROBLEM CREATING ROOT FILE" << G4endl;
    return;
  }

  // Tracking plane hits
  tphits = new TTree("TrackingPlaneHits","TrackingPlaneHits");
    tphits->Branch( "muonID",  &tp_muonID,  "muonID/I" );
    tphits->Branch( "trackID", &tp_trackID, "trackID/I");
    tphits->Branch( "pdgID",   &tp_pdgID,   "pdgID/I"  );
    tphits->Branch( "nplane",  &tp_nplane,  "nplane/I" );
    tphits->Branch( "nturn",   &tp_nturn,   "nturn/F"  );
    tphits->Branch( "rglobal", &tp_rglobal, "rglobal/F");
    tphits->Branch( "uglobal", &tp_uglobal, "uglobal/F");
    tphits->Branch( "rlocal",  &tp_rlocal,  "rlocal/F" );
    tphits->Branch( "ulocal",  &tp_ulocal,  "ulocal/F" );
    tphits->Branch( "t",       &tp_t,       "t/F"      );
    tphits->Branch( "x",       &tp_x,       "x/F"      );
    tphits->Branch( "xp",      &tp_xp,      "xp/F"     );
    tphits->Branch( "y",       &tp_y,       "y/F"      );
    tphits->Branch( "yp",      &tp_yp,      "yp/F"     );
    tphits->Branch( "z",       &tp_z,       "z/F"      );
    tphits->Branch("dpp",      &tp_dpp,     "dpp/F"    );
    tphits->Branch( "sx",      &tp_sx,      "sx/F"     );
    tphits->Branch( "sy",      &tp_sy,      "sy/F"     );
    tphits->Branch( "sz",      &tp_sz,      "sz/F"     );

  srhits = new TTree("StorageRegionHits","StorageRegionHits");
    srhits->Branch( "muonID",  &sr_muonID,  "muonID/I" );
    srhits->Branch( "trackID", &sr_trackID, "trackID/I");
    srhits->Branch( "pdgID",   &sr_pdgID,   "pdgID/I"  );
    srhits->Branch( "nturn",   &sr_nturn,   "nturn/I"  );
    srhits->Branch( "rglobal", &sr_rglobal, "rglobal/F");
    srhits->Branch( "uglobal", &sr_uglobal, "uglobal/F");
    srhits->Branch( "rlocal",  &sr_rlocal,  "rlocal/F" );
    srhits->Branch( "ulocal",  &sr_ulocal,  "ulocal/F" );
    srhits->Branch( "t",       &sr_t,       "t/F"      );
    srhits->Branch( "x",       &sr_x,       "x/F"      );
    srhits->Branch( "xp",      &sr_xp,      "xp/F"     );
    srhits->Branch( "y",       &sr_y,       "y/F"      );
    srhits->Branch( "yp",      &sr_yp,      "yp/F"     );
    srhits->Branch( "z",       &sr_z,       "z/F"      );
    srhits->Branch("dpp",      &sr_dpp,     "dpp/F"    );
    srhits->Branch( "sx",      &sr_sx,      "sx/F"     );
    srhits->Branch( "sy",      &sr_sy,      "sy/F"     );
    srhits->Branch( "sz",      &sr_sz,      "sz/F"     );

  colhits  = new TTree("CollimatorHits","CollimatorHits" );
    colhits->Branch( "muonID",  &col_muonID,  "muonID/I" );
    colhits->Branch( "trackID", &col_trackID, "trackID/I");
    colhits->Branch( "pdgID",   &col_pdgID,   "pdgID/I"  );
    colhits->Branch( "ncol",    &col_ncol,    "ncol/I"   );
    colhits->Branch( "nturn",   &col_nturn,   "nturn/I"  );
    //
    colhits->Branch( "r0g",     &col_r0g,     "r0g/F"    );
    colhits->Branch( "u0g",     &col_u0g,     "u0g/F"    );
    colhits->Branch( "r0l",     &col_r0l,     "r0l/F"    );
    colhits->Branch( "u0l",     &col_u0l,     "u0l/F"    );
    colhits->Branch( "t0",      &col_t0,      "t0/F"     );
    colhits->Branch( "x0",      &col_x0,      "x0/F"     );
    colhits->Branch( "xp0",     &col_xp0,     "xp0/F"    );
    colhits->Branch( "y0",      &col_y0,      "y0/F"     );
    colhits->Branch( "yp0",     &col_yp0,     "yp0/F"    );
    colhits->Branch( "z0",      &col_z0,      "z0/F"     );
    colhits->Branch( "dpp0",    &col_dpp0,    "dpp0/F"   );
    colhits->Branch( "p0mag",   &col_p0mag,   "p0mag/F"  );
    //
    colhits->Branch( "r1g",     &col_r1g,     "r1g/F"    );
    colhits->Branch( "u1g",     &col_u1g,     "u1g/F"    );
    colhits->Branch( "r1l",     &col_r1l,     "r1l/F"    );
    colhits->Branch( "u1l",     &col_u1l,     "u1l/F"    );
    colhits->Branch( "t1",      &col_t1,      "t1/F"     );
    colhits->Branch( "x1",      &col_x1,      "x1/F"     );
    colhits->Branch( "xp1",     &col_xp1,     "xp1/F"    );
    colhits->Branch( "y1",      &col_y1,      "y1/F"     );
    colhits->Branch( "yp1",     &col_yp1,     "yp1/F"    );
    colhits->Branch( "z1",      &col_z1,      "z1/F"     );
    colhits->Branch( "dpp1",    &col_dpp1,    "dpp1/F"   );
    colhits->Branch( "p1mag",   &col_p1mag,   "p1mag/F"  );
}


void RootManager::Save()
{
  if (rootfile) {
    rootfile->Write();
    rootfile->Close();
    G4cout << "\n Writing ROOT ouput to \"" << outfile << "\"\n\n";
  }
}


void RootManager::FillTrackingPlaneTree(
  G4int muonID,
  G4int trackID,
  G4int pdgID,
  G4int nplane,
  G4double nturn,
  G4double rglobal,
  G4double uglobal,
  G4double rlocal,
  G4double ulocal,
  G4double time,
  G4ThreeVector pos,
  G4ThreeVector mom,
  G4ThreeVector pol)
{
  tp_muonID  = muonID;
  tp_trackID = trackID;
  tp_pdgID   = pdgID;
  tp_nplane  = nplane;
  tp_nturn   = float(nturn);
  tp_rglobal = float(rglobal);
  tp_uglobal = float(uglobal);
  tp_rlocal  = float(rlocal);
  tp_ulocal  = float(ulocal);
  tp_t   = float(time);
  tp_x   = float(pos.x());
  tp_y   = float(pos.y());
  tp_z   = float(pos.z());
  tp_xp  = float(mom.x());
  tp_yp  = float(mom.y());
  tp_dpp = float(mom.z());
  tp_sx  = float(pol.x());
  tp_sy  = float(pol.y());
  tp_sz  = float(pol.z());

  if (tphits) tphits->Fill();
}


void RootManager::FillStorageRegionTree(
  G4int muonID,
  G4int trackID,
  G4int pdgID,
  G4int nturn,
  G4double rglobal,
  G4double uglobal,
  G4double rlocal,
  G4double ulocal,
  G4double time,
  G4ThreeVector pos,
  G4ThreeVector mom,
  G4ThreeVector pol)
{
  sr_muonID  = muonID;
  sr_trackID = trackID;
  sr_pdgID   = pdgID;
  sr_nturn   = nturn;
  sr_rglobal = float(rglobal);
  sr_uglobal = float(uglobal);
  sr_rlocal  = float(rlocal);
  sr_ulocal  = float(ulocal);
  sr_t   = float(time);
  sr_x   = float(pos.x());
  sr_y   = float(pos.y());
  sr_z   = float(pos.z());
  sr_xp  = float(mom.x());
  sr_yp  = float(mom.y());
  sr_dpp = float(mom.z());
  sr_sx  = float(pol.x());
  sr_sy  = float(pol.y());
  sr_sz  = float(pol.z());

  if (srhits) srhits->Fill();
}


void RootManager::FillCollimatorTree(
  G4int muonID,
  G4int trackID,
  G4int pdgID,
  G4int ncol,
  G4int nturn,
  //
  G4double      r0g,
  G4double      u0g,
  G4double      r0l,
  G4double      u0l,
  G4double      t0,
  G4ThreeVector pos0,
  G4ThreeVector mom0,
  G4double      p0mag,
  //
  G4double      r1g,
  G4double      u1g,
  G4double      r1l,
  G4double      u1l,
  G4double      t1,
  G4ThreeVector pos1,
  G4ThreeVector mom1,
  G4double      p1mag)
{
  col_muonID  = muonID;
  col_trackID = trackID;
  col_pdgID   = pdgID;
  col_ncol    = ncol;
  col_nturn   = nturn;
  //
  col_r0g     = float(r0g);
  col_u0g     = float(u0g);
  col_r0l     = float(r0l);
  col_u0l     = float(u0l);
  col_t0      = float(t0);
  col_x0      = float(pos0.x());
  col_y0      = float(pos0.y());
  col_z0      = float(pos0.z());
  col_xp0     = float(mom0.x());
  col_yp0     = float(mom0.y());
  col_dpp0    = float(mom0.z());
  col_p0mag   = float(p0mag);
  //
  col_r1g     = float(r1g);
  col_u1g     = float(u1g);
  col_r1l     = float(r1l);
  col_u1l     = float(u1l);
  col_t1      = float(t1);
  col_x1      = float(pos1.x());
  col_y1      = float(pos1.y());
  col_z1      = float(pos1.z());
  col_xp1     = float(mom1.x());
  col_yp1     = float(mom1.y());
  col_dpp1    = float(mom1.z());
  col_p1mag   = float(p1mag);

  if (colhits) colhits->Fill();
}

