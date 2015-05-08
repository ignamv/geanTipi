
#include "B1SensitiveDetector.hh"
#include "B1Hit.hh"
#include "G4SDManager.hh"
#include "Analysis.hh"
#include <vector>

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
            step->GetPreStepPoint()->GetPosition(),
            step->GetPreStepPoint()->GetWeight());
    fHitsCollection->insert( newHit );
    return true;
}

void B1SensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
  G4int nofHits = fHitsCollection->entries();
  if (nofHits == 0)
  {
      return;
  }
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  std::vector<B1Hit*> *vec = fHitsCollection->GetVector();
  for ( std::vector<B1Hit*>::iterator it = vec->begin(); it != vec->end(); it++)
  {
      man->FillNtupleDColumn(0, (*it)->energy);
      man->FillNtupleDColumn(1, (*it)->position[0]);
      man->FillNtupleDColumn(2, (*it)->position[1]);
      man->FillNtupleDColumn(3, (*it)->position[2]);
      man->FillNtupleDColumn(4, (*it)->weight);
      man->AddNtupleRow(); 
  }
}
