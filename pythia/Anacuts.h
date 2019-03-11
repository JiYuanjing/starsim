#include "TH1.h"
#include "TH2.h"
#include <map>
#include <string>
#include <vector>
#include <map>
using namespace std;
namespace Cuts{
  double eta = 1;
  double rapidity = 1;
}
// std::map<int,std::string> ParticleName = {
  // {4122,"Lc"},{411,"Dplus"},{421,"D0"},{413,"Dstar"},{431,"Ds"}
// }; 

int cbaryon[20]={4122,4222,4212,4112,4214,4232,4132,4322,4312,4324,4314,4332,4334,4412,4422,4414,4424,4432,4434,4444};
int cmesons[18]={411,421,10411,10421,413,423,10413,10423,20413,20423,415,425,431,10431,433,10433,20433,435};
int ccbar[13]={441,10441,100441,443,10443,20443,100443,30443,9000443,9010443,9020443,445,100445};

int const NParticle = 8;
string  charmsname[NParticle]={"D0","Dstar","Dplus","Ds","Lc","K","p","pi"};
int pIndex[NParticle]={421,413,411,431,4122,321,2212,211};
TH1F* hparticle[NParticle];
//used for analysis
void InitHists(){
  int ip=0;
  cout<<"start initialize hists..."<<endl;
  for (ip=0;ip<NParticle;ip++){
    hparticle[ip] = new TH1F(Form("h%s",charmsname[ip].c_str()), Form("%s;p_{T}(GeV);Counts",charmsname[ip].c_str()),200,0,20);
    hparticle[ip]->SetDirectory(0);
    cout<<"initialize "<<charmsname[ip]<<endl;
  }
}
void WriteFile(TFile* f){
  f->cd();
  for (int ip=0;ip<NParticle;ip++){
    hparticle[ip]->Write();
  }
}

int getparticleIndex(int id){
  for (int i=0;i<NParticle;i++){
    if (id==pIndex[i]) return i;
  }
    return -1;

}
