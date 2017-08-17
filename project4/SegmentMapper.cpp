#include "provided.h"
#include <vector>
#include "MyMap.h"
#include "support.h"
using namespace std;

//bool operator==(const GeoCoord& check1, const GeoCoord& check2)  {
//    if (check1.latitude == check2.latitude) {
//        if (check1.longitude == check2.longitude){
//            return true;
//        }
//    }
//    return false;
//}
//
//bool operator>(const GeoCoord& check1, const GeoCoord& check2)  {
//    if (check1.latitude > check2.latitude) {
//        return true;
//    }
//    else if (check1.latitude == check2.latitude){
//        if (check1.longitude > check2.longitude)
//            return true;
//    }
//    return false;
//}
//
//
//bool operator<(const GeoCoord& check1, const GeoCoord& check2) {
//    if (check1.latitude < check2.latitude) {
//        return true;
//    }
//    else if (check1.latitude == check2.latitude){
//        if (check1.longitude < check2.longitude)
//            return true;
//    }
//    return false;
//}
//
//bool operator==(const StreetSegment& check1, const StreetSegment& check2) {
//    if (check1.streetName == check2.streetName) {
//        if (check1.segment.start == check2.segment.start && check1.segment.end == check2.segment.end){
//            return true;
//        }
//    }
//    return false;
//}
//
//bool operator>(const StreetSegment& check1, const StreetSegment& check2) {
//    if (check1.streetName > check2.streetName) {
//        return true;
//    }
//    else if (check1.streetName == check2.streetName){
//        if (check1.segment.start == check2.segment.start && check1.segment.end == check2.segment.end)
//            return false;
//    }
//    return true;
//}
//
//
//bool operator<(const StreetSegment& check1, const StreetSegment& check2) {
//    if (check1.streetName < check2.streetName) {
//        return true;
//    }
//    else if (check1.streetName == check2.streetName){
//        if (check1.segment.start == check2.segment.start && check1.segment.end == check2.segment.end)
//            return false;
//    }
//    return true;}



class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord, vector<StreetSegment> > segmentMap;
};


SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for (int i = 0; i < ml.getNumSegments(); i++) {
        StreetSegment seg;
        if (ml.getSegment(i, seg)) {
            
                    //I found that running the attractions first is better beause otherwise you might not
                                                 //add an attraction that you need to
            for (int i = 0; i < seg.attractions.size(); i++) {
                vector<StreetSegment>* strSegsAttr = segmentMap.find(seg.attractions[i].geocoordinates);
                if (strSegsAttr != nullptr) {
                    (*strSegsAttr).push_back(seg);      //add the seg to an existing vector of segments
                }
                else {                                          //avoid repeats because maps do not double associate
                    vector<StreetSegment> newAssocAttr;
                    newAssocAttr.push_back(seg);
                    segmentMap.associate(seg.attractions[i].geocoordinates, newAssocAttr);
                }
            }
            
            //you essentially repeat the process for start and end because you need to add the start and end coordinates
            //regardless of whether the segments have already been added

            
            GeoCoord geoStart = seg.segment.start;                     //map start points to street segments
            vector<StreetSegment>* strSegsStart = segmentMap.find(geoStart);     //find an existing vector for that geocoord
            
            if (strSegsStart != nullptr) {
                vector<StreetSegment>::iterator it = (*strSegsStart).begin();
                while (it != (*strSegsStart).end()) {
                    if (*it == seg)
                        break;
                    it++;
                }
                if (!(*strSegsStart).empty() && it != (*strSegsStart).end()) {
                    //continue;
                }
                else {                                  //don't need continue here because you still want to run the end coord checks
                    (*strSegsStart).push_back(seg);
                }
            }
            
            
            else {
                vector<StreetSegment> newAssoc1;
                newAssoc1.push_back(seg);
                segmentMap.associate(geoStart, newAssoc1);
            }
            
            
            GeoCoord geoEnd = seg.segment.end;
            vector<StreetSegment>* strSegsEnd = segmentMap.find(geoEnd);        //map end points
            
            if (strSegsEnd != nullptr) {
                vector<StreetSegment>::iterator it2 = (*strSegsEnd).begin();
                while (it2 != (*strSegsEnd).end()) {
                    if (*it2 == seg)
                        break;
                    it2++;
                }
                if (!(*strSegsEnd).empty() && it2 != (*strSegsEnd).end()) {
                    //continue;
                }
                else {
                    (*strSegsEnd).push_back(seg);
                }
            }                   //don't need continue here because you still want to run the end coord checks
            
            
            else {
                vector<StreetSegment> newAssoc2;
                newAssoc2.push_back(seg);
                segmentMap.associate(geoEnd, newAssoc2);
            }
            
            
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    vector<StreetSegment> segments;
    if (segmentMap.find(gc) != nullptr) {
        segments = *segmentMap.find(gc);
    }
    
	return segments;  // This compiles, but may not be correct
}




//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
