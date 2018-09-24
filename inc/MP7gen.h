#ifndef PM7GEN_HH
#define PM7GEN_HH

#include <cassert>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <utility>

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TEllipse.h"
#include "TList.h"
#include "TDirectory.h"

class mp7Word{

public:

    mp7Word(){ 
        data  = 0; 
        valid = false; 
    }

    uint32_t data;
    bool valid;

    string getWord(){
        stringstream out;    
        if(valid)
            out << 1 << "v";
        else
            out << 0 << "v";
        out << std::hex << std::setfill('0') << std::setw(8) << data << dec;
        return out.str();
    }
    
};


unsigned layer_localLayer[] = {

   100, //0
     0, //1
   100, //2
     0, //3
   100, //4
     0, //5
   100, //6
     0, //7
   100, //8
     0, //9
   100, //10
     0, //11
   100, //12
     0, //13
   100, //14
     0, //15
   100, //16
     0, //17
   100, //18
     1, //19
   100, //20
     1, //21
   100, //22
     1, //23
   100, //24
     1, //25
   100, //26
     1, //27
   100, //28
     1, //29
     1, //30
     1, //31
     1, //32
     2, //33
     2, //34
     2, //35
     2, //36
     2, //37
     2, //38
     2, //39
     2, //40
     2, //41
     3, //42
     3, //43
     3, //44
     4, //45
     3, //46
     4, //47
     3, //48
     4, //49
     2, //50
     3, //51
     4  //52

};



class hgcWord : public mp7Word { 

public: 

    hgcWord(bool valid=false) { this->valid =valid; }

    hgcWord(double energy, double maxEnergy, uint32_t nEnergyBits,
            uint32_t layer,                  uint32_t nLayerBits ,
            double phi, double maxPhi,       uint32_t nPhiBits   ,
            double rz , double maxRz ,       uint32_t nRzBits ){
        
        //mp7Word::mp7Word();

        _energyMask = 0;
        _layerMask  = 0;
        _phiMask    = 0;
        _rzMask     = 0;

        for(unsigned i=0; i<32; i++) {
            if( i < nEnergyBits )  _energyMask = _energyMask | (0x1<<i);
            if( i < nLayerBits  )  _layerMask  = _layerMask  | (0x1<<i); 
            if( i < nPhiBits    )  _phiMask    = _phiMask    | (0x1<<i);
            if( i < nRzBits     )  _rzMask     = _rzMask     | (0x1<<i);
        }  
        
        _intEnergy = energy / ( maxEnergy / pow(2, nEnergyBits) );
        _intLayer  = layer_localLayer[ layer ];
        _intPhi    = phi    / ( maxPhi    / pow(2, nPhiBits)    );
        _intRz     = rz     / ( maxRz     / pow(2, nRzBits )    );

        this->valid = true;
        this->data = _intRz & _rzMask; 
        this->data = this->data | (_intPhi    & _phiMask   ) << (nRzBits                        ); 
        this->data = this->data | (_intLayer  & _layerMask ) << (nRzBits + nPhiBits             ); 
        this->data = this->data | (_intEnergy & _energyMask) << (nRzBits + nPhiBits + nLayerBits);
        
        cout << "lay  " << std::hex << layer << " " << std::dec
             << endl
             << "iley " << std::hex << _intLayer  << " " << std::dec
             << endl;
//             << "leyM " << std::hex << layerMask  << " " << std::dec
//             << "phi " << std::hex << intPhi    << " " << std::dec 
//             << "r/z " << std::hex << intRz     << " " << std::dec 
//             << "r/zm " << std::hex << rzMask << " " << (intRz & rzMask) << " " << std::dec 
//             << "data " << std::hex << mp7.data << " " << std::dec 
//             << endl;

    }
    
    uint32_t rz() { 
        return ( _intRz & _rzMask ); 
    }

    uint32_t phi() {
        return (_intPhi & _phiMask ); 
    }

    uint32_t layer() {
        return ( _intLayer& _layerMask ); 
    }

    uint32_t energy() {
        return ( _intEnergy & _energyMask ); 
    }

    bool operator< (hgcWord& o)
    {
        return this->rz() < o.rz();
    }

private:
    
    uint32_t _intEnergy;
    uint32_t _intLayer ;
    uint32_t _intPhi   ;
    uint32_t _intRz    ;

    uint32_t  _energyMask ;
    uint32_t  _layerMask  ;
    uint32_t  _phiMask    ;
    uint32_t  _rzMask     ;

};


template<class T> 
class MP7gen {

public:

    static const unsigned nLinks = 72;

    MP7gen( std::string fileOutName, vector<T> *allLinks ) {
        
        _fileOutName = fileOutName;
        ofstream outFile(_fileOutName);
        
        unsigned nFrames=0;
        vector<hgcWord> allLinksDigital[this->nLinks];
        for(unsigned ilink=0; ilink<this->nLinks; ilink++) {
            for(unsigned iframe=0; iframe<allLinks[ilink].size(); iframe++) {
             
                int isector = int( (allLinks[ilink].at(iframe).Phi() + M_PI) / 0.5235987755982988 );
                double phi = allLinks[ilink].at(iframe).Phi() + M_PI - isector*0.5235987755982988;
                cout << "link" << ilink << endl;
                hgcWord data =  hgcWord( allLinks[ilink].at(iframe).Energy(), 100, 8 ,
                                         allLinks[ilink].at(iframe).layer() ,   3 ,
                                         phi   , 0.5235987755982988, 10,
                                         allLinks[ilink].at(iframe).r()/allLinks[ilink].at(iframe).z(), 0.7, 11 
                    );
                if( data.energy()>0 ) {
                    allLinksDigital[ilink].push_back( data );
                    nFrames = allLinksDigital[ilink].size()>nFrames ? allLinksDigital[ilink].size() : nFrames;
                }
                
            }
        }
        cout << "nFrames " << nFrames << endl;

        for(unsigned iframe=0; iframe<nFrames; iframe++){

            vector<hgcWord> dataFrame; 
            
            for(unsigned ilink=0; ilink<this->nLinks; ilink++) { 

                allLinksDigital[ilink].size();

                if( allLinksDigital[ilink].size() <= iframe )
                    dataFrame.push_back( hgcWord(true) ); 
                
                else
                        dataFrame.push_back( allLinksDigital[ilink].at(iframe) );

            }
            
            _dataFrames.push_back( dataFrame );
            //cout << "len " << _dataFrames.size() << endl;
            
        }
        


//        if ( nFrames>0 ) {
//            cout << "************" << endl; 
//            //cout << "len " << _dataFrames.size() << endl;
//            vector<hgcWord> arr = _dataFrames.at( 1 );
//            for(unsigned ilink=0; ilink<this->nLinks; ilink++) { 
//                cout << "-" << std::hex << arr.at(ilink).data << std::dec << endl ;
//            }
//            cout << "************" << endl; 
            
        
        
        outFile << this->headerLine(0) << endl;
        
        for(unsigned iframe=0; iframe<_dataFrames.size(); iframe++){
            outFile << dataLine(iframe, _dataFrames.at(iframe) );
        }
        
    }
    
//    void dumpIntoMP7datafile event(vector<const T*> allHits) {
//      
//        std::cout << " >>> MP7gen : writing " << _fileOutName << " to disk." << std::endl;
//        
//        this->headerLine(l);
//        
//        for(unsigned w(0); w<Geometry::numberOfWafers(l); w++) {
//            
//            for( unsigned c(0); c<Geometry::numberOfTriggerCells(l,w); c++ ) {
//                if ( !Geometry::isTriggerCellPresent(e, l, w, c) ) continue;
//                
//                if ( event.trgHits( e, l, w, c ).transverseMips() > 1 ){
//                    hOccupancyTrgHitEne.Fill(e, l, 
//                                             Geometry::getTCcenter(e, l, w, c).first, 
//                                             Geometry::getTCcenter(e, l, w, c).second, 
//                                             event.trgHits( e, l, w, c ).simMips() );
//                    hgcWord hgcW;
//                    hgcW.energy = event.trgHits( e, l, w, c ).transverseMips();
//                    hgcW.addr.x = Geometry::getWafer(e, l, w)->getNormId().first;
//                    hgcW.addr.y = Geometry::getWafer(e, l, w)->getNormId().second;
//                    hgcW.valid  = 1;
//                    
//                    mp7Word mp7W = hgc2mp7(hgcW);
//                    std::cout << this->dataLine(0, mp7W);
//                }
//            }
//            
//        /* write single event file */
//        writeEventOnDisk=true;
//        if(writeEventOnDisk)
//            this->write( fEventNumber );
//        
///***************************/
//
//        return true;
//    }
  

    void write(unsigned evt){
        
        stringstream ssEvt;
        ssEvt << evt;
        string sEvt = ssEvt.str();
        
        std::string fileName("mp7Gen");
        std::cout << "Out file: " << (fileName+"_"+sEvt.c_str()+".root").c_str() << endl;
        TFile fs( (fileName+"_"+sEvt.c_str()+".root").c_str(), "RECREATE" );
    
//        hOccupancyTrgHitEne.Write(&fs);
        
        fs.Close();
    
    }

    
    string dataLine(int frameId, vector<hgcWord> w){

        stringstream line;
        
        line << std::right << "Frame " << setfill('0') << setw(4) << frameId << " : ";
        for(unsigned ilink=0; ilink<w.size(); ilink++)
            line << " " << w.at(ilink).getWord();
        line << std::endl;
        
        return line.str();
        
    }
    
    
    string headerLine(int boardId){
        
        stringstream headerBlock;
        
        headerBlock << "Board MP" << boardId <<  std::endl;
        
        headerBlock << " Quad/Chan : ";
        for(int iquad=0; iquad<24; iquad++)
            for(int ichan=1; ichan<=3; ichan++)
                headerBlock << "   q" <<  setfill('0') << setw(2) << iquad << "c" << setw(1) << ichan << "   "; 
        headerBlock << std::endl;
        
        headerBlock << "      Link : ";
        for(unsigned ilink=0; ilink<this->nLinks; ilink++)
            headerBlock << std::right << "    " << setw(2) << ilink << "     "; 
        //headerBlock <<  std::endl;
        
        return headerBlock.str();
        
    }
    
    
private:
  
    std::string _fileOutName;
    
    vector< vector<hgcWord> > _dataFrames;



};

#endif
