// macro to instantiate the Geant3 from within
// STAR  C++  framework and get the starsim prompt
// To use it do
//  root4star starsim.C
#include <algorithm>
#include <vector>
#include "Anacuts.h"
#include "myfunction.h"

class StarGenEvent;
// TString LHAPDF_DATA_PATH="/afs/cern.ch/sw/lcg/external/lhapdfsets/current/";
TString LHAPDF_DATA_PATH="/star/u/jiyj/sphenix/Lc/production/pythia/opendecay/pdf";
// ----------------------------------------------------------------------------
void starsim( Int_t nevents=3e5,  int mode = 1, char* rootfile = "test.root")
//mode = 0, no decay 
//mode =1, decay all
//mode =2, decay all except Lc
{ 
  TStopwatch* watch = new TStopwatch();
  watch->Start();
  gROOT->ProcessLine(".L bfc.C");
  {
    TString simple = "tables nodefault";
    bfc(0, simple );
  }

  gSystem->Load( "libVMC.so");
  gSystem->Load("St_g2t.so");
  gSystem->Load( "St_geant_Maker.so" );
  gSystem->Load( "StarGeneratorUtil.so");
  gSystem->Load( "StarGeneratorEvent.so");
  gSystem->Load( "StarGeneratorBase.so" );
  gSystem->Load( "libMathMore.so"   );  
  gSystem->Setenv("LHAPDF_DATA_PATH", LHAPDF_DATA_PATH.Data() );
  gSystem->Load( "/opt/star/$STAR_HOST_SYS/lib/libLHAPDF.so");
  gSystem->Load( "Pythia8_1_86.so"  );
  // gSystem->Load( "Pythia8_2_35.so"  );

  // Setup RNG seed and map all ROOT TRandom here
  StarRandom::capture();
  // StarRandom::seed( rngSeed );
  StarRandom::seed( (unsigned)time(NULL) );

  // Setup an event generator
  StarPythia8 *pythia8 = new StarPythia8();    
  {
    pythia8->SetFrame("CMS", 200.0);
    pythia8->SetBlue("proton");
    pythia8->SetYell("proton");            
    // pythia8->ReadFile("star_hf_tune_v1.1.cmnd");
    pythia8->Set("HardQCD:qqbar2ccbar=on");
    pythia8->Set("HardQCD:gg2ccbar=on");
  }

  // Initialize primary event generator and all sub makers
  pythia8->Init();
  // Trigger on nevents
  // std::vector<int> charm;
  // for (int i=0;i<20;i++){ charm.push_back(cbaryon[i]);}
  // for (int i=0;i<18;i++){ charm.push_back(cmesons[i]);}
  // for (int i=0;i<13;i++){ charm.push_back(cbaryon[i]);}
  // std::vector<int>::iterator it;
  TH1F* hcquark = new TH1F("hcquark","hcquark",400,-20,20);
  TH1F* hnparticles = new TH1F("hnparticles","nparticles",100,0,400);
  InitHists();
  for (int iev=0;iev<nevents;iev++){
    if (iev%10000==0) cout<<"run "<<iev<<endl;
    pythia8->Clear();
    pythia8->Make();
    // pythia8->Generate();
    int npar = pythia8->Event()->GetNumberOfParticles();
    hnparticles->Fill(npar);
    // cout<<"generate: ";
    // cout<<npar<<endl;
    StarGenEvent* evt = pythia8->Event();
    for (int ipar=0;ipar<npar;ipar++){
      StarGenParticle* p =  ((*evt)[ipar]);
      if (fabs(p->momentum().Rapidity())>=1) continue;
      int id = p->GetId();
      // cout<<id<<" ";
      int idxmom = p->GetFirstMother();
      int momid = ((*evt)[idxmom])->GetId();
      if (abs(momid)==4) {
        // if (abs(id) ==321 ) cout<<p->GetPx()<<" "<<p->GetPy()<<" "<<p->GetPz()<<endl;
        int findparticle = getparticleIndex(id);
      // cout<<findparticle<<endl;
        if (findparticle > -1) hparticle[findparticle]->Fill(p->pt());
      }
      if (abs(id)==4) hcquark->Fill(p->pt()*(id/abs(id)));
      
    }
    // cout<<endl;
  }
  // Finish the chain
  pythia8->Finish();
  // chain->Finish();
  cout << "Writing File" << endl;
  TFile* mFile = new TFile(rootfile,"recreate");
  WriteFile(mFile);
  hcquark->Write();
  hnparticles->Write();
  mFile->Close();
  watch->Stop();
  // EXIT starsim
  time_t now2 = time(0);
  cout << "============================================================================" << endl;
  watch->Print();
  cout << "Program finished at: " << ctime(&now2);
  cout << "============================================================================" << endl;

}
