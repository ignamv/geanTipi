
#include "G4VSensitiveDetector.hh"
#include "B1Hit.hh"

class B1SensitiveDetector : public G4VSensitiveDetector
{
    public:
        B1SensitiveDetector(G4String name);
        G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
        void Initialize(G4HCofThisEvent*);
        void EndOfEvent(G4HCofThisEvent *);

    private:
        B1HitsCollection *fHitsCollection;

};

