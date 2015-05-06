#include "RunAction.hh"
#include "Analysis.hh"

RunAction::RunAction()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    // Create ntuple
    man->CreateNtuple("Name", "Title");
    man->CreateNtupleDColumn("E");
    man->CreateNtupleDColumn("x");
    man->CreateNtupleDColumn("y");
    man->CreateNtupleDColumn("z");
    man->FinishNtuple();
}

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();  
}

void RunAction::BeginOfRunAction(const G4Run* )
{
    G4AnalysisManager::Instance()->OpenFile("File");
}

void RunAction::EndOfRunAction(const G4Run* )
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}
