#include "provided.h"
#include <string>
#include "MyMap.h"
#include <iostream>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> attrMap;
    int numAttractions;
    string toLowerCase (const string &convert) const;
};

string AttractionMapperImpl::toLowerCase(const string& convert) const {
    string returnThis;
    for (int i = 0; i < convert.size(); i++) {              //to avoid case sensitivity
        returnThis += tolower(convert[i]);
    }
    return returnThis;
}

AttractionMapperImpl::AttractionMapperImpl()
{
    numAttractions = 0;
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    StreetSegment seg;
    for (int i = 0; i < ml.getNumSegments(); i++) {
        if (ml.getSegment(i, seg)) {
            for (int j = 0; j < seg.attractions.size(); j++) {
                attrMap.associate(toLowerCase(seg.attractions[j].name), seg.attractions[j].geocoordinates);
                numAttractions++;
                //cerr << seg.attractions[j].name << endl;
            }
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    const GeoCoord* temp = attrMap.find(toLowerCase(attraction));
    if (temp != nullptr) {
        gc = *temp;
        return true;
    }
	return false;  // This compiles, but may not be correct
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
