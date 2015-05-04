//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1DetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"
#include "B1SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
    fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;

    //     
    // World
    //
    G4double world_sizeXY = 100 * cm;
    G4double world_sizeZ  = 100 * cm;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld =    
        new G4Box("World",                       //its name
                0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

    G4LogicalVolume* logicWorld =                         
        new G4LogicalVolume(solidWorld,          //its solid
                world_mat,           //its material
                "World");            //its name

    G4VPhysicalVolume* physWorld = 
        new G4PVPlacement(0,                     //no rotation
                G4ThreeVector(),       //at (0,0,0)
                logicWorld,            //its logical volume
                "World",               //its name
                0,                     //its mother  volume
                false,                 //no boolean operation
                0,                     //copy number
                checkOverlaps);        //overlaps checking

    //     
    // Mano
    //  
    G4Material* tissue = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
    G4ThreeVector pos1 = G4ThreeVector(0, 0, 20*cm);

    G4Box *solidmano= new G4Box("solidmano", 50.0*mm, 100.0*mm, 10.0*mm );
    G4LogicalVolume * mano = new G4LogicalVolume(solidmano,      //its solid
            tissue,         //its material
            "mano" ,         //its name
            0,0,0);

    //G4VPhysicalVolume *= new G4PVPlacement(
    new G4PVPlacement(
        0,
        G4ThreeVector(0, 0, 200*mm),
        mano,
        "mano",
        logicWorld,
        false,
        0,
        checkOverlaps);
    // Set Shape2 as scoring volume
    //
    fScoringVolume = mano;

    //
    //always return the physical World
    //
    return physWorld;
}

void B1DetectorConstruction::ConstructSDandField()
{
    B1SensitiveDetector *sdetector = new B1SensitiveDetector("sdetector");
    SetSensitiveDetector("mano", sdetector, false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
