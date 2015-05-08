#ifndef B1HIT_H

#define B1HIT_H

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"

class B1Hit : public G4VHit
{
    public:
        B1Hit(G4double _energy, G4ThreeVector _position, G4double _weight):
            energy(_energy),
            position(_position),
            weight(_weight) {}
        void Print() 
        { 
            G4cout << "Hit@\t" << energy << "\t" << position 
                << "\tWeight " << weight << G4endl; 
        } 
        G4double energy;
        G4ThreeVector position;
        G4double weight;

        inline void * operator new(size_t);
        inline void operator delete(void *aHit);
};

typedef G4THitsCollection<B1Hit> B1HitsCollection;

extern G4ThreadLocal G4Allocator<B1Hit>* B1HitAllocator;

inline void* B1Hit::operator new(size_t)
{
      if(!B1HitAllocator) 
      {
          B1HitAllocator = new G4Allocator<B1Hit>;
      }
      return (void *) B1HitAllocator->MallocSingle();
}

inline void B1Hit::operator delete(void *aHit)
{
      B1HitAllocator->FreeSingle((B1Hit*) aHit);
}

#endif
