void plot2D(int En=5){

  TFile *_file0;
  if(En==5){
  _file0 = TFile::Open("../rootfiles/compton_5GeV_beam_synchscatt_1M.root");
  }
  if(En==10){
  _file0 = TFile::Open("../rootfiles/compton_10GeV_beam_synchscatt_1M.root");
  }
  if(En==18){
  _file0 = TFile::Open("../rootfiles/compton_18GeV_beam_synchscatt_1M.root");
  }

  //../rootfiles/compton_10GeV_beam_synchscatt_1M.root
  //../rootfiles/compton_18GeV_beam_synchscatt_1M.root
  //../rootfiles/compton_5GeV_beam_synchscatt_1M.root

  TTree *t = (TTree*)_file0->Get("DetectorTree");

  const Int_t Np=2;
  Int_t pID[Np] = {11, 22};

  const Int_t Ndet=2;//=22;
  TString Dets[Ndet] = {
    // "Det_Q12EF_5_before_hits",
    // "Det_Q12EF_5_after_hits",
    // "Det_D21EF_5_before_hits",
    // "Det_D21EF_5_after_hits",
    // "Det_Q11EF_5_before_hits",
    // "Det_Q11EF_5_after_hits",
    // "Det_D22EF_5_before_hits",
    // "Det_D22EF_5_after_hits",
     "Det_Q10EF_5_before_hits",
    // "Det_Q10EF_5_after_hits",
    // "Det_Q9EF_5_before_hits",
    //     "Det_Q9EF_5_after_hits",
     "Det_Q8EF_5_before_hits"
    // "Det_Q8EF_5_after_hits",
    // "Det_Q7EF_5_before_hits",
    // "Det_Q7EF_5_after_hits",
    // "Det_Q6EF_5_before_hits",
    // "Det_Q6EF_5_after_hits",
    // "Det_Q5EF_5_before_hits",
    // "Det_Q5EF_5_after_hits",
    // "Det_Q4EF_5_before_hits",
    // "Det_Q4EF_5_after_hits"
};

  double cx,cy,cE,N;
  TH1D *h;
  TCanvas *c1 = new TCanvas("c1","c1", 0, 0, 2000,1000);
  TCanvas *c2 = new TCanvas("c2","c2", 100, 100, 2000,1000);
  c1->Divide(3,1);
  c2->Divide(3,1);


  //   for(int jj=0;jj<Np;jj++){
  //just do photons
  int jj=1;
   for(int ii=0;ii<Ndet;ii++){
     if(ii==0){
       //      cout<<"ii is 0"<<endl;
     c1->cd(1); // 00, 01,10,11 1, 3, 5, 7
     }
     if(ii==1){
     c2->cd(1); // 00, 01,10,11 1, 3, 5, 7
     }
     cout<<pID[jj]<<" "<<Dets[ii].Data()<<" ";

     t->Draw(Form("%s.y:%s.x",Dets[ii].Data(),Dets[ii].Data()),Form("%s.pID==%d",Dets[ii].Data(),pID[jj]));
  //  h=(TH1D*)gDirectory->FindObject(Form("hx%d_%d",ii,jj));
  //      cx=h->GetMean();
  //  cout<<cx<<" ";

     if(ii==0){
       //     c1->cd(2+(ii+jj*2)*3);
     c1->cd(2);
     }
     if(ii==1){
     c2->cd(2);
     }

     t->Draw(Form("%s.e:%s.x",Dets[ii].Data(),Dets[ii].Data()),Form("%s.pID==%d",Dets[ii].Data(),pID[jj]));
    // h=(TH1D*)gDirectory->FindObject(Form("hy%d_%d",ii,jj));
    //cy=h->GetMean();
    //cout<<cy<<" ";
    //    N=h->GetEntries();
    //     cout<<N<<endl;

     if(ii==0){
       c1->cd(3);
     }
     if(ii==1){
       c2->cd(3);
     }
     t->Draw(Form("%s.e>>hE%d_%d",Dets[ii].Data(),ii,jj),Form("(%s.pID==%d)*%s.e",Dets[ii].Data(),pID[jj],Dets[ii].Data()),"prof");
        h=(TH1D*)gDirectory->FindObject(Form("hE%d_%d",ii,jj));
        cE=h->GetMean();
        cout<<cE<<" ";
        N=h->GetEntries();
        cout<<N<<endl;

	if(ii==0){
	  //     c1->cd(1+(jj)*3)->SetLogy(1);
	  //     c1->cd(2+(jj)*3)->SetLogy(1);
     c1->cd(3)->SetLogy(1);
	}
	if(ii==1){
	  //     c2->cd(1+(jj)*3)->SetLogy(1);
	  //     c2->cd(2+(jj)*3)->SetLogy(1);
     c2->cd(3)->SetLogy(1);
	}

   }

   // }



}
