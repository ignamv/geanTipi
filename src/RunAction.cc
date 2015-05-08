#include "G4Run.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include <string>

RunAction::RunAction()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    // Create ntuple
    man->CreateNtuple("Name", "Title");
    man->CreateNtupleDColumn("E");
    man->CreateNtupleDColumn("x");
    man->CreateNtupleDColumn("y");
    man->CreateNtupleDColumn("z");
    man->CreateNtupleDColumn("weight");
    man->FinishNtuple();
}

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();  
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    std::ostringstream filename;
    filename << "Run";
    filename.fill('0');
    filename.width(6);
    filename << run->GetRunID();
    G4AnalysisManager::Instance()->OpenFile(filename.str());
}

void RunAction::EndOfRunAction(const G4Run* )
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}
