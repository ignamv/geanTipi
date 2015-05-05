
#include "B1PhysicsList.hh"
#include "G4PhysicsListHelper.hh"
#include "G4Electron.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"

B1PhysicsList::B1PhysicsList():
    G4VModularPhysicsList()
{
    //RegisterPhysics(new G4EmLivermorePhysics());
    //RegisterPhysics(new G4EmPenelopePhysics());
    RegisterPhysics(new G4EmStandardPhysics());
}

