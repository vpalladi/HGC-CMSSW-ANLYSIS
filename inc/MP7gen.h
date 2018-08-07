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


class hgcWord : public mp7Word { 

public: 

    hgcWord() { }
    hgcWord(double energy, double maxEnergy, uint32_t nEnergyBits,
            uint32_t layer,                  uint32_t nLayerBits ,
            double phi, double maxPhi,       uint32_t nPhiBits   ,
            double rz , double maxRz ,       uint32_t nRzBits ){
        
        //mp7Word::mp7Word();
        
        for(unsigned i=0; i<32; i++) {
            if( i < nEnergyBits )  _energyMask = _energyMask | (0x1<<i);
            if( i < nLayerBits  )  _layerMask  = _layerMask  | (0x1<<i); 
            if( i < nPhiBits    )  _phiMask    = _phiMask    | (0x1<<i);
            if( i < nRzBits     )  _rzMask     = _rzMask     | (0x1<<i);
        }  

        uint32_t intEnergy = energy / ( maxEnergy / pow(2, nEnergyBits) ); 
        uint32_t intLayer  = layer;
        uint32_t intPhi    = phi    / ( maxPhi    / pow(2, nPhiBits)    ); 
        uint32_t intRz     = rz     / ( maxRz     / pow(2, nRzBits )    ); 
        
        this->valid = true;
        this->data = intRz & _rzMask; 
        this->data = this->data | (intPhi    & _phiMask   ) << (nRzBits                        ); 
        this->data = this->data | (intLayer  & _layerMask ) << (nRzBits + nPhiBits             ); 
        this->data = this->data | (intEnergy & _energyMask) << (nRzBits + nPhiBits + nLayerBits);
        
//        cout << "ene " << std::hex << intEnergy << " " << std::dec 
//             << "ley " << std::hex << intLayer  << " " << std::dec
//             << "leyM " << std::hex << layerMask  << " " << std::dec
//             << "phi " << std::hex << intPhi    << " " << std::dec 
//             << "r/z " << std::hex << intRz     << " " << std::dec 
//             << "r/zm " << std::hex << rzMask << " " << (intRz & rzMask) << " " << std::dec 
//             << "data " << std::hex << mp7.data << " " << std::dec 
//             << endl;

    }
    
    uint32_t rz() { 
        return (data & _rzMask ); 
    }

    bool operator< (hgcWord& o)
    {
        return this->rz() < o.rz();
    }

private:
    
    uint32_t  _energyMask = 0 ;
    uint32_t  _layerMask  = 0 ;
    uint32_t  _phiMask    = 0 ;
    uint32_t  _rzMask     = 0 ;

};


template<class T> 
class MP7gen {

public:

    static const unsigned nLinks = 72;

    MP7gen( std::string fileOutName, vector<T> *allLinks ) {
        
        _fileOutName = fileOutName;
        
        unsigned nFrames=0;
        for(unsigned i=0; i<this->nLinks; i++)
            nFrames = allLinks[i].size()>nFrames ? allLinks[i].size() : nFrames;

        cout << "nFrames " << nFrames << endl;
        
        for(unsigned iframe=0; iframe<nFrames; iframe++){

            vector<hgcWord> dataFrame; 
            
            for(unsigned ilink=0; ilink<this->nLinks; ilink++) { 

                allLinks[ilink].size();

                if( allLinks[ilink].size() <= iframe ){
                    dataFrame.push_back( hgcWord () ); 
                }
                else {

                    int isector = int( (allLinks[ilink].at(iframe).Phi() + M_PI) / 0.5235987755982988 );
                    double phi = allLinks[ilink].at(iframe).Phi() + M_PI - isector*0.5235987755982988;
                    
                    dataFrame.push_back( hgcWord( allLinks[ilink].at(iframe).Energy(), 100, 8 ,
                                                  allLinks[ilink].at(iframe).layer() ,   3 ,
                                                  phi   , 0.5235987755982988, 10,
                                                  allLinks[ilink].at(iframe).r()/allLinks[ilink].at(iframe).z(), 0.7, 11 
                                             )
                        );
                }

                cout << ilink << " " << std::hex << dataFrame.back().data << std::dec << endl;

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
            

        cout << this->headerLine(0) << endl;
        
        for(unsigned iframe=0; iframe<_dataFrames.size(); iframe++){
            cout << dataLine(iframe, _dataFrames.at(iframe) );
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
        headerBlock <<  std::endl;
        
        return headerBlock.str();
        
    }
    
    
private:
  
    std::string _fileOutName;
    
    vector< vector<hgcWord> > _dataFrames;



};

#endif
