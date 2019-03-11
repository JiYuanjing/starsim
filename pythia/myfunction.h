#include "TPDF.h"
void setPad(Double_t left, Double_t right, Double_t top, Double_t bottom, int color=10)
{
  gPad->SetFillColor(color);
  gPad->SetBorderMode(0);
  gPad->SetBorderSize(0);
  gPad->SetFrameFillColor(10);
  gPad->SetFrameBorderMode(0);
  gPad->SetFrameBorderSize(0);
  gPad->SetLeftMargin(left);
  gPad->SetRightMargin(right);
  gPad->SetTopMargin(top);
  gPad->SetBottomMargin(bottom);
}
void addpdf(TPDF* const  pdf,TCanvas* c)
{
  c->cd();
  pdf->On();
  c->Update();
  pdf->NewPage();
  pdf->Off();
}

void drawtitle(TPDF* pdf,TCanvas* c,string s){
  c->cd();
  c->Draw();
  setPad(0.1,0.1,0.05,0.12);
  TLatex t;
  t.SetTextSize(0.05);
  t.DrawText(0.2,0.5,s.c_str());
  TLatex la;
  la.SetTextSize(0.035);
  la.DrawText(0.1,0.3,(new TDatime())->AsSQLString());
  la.DrawText(0.1,0.2,"by Yuanjing");
  pdf->On();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}

TH1* RebinHist(TH1* h, double *point, int *rebin, int num){
  double oldwidth = h->GetBinWidth(1);     
  const int NUMMAX = h->GetNbinsX();
  double binedge[NUMMAX];
  binedge[0]=point[0];
  int ipoint=0;
  int ip;
  for (ip=1;ip<NUMMAX;ip++) {
    if (binedge[ip-1]<point[ipoint+1]&&binedge[ip-1]>=point[ipoint]){
       binedge[ip] = binedge[ip-1]+oldwidth*rebin[ipoint];}
    else {
        ipoint++; 
        binedge[ip] = binedge[ip-1]+oldwidth*rebin[ipoint];}
    if (binedge[ip]>point[num]) {binedge[ip]=point[num]; break;}
    // cout<<binedge[ip]<<endl;
  }
  return h->Rebin(ip,Form("%s_rebin",h->GetName()),binedge);
}
