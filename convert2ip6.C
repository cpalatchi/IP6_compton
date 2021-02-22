#include <fstream>
#include <iostream>

using namespace std;

void convert2ip6(string finNm, string foutNm, long maxNevents){
  
  TVector3 quadDir(0,0,1);
  quadDir.RotateY(-0.0103371112);

  ofstream outf;
  outf.open(foutNm);

  ifstream infile;
  infile.open(finNm);

  double gPx,gPy,gPz,gE;
  double ePx,ePy,ePz,eE;
  double uXsec,pXsec,uXsecAlphaCorr,pXsecAlphaCorr;

  string line;
  for(int i=0;i<3;i++){
    getline(infile,line);
    cout<<line<<endl;
  }

  int i=0;
  double prevd1(0);
  while (!infile.eof() && (i<maxNevents || maxNevents<0)){
    getline(infile,line);
    stringstream ss(line);
    ss>>gPx>>gPy>>gPz>>gE
      >>uXsec>>pXsec>>uXsecAlphaCorr>>pXsecAlphaCorr
      >>ePx>>ePy>>ePz>>eE;

    if(i>1 && prevd1 == gPx) {
      cout<<"skipping event "<<i<<" "<<gPx<<endl;
      cout<<line<<endl;
      continue;
    }
    i++;

    if(i%100000==1)
      cout<<"\tprocessing event "<<i<<endl;

    //TVector3 quadPos(18.4809,0,-9286.90025);//actual quadPos
    string evInfo=Form("EVENT:  %d 2 18.5466  -0.000107986  -9280.5 %f %f",i,uXsec,pXsec);
    string gInfo = Form("TRACK:  22 %f %f %f",gPx,gPy,gPz);
    string eInfo = Form("TRACK:  11 %f %f %f",ePx,ePy,ePz);
    outf<<evInfo<<"\n"<<gInfo<<"\n"<<eInfo<<endl;
    prevd1 = gPx;
  }
  
  cout<<"processed "<<i<<" events\nQuitting!"<<endl;
}
