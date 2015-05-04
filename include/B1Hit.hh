#ifndef B1HIT_H

#define B1HIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"

class B1Hit : public G4VHit
{
    public:
        B1Hit(G4double _energy, G4ThreeVector _position):
            energy(_energy),
            position(_position) {}
        void Print() { 
            G4cout << "Hit@\t" << energy << "\t" << position.z() << G4endl; 
        } 
        G4double energy;
        G4ThreeVector position;
};

typedef G4THitsCollection<B1Hit> B1HitsCollection;

#endif
