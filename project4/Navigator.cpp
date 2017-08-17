#include "provided.h"
#include <string>
#include <vector>
#include <queue>
#include <list>
#include "support.h"
#include <iostream>
#include "MyMap.h"
#include <algorithm>
using namespace std;


struct container {          //creating nodes to use in the A* implementation, stores the coordinate of the node as well as the priority of the node
    GeoCoord coord;
    double value;
    string name;
    bool operator<(const container& a) const {      //we reverse the comparison operators because we want it to be ordered with the min values at the top
        return value > a.value;
    }
    bool operator>(const container& a) const {
        return value < a.value;
    }
    bool operator==(const container& a) const {
        return value == a.value;
    }
};


class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    AttractionMapper attrMapper;
    SegmentMapper segMapper;
    MapLoader maploader;
    string direction(double angle) const;
};


string NavigatorImpl::direction(double angle) const{
    if (0 <= angle && angle <= 22.5) {
        return ("east");
    }
    else if (22.5 < angle && angle <= 67.5) {
        return ("northeast");
    }
    else if (67.5 < angle && angle <= 112.5) {
        return ("north");
    }
    else if (112.5 < angle && angle <= 157.5) {
        return ("northwest");
    }
    else if (157.5 < angle && angle <= 202.5) {
        return ("west");
    }
    else if (202.5 < angle && angle <= 247.5) {
        return ("southwest");
    }
    else if (247.5 < angle && angle <= 292.5) {
        return ("south");
    }
    else if (292.5 < angle && angle <= 337.5) {
        return ("southeast");
    }
    else {
        return ("east");
    }
}

NavigatorImpl::NavigatorImpl()
{
    //loadMapData("/Users/kubilay/Desktop/CS32/project4/project4/mapdata.txt");
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if (maploader.load(mapFile)) {
        attrMapper.init(maploader);     //won't load these if the map doesn't load properly
        segMapper.init(maploader);
        return true;
    }
	return false;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    if (start == end) {
        //cout << "you are already at your destination" << endl;
        return NAV_SUCCESS;
    }
    bool found = false;
    container startCoord, endCoord;                                 //A* implementation
    if (!attrMapper.getGeoCoord(start, startCoord.coord)) {
        return NAV_BAD_SOURCE;
    }
    if (!attrMapper.getGeoCoord(end, endCoord.coord)) {
        return NAV_BAD_DESTINATION;
    }
    
    //A* implementation data structures

    
    MyMap<GeoCoord, double> cost;           //prioritize
    MyMap<GeoCoord, GeoCoord> cameFrom;     //keep track of the path
    priority_queue<container> queue;
    
    vector<StreetSegment> associatedWithStart = segMapper.getSegments(startCoord.coord);    //incase the start or end is on an intersection
    vector<StreetSegment> associateWithEnd = segMapper.getSegments(endCoord.coord);
    
    
    startCoord.value = distanceEarthMiles(startCoord.coord, endCoord.coord);
    
    cost.associate(startCoord.coord, 0);
    cameFrom.associate(startCoord.coord, startCoord.coord);
    queue.push(startCoord);
    
    
    //A* navigation implementation, used redblobgames.com and youtube as a reference for research on A* and Dijkstra's algorithm

    
    while (!queue.empty()) {
        container top = queue.top();
        //cout << top.coord.latitudeText << ", " << top.coord.longitudeText << endl;
        queue.pop();
        
        vector<StreetSegment> associatedWithTop = segMapper.getSegments(top.coord);
        
        for (int i = 0; i < associatedWithTop.size(); i++) {
            for (int j = 0; j < associateWithEnd.size(); j++) {
                if (associatedWithTop[i] == associateWithEnd[j]) {      //if we found the route
                    found = true;
                    //cout << "found" << endl;
                    cameFrom.associate(endCoord.coord, top.coord);
                    //break;//return NAV_SUCCESS;
                }
            }
        }
        
        if (found)
            break;
        
        container nextCoord;

        for (int k = 0; k < associatedWithTop.size(); k++) {        //if we haven't found the final point yet
            
            
            StreetSegment nextSeg = associatedWithTop[k];
            GeoSegment nextGeoSeg = nextSeg.segment;
            
            if (nextGeoSeg.start == top.coord) {        //if we are at the the start of a street segment, we need to push the end
                nextCoord.coord = nextGeoSeg.end;
                
                if (!(*(cameFrom.find(top.coord)) == nextCoord.coord)) {
                    double newCost = *(cost.find(top.coord)) + distanceEarthMiles(top.coord, nextCoord.coord);      //part of the A* implementation
                    
                    if (cost.find(nextCoord.coord) == nullptr || *(cost.find(nextCoord.coord)) > newCost) {
                        cost.associate(nextCoord.coord, newCost);
                        nextCoord.value = distanceEarthMiles(nextCoord.coord, endCoord.coord) + newCost;    //heuristic value (A* implementation)
                        queue.push(nextCoord);
                        cameFrom.associate(nextCoord.coord, top.coord);
                        //cout << nextCoord.coord.latitudeText << ", " << nextCoord.coord.longitudeText << endl;
                    }
                }
            }//segMapper.getSegments(nextCoord.coord).size() != 0
            
            else if (nextGeoSeg.end == top.coord) {
                nextCoord.coord = nextGeoSeg.start;             //if we are at the the end of a street segment, we need to push the start
                
                
                if (!(*(cameFrom.find(top.coord)) == nextCoord.coord)) {
                    double newCost = *(cost.find(top.coord)) + distanceEarthMiles(top.coord, nextCoord.coord);
                    
                    if (cost.find(nextCoord.coord) == nullptr || *(cost.find(nextCoord.coord)) > newCost) {     //prioritize based on cost
                        cost.associate(nextCoord.coord, newCost);
                        nextCoord.value = distanceEarthMiles(nextCoord.coord, endCoord.coord) + newCost;        //if no cost is found, then its a new node
                        queue.push(nextCoord);
                        cameFrom.associate(nextCoord.coord, top.coord);
                        //cout << nextCoord.coord.latitudeText << ", " << nextCoord.coord.longitudeText << endl;
                    }

                }   //segMapper.getSegments(nextCoord.coord).size() != 0
            }
            
            else {      //for the starting coordinate that is in the middle of a segment
                container end, start;
                end.coord = nextGeoSeg.end;
                start.coord = nextGeoSeg.start;
                double endCost = distanceEarthMiles(top.coord, end.coord);
                double startCost = distanceEarthMiles(top.coord, start.coord);
                
                if (segMapper.getSegments(end.coord).size() != 0 || *(cost.find(nextCoord.coord)) > endCost) {
                    end.value = distanceEarthMiles(end.coord, endCoord.coord) + endCost ;
                    cost.associate(end.coord, endCost);
                    queue.push(end);
                    cameFrom.associate(end.coord, top.coord);

                }
                if (segMapper.getSegments(start.coord).size() != 0 || *(cost.find(nextCoord.coord)) > startCost) {
                    start.value = distanceEarthMiles(end.coord, endCoord.coord) + startCost ;
                    cost.associate(start.coord, startCost);
                    queue.push(start);
                    cameFrom.associate(start.coord, top.coord);
                }
            }
        }
    }
    
    if (!found) {
        //cout << "no route" << endl;
        return NAV_NO_ROUTE;
    }
    
    GeoCoord ending = endCoord.coord;
    while (true) {                          //we don't know how long this needs to run so, we just let it run forever until it returns
        
        //cout << ending.latitudeText << ", " << ending.longitudeText << endl;
        
        GeoCoord secondLast = *(cameFrom.find(ending));
        
        vector<StreetSegment> endStreets = segMapper.getSegments(ending);                       //used to find the connections between two points
        vector<StreetSegment> secondToEndStreets = segMapper.getSegments(secondLast);
        
        for (int i = 0; i < endStreets.size(); i++) {                   //find's the right street name
            for (int j = 0; j < secondToEndStreets.size(); j++) {
                if (endStreets[i] == secondToEndStreets[j]) {
                    GeoSegment nextGeoSeg(secondLast, ending);
                    NavSegment navSeg;
                    navSeg.m_geoSegment = nextGeoSeg;                          //creates a nav segment with the correct information based on the back tracking map
                    navSeg.m_direction = direction(angleOfLine(nextGeoSeg));
                    navSeg.m_streetName = endStreets[i].streetName;
                    navSeg.m_distance = distanceEarthMiles(navSeg.m_geoSegment.start, navSeg.m_geoSegment.end);
                    directions.insert(directions.begin(), navSeg);
                }                       //need to insert at beginning because we are traversing in reverse order
            }
        }
        
        //at this point, we've only added in the proceed segments, so we now insert the turn segments based on the existing directions vector
        
        ending = secondLast;
        
        if (*(cameFrom.find(ending)) == ending) {
            vector<NavSegment>::iterator iter = directions.begin();
            
            for (int k = 0; k < directions.size()-1; k++) {
                iter++;
                if (directions[k].m_streetName != directions[k+1].m_streetName) {
                    string direction;
                    if (angleBetween2Lines(directions[k].m_geoSegment, directions[k+1].m_geoSegment) < 180) {
                        direction = "left";
                    }
                    else {
                        direction = "right";
                    }
                    iter = directions.insert(iter, NavSegment(direction, directions[k+1].m_streetName));
                }
            }
            return NAV_SUCCESS;         //at this point, we've inserted the complete path, ignoring the nodes we visited but did not follow in our final path
        }
    }
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}


