#ifndef RootManager_hh
#define RootManager_hh 1

#include "globals.hh"
#include "G4ThreeVector.hh"

class RootMessenger;
class TFile;
class TTree;

class RootManager
{
  public:
    RootManager();
   ~RootManager();

    void SetOutputFile(G4String);
    void Book();
    void Save();

  private:
    RootMessenger* rootMsgr_;
    G4String       outfile;
    TFile*         rootfile;
    TTree*         tphits;
    TTree*         srhits;
    TTree*         colhits;

    // Tracking plane hits
    int   tp_muonID;
    int   tp_trackID;
    int   tp_pdgID;
    int   tp_nplane;
    float tp_nturn;
    float tp_rglobal;
    float tp_uglobal;
    float tp_rlocal;
    float tp_ulocal;
    float tp_t;
    float tp_x;
    float tp_xp;
    float tp_y;
    float tp_yp;
    float tp_z;
    float tp_dpp;
    float tp_sx;
    float tp_sy;
    float tp_sz;

  public:
    void FillTrackingPlaneTree(
      G4int         muonID,
      G4int         trackID,
      G4int         pdgID,
      G4int         nplane,
      G4double      nturn,
      G4double      rglobal,
      G4double      uglobal,
      G4double      rlocal,
      G4double      ulocal,
      G4double      time,
      G4ThreeVector pos,
      G4ThreeVector mom,
      G4ThreeVector pol
    );

  private:
    // Storage region hits
    int   sr_muonID;
    int   sr_trackID;
    int   sr_pdgID;
    int   sr_nturn;
    float sr_rglobal;
    float sr_uglobal;
    float sr_rlocal;
    float sr_ulocal;
    float sr_t;
    float sr_x;
    float sr_xp;
    float sr_y;
    float sr_yp;
    float sr_z;
    float sr_dpp;
    float sr_sx;
    float sr_sy;
    float sr_sz;

  public:
    void FillStorageRegionTree(
      G4int         muonID,
      G4int         trackID,
      G4int         pdgID,
      G4int         nturn,
      G4double      rglobal,
      G4double      uglobal,
      G4double      rlocal,
      G4double      ulocal,
      G4double      time,
      G4ThreeVector pos,
      G4ThreeVector mom,
      G4ThreeVector pol
    );

  private:
    // Collimator hits
    int   col_muonID;
    int   col_trackID;
    int   col_pdgID;
    int   col_ncol;
    int   col_nturn;
    //
    float col_r0g;
    float col_u0g;
    float col_r0l;
    float col_u0l;
    float col_t0;
    float col_x0;
    float col_xp0;
    float col_y0;
    float col_yp0;
    float col_z0;
    float col_dpp0;
    float col_p0mag;
    //
    float col_r1g;
    float col_u1g;
    float col_r1l;
    float col_u1l;
    float col_t1;
    float col_x1;
    float col_xp1;
    float col_y1;
    float col_yp1;
    float col_z1;
    float col_dpp1;
    float col_p1mag;

  public:
    void FillCollimatorTree(
      G4int         muonID,
      G4int         trackID,
      G4int         pdgID,
      G4int         ncol,
      G4int         nturn,
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
      G4double      p1mag
    );
};

#endif
