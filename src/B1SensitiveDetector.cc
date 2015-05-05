
#include "B1SensitiveDetector.hh"
#include "B1Hit.hh"
#include "G4SDManager.hh"

    B1SensitiveDetector::B1SensitiveDetector(G4String name)
: G4VSensitiveDetector(name)
{
    G4SDManager *sdman=G4SDManager::GetSDMpointer();
    sdman->AddNewDetector(this);
}

void B1SensitiveDetector::Initialize(G4HCofThisEvent *)
{
    const char collectionname[] = "hitscollection";
    fHitsCollection 
        = new B1HitsCollection(SensitiveDetectorName, collectionname); 
    // Add this collection in hce
    //G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionname);
    //hce->AddHitsCollection( hcID, fHitsCollection );
}


G4bool B1SensitiveDetector::ProcessHits(G4Step *step, 
        G4TouchableHistory *)
{
    B1Hit *newHit = new B1Hit(step->GetTotalEnergyDeposit(), 
            step->GetPostStepPoint()->GetPosition());
    fHitsCollection->insert( newHit );
    return true;
}

void B1SensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    if ( true ) { //verboseLevel>1 ) {
        G4int nofHits = fHitsCollection->entries();
        for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
    }
    }
