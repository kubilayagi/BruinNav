#include "provided.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;

private:
    size_t numSegments;
    vector<StreetSegment> m_streetSegs;
    string removeSpaces(const string str);
};


string MapLoaderImpl::removeSpaces(string str) {
    string noSpace;
    for (int i = 0; i < str.size(); i++) {
        if (!isspace(str[i])) {
            noSpace += str[i];
        }
    }
    return noSpace;
}

MapLoaderImpl::MapLoaderImpl()
{
    numSegments = 0;
}

MapLoaderImpl::~MapLoaderImpl()         
{
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream myStream;
    myStream.open(mapFile);
    
    if (myStream.fail()) {
        return false;
    }
    
    string segName = "";
    string lat1, lon1, lat2, lon2;
    char badchar;
    int numAttr = 0;
    string attrName, attrLat, attrLon;
    

    
    while(!myStream.eof()) {
        StreetSegment seg;                  
        getline(myStream, segName);
        if (segName == "") {
            break;
        }
        seg.streetName = segName;
        getline (myStream, lat1, ',');
        lat1 = removeSpaces(lat1);
        //getline (myStream, badchar, ' ');       //to prevent the extra space from showing up in the coords
        
        while (isspace(myStream.peek())) {
            //myStream >> badchar;
            myStream.get(badchar);
        }
        
        getline (myStream, lon1, ' ');
        lon1 = removeSpaces(lon1);
        
        getline(myStream, lat2, ',');
        lat2 = removeSpaces(lat2);
        
        while (isspace(myStream.peek())) {
            //myStream >> badchar;                    //removes preceding spaces before longitudes bc it doesn't work otherwise
            myStream.get(badchar);
        }
        
        getline(myStream, lon2);
        lon2 = removeSpaces(lon2);
        
        myStream >> numAttr;
        myStream.ignore(10000, '\n');
        //numAttr = removeSpaces(numAttr);
        
        GeoCoord start(lat1, lon1);
        GeoCoord end(lat2, lon2);
        GeoSegment geoSeg(start, end);
        seg.segment = geoSeg;
        
        
//        cout << seg.streetName << endl;
//        cout << lat1 << lon1 << lat2 << lon2 << endl;
//        cout << numAttr << endl;
        

        if (numAttr != 0) {
            //int run = atoi(numAttr.c_str());
            while (numAttr > 0) {
                Attraction attrObj;
                getline (myStream, attrName, '|');
                //attrName = removeSpaces(attrName);
                
                getline (myStream, attrLat, ',');
                attrLat = removeSpaces(attrLat);
                
                //cerr << attrName << endl;
                
                //getline (myStream, badchar, ' ');
                
                getline (myStream, attrLon);
                attrLon = removeSpaces(attrLon);
                
                GeoCoord attractionCoords(attrLat, attrLon);
                attrObj.geocoordinates = attractionCoords;
                attrObj.name = attrName;
                seg.attractions.push_back(attrObj);
                numAttr--;
            }
        }
        m_streetSegs.push_back(seg);
        numSegments++;
    }
    
    if (myStream.eof()) {
        myStream.close();
        return true;
    }
    
    return false;  // This compiles, but may not be correct
}

size_t MapLoaderImpl::getNumSegments() const
{
	return numSegments; // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const     //TODO: change to set all properties equal separately
{
    if (segNum < 0 || segNum >= m_streetSegs.size()) {                //size is always greater than zero
        return false;  // This compiles, but may not be correct
    }
    seg = m_streetSegs[segNum];
    return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
