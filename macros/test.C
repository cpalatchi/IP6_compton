void test(){

  TFile *_file0 = TFile::Open("../rootfiles/compton_10GeV_new10_10events.root");
  TTree *t = (TTree*)_file0->Get("DetectorTree");

  const Int_t Np=2;
  Int_t pID[Np] = {11, 22};


  const Int_t Ndet=22;
  TString Dets[Ndet] = {
    "Det_Q12EF_5_before_hits",
    "Det_Q12EF_5_after_hits",
    "Det_D21EF_5_before_hits",
    "Det_D21EF_5_after_hits",
    "Det_Q11EF_5_before_hits",
    "Det_Q11EF_5_after_hits",
    "Det_D22EF_5_before_hits",
    "Det_D22EF_5_after_hits",
    "Det_Q10EF_5_before_hits",
    "Det_Q10EF_5_after_hits",
    "Det_Q9EF_5_before_hits",
    "Det_Q9EF_5_after_hits",
    "Det_Q8EF_5_before_hits",
    "Det_Q8EF_5_after_hits",
    "Det_Q7EF_5_before_hits",
    "Det_Q7EF_5_after_hits",
    "Det_Q6EF_5_before_hits",
    "Det_Q6EF_5_after_hits",
    "Det_Q5EF_5_before_hits",
    "Det_Q5EF_5_after_hits",
    "Det_Q4EF_5_before_hits",
    "Det_Q4EF_5_after_hits"
};

  double cx,cy,cz,N;
  TH1D *h;
  TCanvas *c1 = new TCanvas("c1","c1", 0, 0, 500,500);

  for(int jj=0;jj<Np;jj++){
   for(int ii=0;ii<Ndet;ii++){
     cout<<pID[jj]<<" "<<Dets[ii].Data()<<" ";

  t->Draw(Form("%s.x>>hx%d_%d",Dets[ii].Data(),ii,jj),Form("%s.pID==%d",Dets[ii].Data(),pID[jj]));
  h=(TH1D*)gDirectory->FindObject(Form("hx%d_%d",ii,jj));
      cx=h->GetMean();
  cout<<cx<<" ";

    t->Draw(Form("%s.y>>hy%d_%d",Dets[ii].Data(),ii,jj),Form("%s.pID==%d",Dets[ii].Data(),pID[jj]));
    h=(TH1D*)gDirectory->FindObject(Form("hy%d_%d",ii,jj));
    cy=h->GetMean();
    cout<<cy<<" ";

    t->Draw(Form("%s.z>>hz%d_%d",Dets[ii].Data(),ii,jj),Form("%s.pID==%d",Dets[ii].Data(),pID[jj]));
    h=(TH1D*)gDirectory->FindObject(Form("hz%d_%d",ii,jj));
    cz=h->GetMean();
    cout<<cz<<" ";
    N=h->GetEntries();
    cout<<N<<endl;

  }
  }
  //  cout<<"help"<<endl;
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.pID");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.trackID");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.parentID");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.x");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.y");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.z");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.px");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.py");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.pz");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.e");//11,22
  //  DetectorTree->Draw("Det_Q9EF_5_before_hits.ke");//11,22
  //DetectorTree->Draw("Det_Q8EF_5_before_hits.y:Det_Q8EF_5_before_hits.x","abs(Det_Q8EF_5_before_hits.y)<3&&abs(Det_Q8EF_5_before_hits.x-185.5)<3&&Det_Q8EF_5_before_hits.pID==22","colz")
  // c1->SetLogz()



}
