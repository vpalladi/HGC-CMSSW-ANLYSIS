#ifndef DETID_H
#define DETID_H 1

/* C */
#include <stdint.h>


class HGCalDetId {

public:

    static const int kHGCalCellOffset      = 0;
    static const int kHGCalCellMask        = 0xFF;
    static const int kHGCalWaferOffset     = 8;
    static const int kHGCalWaferMask       = 0x3FF;
    static const int kHGCalWaferTypeOffset = 18;
    static const int kHGCalWaferTypeMask   = 0x1;
    static const int kHGCalLayerOffset     = 19;
    static const int kHGCalLayerMask       = 0x1F;
    static const int kHGCalZsideOffset     = 24;
    static const int kHGCalZsideMask       = 0x1;
    static const int kHGCalMaskCell        = 0xFFFFFF00;

    /** Create a null cellid*/
    HGCalDetId();

    /** Create cellid from raw id (0=invalid tower id) */
    HGCalDetId(uint32_t rawid) { id_ = rawid; }

    /** Constructor from subdetector, zplus, layer, module, cell numbers */
//    HGCalDetId(ForwardSubdetector subdet, int zp, int lay, int wafertype, int wafer, int cell);
  
    /** Converter for a geometry cell id */
    HGCalDetId geometryCell () const {return id_&kHGCalMaskCell;}

    /// get the absolute value of the cell #'s in x and y
    int cell() const { return id_&kHGCalCellMask; }

    int thirdId() const { return (id_&kHGCalCellMask)>>4 ;}

    /// get the wafer #
    int wafer() const { return (id_>>kHGCalWaferOffset)&kHGCalWaferMask; }

    /// get the wafer type
    int waferType() const { return ((id_>>kHGCalWaferTypeOffset)&kHGCalWaferTypeMask ? 1 : -1); }

    /// get the layer #
    int layer() const { return (id_>>kHGCalLayerOffset)&kHGCalLayerMask; }

    /// get the z-side of the cell (1/-1)
    int zside() const { return ((id_>>kHGCalZsideOffset) & kHGCalZsideMask ? 1 : -1); }

    /// consistency check : no bits left => no overhead
    bool isHGCal()   const { return true; }
    bool isForward() const { return true; }
//    static bool isValid(ForwardSubdetector subdet, int zp, int lay, 
//                        int wafertype, int wafer, int cell);

//    static const HGCalDetId Undefined;

private: 

    uint32_t id_;


};


//std::ostream& operator<<(std::ostream&,const HGCalDetId& id);

#endif
