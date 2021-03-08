
//_____________________________________________________________________________
//
// main detector construction,
// detector definition is here
//_____________________________________________________________________________

//C++
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <fstream>

//Geant
#include "G4GenericMessenger.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

//local classes
#include "DetectorConstruction.h"
#include "RootOut.h"
#include "MCEvent.h"
#include "EventReader.h"
#include "BeamMagnetDipole.h"
#include "BeamMagnetQuadrupole.h"
#include "BeamMagnetQuadrupole_cone.h"
#include "electronDet.h"
//#include "planeDet.h"
#include "photonDet.h"
#include "calBox.h"
#include "genericDet.h"
#include "chamberBox.h"
#include "BeamPipe.h"

using namespace std;

//_____________________________________________________________________________
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), fDet(0), fOut(0), fMsg(0), 
	fIncMagnet(0), fIncB2eR(0),fIncQF1(0) {

		G4cout << "DetectorConstruction::DetectorConstruction" << G4endl;

		//output file and tree
		fOut = new RootOut();

		//all detectors and their parts
		fDet = new std::vector<Detector*>;

		//MC event, also inherits from Detector
		fMC = new MCEvent();
		AddDetector(fMC);


		//messenger for detectors and components
		fMsg = new G4GenericMessenger(this, "/EPol/construct/");
		fMsg->DeclareProperty("B2eR", fIncB2eR);
		fMsg->DeclareProperty("QF1", fIncQF1);

	}//DetectorConstruction

//_____________________________________________________________________________
DetectorConstruction::~DetectorConstruction() {

	//write the tree and close output file
	fOut->Close();

	delete fDet;

}//~DetectorConstruction

//_____________________________________________________________________________
G4VPhysicalVolume* DetectorConstruction::Construct() {

	G4cout << G4endl << "DetectorConstruction::Construct" << G4endl;

	//vacuum top material
	G4Material* top_m = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

	//top world volume
	G4Box *top_s = new G4Box("top_s", 20*meter, 20*meter, 200*meter);
	//G4Box *top_s = new G4Box("top_s", 3*m, 3*m, 3*m);
	G4LogicalVolume *top_l = new G4LogicalVolume(top_s, top_m, "top_l");
	top_l->SetVisAttributes( G4VisAttributes::GetInvisible() );
	G4VPhysicalVolume *top_p = new G4PVPlacement(0, G4ThreeVector(), top_l, "top_p", 0, false, 0);




	//detectors

	//beamline magnet
	//if(fIncB2eR) new BeamMagnet(-12.254*meter, top_l);
	ifstream magfile;
	magfile.open("../polarimeter_magnet.dat");

	string line;


	if(magfile.fail()) {
		G4cout << "eee ****************"<<endl;
		string description = "Can't open mag file";
		G4Exception("EventReader::OpenInput", "InputNotOpen01", FatalException, description.c_str());
	}

	string cc1,cc2;

	//Caryn add for names of detectors
	string detname;

	G4double xpos,ypos,zpos,length,r1,r2,dout,angle,bfield,gradient;

	int number = 0;

	while(!magfile.eof()){
		getline(magfile,line);
		//if(number>12)continue;
		if(line=="")continue;
		number++;

		stringstream ss(line);
		ss>>cc1;
		ss>>cc2;
		ss>>xpos;
		ss>>ypos;
		ss>>zpos;
		ss>>r1;
		ss>>r2;
		ss>>dout;
		ss>>length;
		ss>>angle;
		ss>>bfield;
		ss>>gradient;
		G4cout << "test= ****************"<<endl;
		G4cout << "read mag file" <<xpos<<" "<<ypos<<" "<<zpos<<" "<<length<<" "<<r1<<" "<<r2<<" "<<dout<<" "<<angle<<" "<<bfield<<" "<<endl;
		if(number!=8&&number!=9&&number!=10&&number!=14){
		if(cc1=="DB")AddDetector(new BeamMagnetDipole(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,bfield*tesla, top_l));//add Dipole
		if(cc1=="QF"){
			G4cout << "read mag file" <<xpos<<" "<<ypos<<" "<<zpos<<" "<<length<<" "<<r1<<" "<<r2<<" "<<dout<<" "<<angle<<" "<<bfield<<" "<<endl;
			AddDetector(new BeamMagnetQuadrupole(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter, 1,0,0,top_l));} //add Quadrupole
		}
//8 9 10
		if(number==8){
			AddDetector(new BeamMagnetQuadrupole_cone(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter,0.09490926*meter,0.0125*meter,0.0125*meter,-14.029944,top_l));//add Quadrupole  0.014029944 shift 0.0042092594
		}
		if(number==9){
                        AddDetector(new BeamMagnetQuadrupole_cone(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter,0.1706421*meter,0.02*meter,0.02*meter,-14.029944,top_l));
                }
		if(number==10){
                        AddDetector(new BeamMagnetQuadrupole_cone(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter,0.2422421*meter,0.025*meter,0.025*meter,-14.029944,top_l));
                }
		if(number==14){
			AddDetector(new BeamMagnetQuadrupole(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter, 0.5,0.5,0.5,top_l));} //add Quadrupole
		//Caryn add detectors before and after dipoles
		if(cc1=="QF"){
		  //		  if(xpos<1&&zpos>-83.5){
		  if(xpos<1){
		   detname = "Det_"+cc2+"_after";
		   AddDetector(new genericDet(detname,xpos*meter,ypos*meter,zpos*meter+0.35*meter,0,2500,1,top_l)); //add Quadrupole
		   detname = "Det_"+cc2+"_before";
		   AddDetector(new genericDet(detname,xpos*meter,ypos*meter,zpos*meter-0.35*meter,0,2500,1,top_l)); //add Quadrupole

		  }
		}
		if(cc1=="DB"){
		  if(xpos<1){
		   detname = "Det_"+cc2+"_after";
		   AddDetector(new genericDet(detname,xpos*meter,ypos*meter,zpos*meter+1.5*meter,0,2500,1,top_l)); //add Quadrupole
		   detname = "Det_"+cc2+"_before";
		   AddDetector(new genericDet(detname,xpos*meter,ypos*meter,zpos*meter-1.5*meter,0,2500,1,top_l)); //add Quadrupole
		  }
		}

	}

//photon det
		AddDetector(new calBox("Gdet_ecal",515.59067,0,-60870.712,-0.0103371112 ,250*mm,400*mm,top_l)); //add Ecal detector 32m from laser IP primary
// to study the recoil electron
//		AddDetector(new electronDet("Edet_QD9",178.78630,0,-83845.821,0.0036928331,top_l));  //recoil electron detector

		//       AddDetector(new planeDet("planeDet",178.78630,0,-84845.821,0.0036928331,top_l));  //recoil electron detector
		//      AddDetector(new genericDet("gDetTst",515.59067,0,-60870-410.712,-0.0103371112 ,250,1, top_l));  //generic detector


	
		//adding chamber
		G4double boxthickness;
		boxthickness=1*(1/8.)*25.4*meter/1000;//5/8" in mm
		AddDetector(new chamberBox("chamber",278,0,-83845.8,0.0036928331 ,640*mm,1*meter,boxthickness, top_l)); //add Ecal detector 32m from laser IP primary
		//		AddDetector(new calBox("Gdet_ecal",515.59067,0,-60870.712,-0.0103371112 ,250*mm,400*mm,top_l)); //add Ecal detector 32m from laser IP primary
		//		AddDetector(new BeamMagnetDipole("edetchamber",278,-0.4,-83845.8,1*meter,0.05*meter,0.05*meter,1*meter,0,0*tesla, top_l));//add Dipole

		//adding beampipe
		G4double wallthickness;
		G4double x1,x2,z1,z2;
		G4double cx,cy,cz,l,r1pipe,r2pipe,theta;
		wallthickness=5*(1/8.)*25.4*meter/1000;//5/8" in mm
		x1=0.1998355*meter;
		x2=0.181002*meter;
		z1=-88.34579*meter;
		z2=-83.245825*meter;
		//outerD=2*0.06*meter;
		cx=(x1+x2)/2;
		cy=0;
		cz=(z1+z2)/2;
		l=(z2-z1);
		theta=-(x2-x1)/(z2-z1);
		r1pipe=0.06*meter-wallthickness;
		r2pipe=r1pipe;
		//		AddDetector(new BeamMagnetDipole("beampipe",cx,cy,cz,l,r1pipe,r2pipe,outerD,theta,0*tesla, top_l));//add Dipole
		wallthickness=(1/8.)*25.4*meter/1000;//changed to 5/8" from 1/8" in mm
		x1=0.1998355*meter;
		x2=0.181002*meter;
		z1=-88.34579*meter;
		z2=-83.8458*meter-1*meter;
		//outerD=2*3*0.06*meter;
		cx=(x1+x2)/2;
		cy=0;
		cz=(z1+z2)/2;
		l=(z2-z1);
		theta=-(x2-x1)/(z2-z1);
		r1pipe=3*0.06*meter-wallthickness;
		r2pipe=3*0.06*meter;
		//		r2pipe=r1pipe;
		AddDetector(new BeamPipe("beampipe",cx,cy,cz,l,r1pipe,r2pipe,theta, top_l));//add Dipole
		//add beam pipe inside 2nd dipole and quads 11,10
		wallthickness=(1/8.)*25.4*meter/1000;//changed to 5/8" from 1/8" in mm
		x1=0.197934*meter;//q11
		x2=0.1998355*meter;//q10
		z1=-92.8690025*meter;//q11
		z2=-88.34579*meter;//q10
		//outerD=2*0.06*meter;
		cx=(x1+x2)/2;
		cy=0;
		cz=(z1+z2)/2;
		l=(z2-z1);
		theta=-(x2-x1)/(z2-z1);
		r1pipe=0.06*meter-wallthickness;
		//		r2pipe=r1pipe;
		r2pipe=0.06*meter;
		AddDetector(new BeamPipe("beampipe0",cx,cy,cz,l,r1pipe,r2pipe,theta, top_l));//add pipe




	// if(fIncB2eR) AddDetector(new BeamMagnetDipole("B2eR",0.238873*meter,0.0*meter, -6.6488*meter,4.22367*meter,0.1*meter,0.1*meter,0.5*meter,-12.699,-0.272916*tesla, top_l));
	//  if(fIncQF1) AddDetector(new BeamMagnetQuadrupole("QF1", -9.5956*meter, top_l));
	// if(fIncQF1) AddDetector(new BeamMagnetQuadrupole("QF2", -19.5956*meter, top_l));


	return top_p;

}//Construct

//_____________________________________________________________________________
void DetectorConstruction::BeginEvent(const G4Event *evt) const {

	//detector loop for  ClearEvent  in each detector
	std::for_each(fDet->begin(), fDet->end(), std::mem_fun( &Detector::ClearEvent ));

	//set MC
	fMC->BeginEvent(evt);

}//BeginEvent

//_____________________________________________________________________________
void DetectorConstruction::FinishEvent() const {

	//detector loop
	std::for_each(fDet->begin(), fDet->end(), std::mem_fun( &Detector::FinishEvent ));

	//fill the output tree
	fOut->FillTree();

}//WriteEvent

//_____________________________________________________________________________
void DetectorConstruction::AddDetector(Detector *det) {

	//add detector to all detectors
	det->Add(fDet);

}//AddDetector

//_____________________________________________________________________________
void DetectorConstruction::CreateOutput() const {

	//open output file
	fOut->Open();

	//detector loop to call CreateOutput
	std::vector<Detector*>::iterator i = fDet->begin();
	while(i != fDet->end()) {
		(*i++)->CreateOutput( fOut->GetTree() );
	}//detector loop

}//CreateOutput

//_____________________________________________________________________________
void DetectorConstruction::ConstructSDandField() {

	G4cout << "DetectorConstruction::ConstructSDandField" << G4endl;

	//detector loop
	std::vector<Detector*>::iterator i;
	for(i = fDet->begin(); i != fDet->end(); ++i) {
		Detector *det = *i;

		G4VSensitiveDetector *sd = dynamic_cast<G4VSensitiveDetector*>(det);
		if(!sd) continue;

		//detector inherits also from G4VSensitiveDetector, add it to Geant

		G4SDManager::GetSDMpointer()->AddNewDetector(sd);
		SetSensitiveDetector(det->GetName(), sd);

		G4cout << "  " << det->GetName() << G4endl;
	}//detector loop

}//ConstructSDandField



















