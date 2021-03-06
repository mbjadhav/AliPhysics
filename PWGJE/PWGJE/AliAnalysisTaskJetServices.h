#ifndef ALIANALYSISTASKJETSERVICES_H
#define ALIANALYSISTASKJETSERVICES_H
 
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// **************************************
// Task used for the correction of determiantion of reconstructed jet spectra
// Compares input (gen) and output (rec) jets   
// *******************************************

#include  "AliAnalysisTaskSE.h"
#include  "THnSparse.h" // cannot forward declare ThnSparseF

////////////////
class AliJetHeader;
class AliESDEvent;
class AliESDVertex;
class AliAODEvent;
class AliAODVertex;
class AliAODVZERO;
class AliAODJet;
class AliGenPythiaEventHeader;
class AliCFManager;
class AliTriggerAnalysis;

class TList;
class TClonesArray;
class TObjArray;
class TChain;
class TH1F;
class TH2F;
class TH3F;
class TProfile;
class TRandom3;



class AliAnalysisTaskJetServices : public AliAnalysisTaskSE
{
 public:
    AliAnalysisTaskJetServices();
    AliAnalysisTaskJetServices(const char* name);
    virtual ~AliAnalysisTaskJetServices();
    // Implementation of interface methods
    virtual void UserCreateOutputObjects();
    virtual void Init();
    virtual void LocalInit() { Init(); }
    virtual void UserExec(Option_t *option);
    virtual void Terminate(Option_t *option);
    virtual void SetZVertexCut(Float_t f){fVtxZCut = f;}
    virtual void SetDeltaZVertexCut(Float_t f){fVtxDeltaZCut = f;}
    virtual void SetPtMinCosmic(Float_t ptMin) {fPtMinCosmic = ptMin;}
    virtual void SetRMinCosmic(Float_t rMin) {fRIsolMinCosmic = rMin;}
    virtual void SetMaxCosmicAngle(Float_t angle)   {fMaxCosmicAngle = angle;}
    virtual Bool_t Notify();

    virtual void SetAODInput(Bool_t b){fUseAODInput = b;}
    virtual void SetRunRange(Float_t fLo,Float_t fUp){fRunRange[0] = fLo;fRunRange[1] = fUp;}
    virtual void SetMCData(Bool_t b){fMC = b;}
    virtual void SetCollisionType(Int_t iType){fCollisionType = iType;}
    virtual void SetUsePhysicsSelection(Bool_t b){fUsePhysicsSelection = b;}
    virtual void SetPhysicsSelectionFlag(Int_t i){fPhysicsSelectionFlag = i;}
    virtual void SetFilterAODCollisions(Bool_t b){fFilterAODCollisions = b;}

    virtual void SetNonStdFile(char *c){fNonStdFile = c;}
    Bool_t IsEventSelected(const AliESDEvent* esd);
    Bool_t IsEventSelected(const AliAODEvent* aod) const;

    Bool_t IsEventPileUp(const AliESDEvent* esd) const;

    Bool_t IsEventCosmic(const AliESDEvent* esd) const;

    Bool_t IsVertexValid(const AliESDVertex *vtx);
    Bool_t IsVertexValid(const AliAODVertex *vtx) const;

    Bool_t IsVertexIn(const AliESDVertex *vtx,const AliESDVertex *vtxSPD);
    Bool_t IsVertexIn(const AliAODVertex *vtx,const AliAODVertex *vtxSPD) const;
    Int_t GetEventClass(AliESDEvent *esd);
    Int_t GetEventClass(AliAODEvent *aod);


    virtual void SetFilterMask(UInt_t i){fFilterMask = i;}
    virtual void SetMinTrackPt(Float_t f){fMinTrackPt = f;}
    virtual void SetTrackEtaWindow(Float_t f){fTrackRecEtaWindow = f;}
    virtual void SetRPMethod(Int_t i){fRPMethod = i;}
    virtual void SetV0Centroids(TProfile *xa,TProfile *ya,
			       TProfile *xc,TProfile *yc);

    virtual void SetNTrigger(Int_t n);
    virtual void SetTrigger(Int_t i,UInt_t it,const char* c = "");


    Bool_t   CalculateReactionPlaneAngleVZERO(AliAODEvent *aod);
    Int_t   GetListOfTracks(TList *list);


    enum { kAllTriggered = 0,
	   kTriggeredVertex,          //1
	   kTriggeredVertexIn,        //2
	   kSelectedALICE,            //3
	   kSelectedALICEVertexValid, //4
	   kSelectedALICEVertexIn,    //5
	   kSelected,                 //6
	   kConstraints};             //7
    enum { kRPTracks = 0, kRPVZEROA,kRPVZEROC,kRPMethods};

    enum { kNoEventCut=1<<0,
	   kPhysicsSelectionCut=1<<1,
	   kContributorsCut1=1<<2,
	   kContributorsCut2=1<<3,
	   kContributorsCut3=1<<4,
	   kVertexTPC=1<<5,
	   kVertexSPD=1<<6,
	   kVertexGlobal=1<<7,
	   kSPDDispersionCut=1<<8,
	   kVertexZCut=1<<9,
	   kVertexRCut=1<<10,
	   kVertexDeltaZCut=1<<11,
	   kTotalEventCuts=(1<<12)-1};
    enum {kPbPb = 0,kPP,kPbP};

 private:

    AliAnalysisTaskJetServices(const AliAnalysisTaskJetServices&);
    AliAnalysisTaskJetServices& operator=(const AliAnalysisTaskJetServices&);

    Bool_t        fUseAODInput;        // take jet from input AOD not from ouptu AOD
    Bool_t        fUsePhysicsSelection;// decide wether we take into account physicsselction task
    Bool_t        fMC;                 // true for MC data to allow correct trigger slection
    Bool_t        fFilterAODCollisions; // filter out collision canditates to the AOD
    UInt_t        fPhysicsSelectionFlag; // defines the glag for acceptance of events from physics selection
    UInt_t        fSelectionInfoESD;   // slection info bit mask
    UInt_t        fEventCutInfoESD;   // event selection info of what is cutted after physics selection
    UInt_t        fFilterMask;         // filter bit for slecected tracks
    Int_t         fRPMethod;           // method for subevent calculation
    Int_t         fCollisionType;      // type of collisions
    Int_t         fNTrigger;           // Number of different triggers
    Float_t       fAvgTrials;          // Average number of trials
    Float_t       fVtxXMean;           // mean x for cuts
    Float_t       fVtxYMean;           // mean y for cuts
    Float_t       fVtxZMean;           // mean z for cuts
    Float_t       fVtxRCut;            // vtx cut in R
    Float_t       fVtxZCut;            // vtzx cut in Z
    Float_t       fVtxDeltaZCut;       // cut on difference between SPD and Global vtx
    Float_t       fPtMinCosmic;        // Minimum pT to be considered as cosmic candidate
    Float_t       fRIsolMinCosmic;     // Minimum R = sqrt{deltaPhi^2 + deltaEta^2} to be considered as cosmic candidate
    Float_t       fMaxCosmicAngle;     // Max deviation from pi (angle between two tracks) in case of cosmic candidate
    Float_t       fRunRange[2];        // only important for real data for 
    Float_t       fCentrality;         // ! centrality
    Float_t       fTrackRecEtaWindow;  // eta window for rec tracks
    Float_t       fMinTrackPt;         // limits the track p_T 
    Float_t       fRPAngle;            // ! RP angle of the reaction plane
    Float_t       fPsiVZEROA;          // ! RP angle from vzeroa
    Float_t       fPsiVZEROC;          // ! RP angle from vzeroc
    UInt_t        *fTriggerBit  ;      //[fNTrigger]

    TRandom3      *fRandomizer;        // ! randomizer

    TString       fNonStdFile;         // outputName for replication
    TString       *fTriggerName;  //[fNTrigger] array of trigger names
    TProfile*     fh1Xsec;             //! pythia cross section is averaged
    TH1F*         fh1Trials;           //! trials are added per notify...
    TH1F*         fh1AvgTrials;        //! average trials are added per event
    TH1F*         fh1PtHard;           //! Pt har of the event...       
    TH1F*         fh1PtHardTrials;     //! Number of trials 
    TH1F*         fh1SelectionInfoESD; //! Masks that satisfy fSelectionInfo
    TH1F*         fh1EventCutInfoESD;  //! Masks that satisfy fSelectionInfo
    TH1F*         fh1CentralityESD;    //! centrality 
    TH1F*         fh1Centrality;       //! centrality 
    TH1F*         fh1RP;               //! RP distribution
    TH2F*         fh2ReducedTrigger;   //! reduced trigger count 
    TH2F*         fh2CentralityTriggerESD;    //! centrality 
    TH2F*         fh2CentralityTrigger;       //! centrality 
    TH2F*         fh2TriggerCount;     //! number of fire triggers in each case
    TH2F*         fh2ESDTriggerCount;  //! number of fire triggers in each case
    TH2F*         fh2TriggerVtx;       //! vtx. position vs. trigger decision
    TH2F*         fh2ESDTriggerVtx;    //! vtx. position vs. trigger decision 
    TH2F*         fh2ESDTriggerRun;    //! fired triggers vs. run number
    TH2F*         fh2VtxXY;            //! XY position of VTX were available
    TH1F*         fh1NCosmicsPerEvent; //! Number of coscmic candidates found in event
    TProfile*     fp1RPXA;              //! mean XA vs run
    TProfile*     fp1RPYA;              //! mean YA vs run
    TProfile*     fp1RPXC;              //! mean XA vs run
    TProfile*     fp1RPYC;              //! mean YA vs run
    TProfile*     fp1CalibRPXA;              // calib mean XA vs run
    TProfile*     fp1CalibRPYA;              // calib YA vs run
    TProfile*     fp1CalibRPXC;              // calib XA vs run
    TProfile*     fp1CalibRPYC;              // calib YA vs run
    TH2F*         fh2RPAC;              //! RP A vs C 
    TH2F*         fh2RPAT;              //! RP A vs tracks 
    TH2F*         fh2RPCT;              //! RP C vs tracks 
    TH2F*         fh2XYA;               //! XY correlations VZERO C
    TH2F*         fh2XYC;               //! XY correlations VZERO C
    TH2F*         fh2RPCentrality;      //! RP vs centrality
    TH2F*         fh2RPACentrality;     //! RP vs centrality
    TH2F*         fh2RPCCentrality;     //! RP vs centrality

    AliTriggerAnalysis *fTriggerAnalysis; //! Trigger Analysis to get the background rates etc.
    TList *fHistList; //! Output list

        // Provisions for replication
    static AliAODHeader*    fgAODHeader;        //! Header for replication
    static AliAODVZERO*    fgAODVZERO;        //! vzero for replication
    static TClonesArray*  fgAODVertices;        //! primary vertex for replication
    ClassDef(AliAnalysisTaskJetServices,17)
};
 
#endif
