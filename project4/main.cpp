// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include "MyMap.h"


// This is the BruinNav main routine.  If the executable built from this file
// and the other .cpp files you write is named BruinNav (or BruinNav.exe on
// Windows), then you can run it with
//  ./BruinNav theMapDataFileName "Start Attraction" "End Attraction"
// to get the turn-by-turn instructions a user wants to see, or
//  ./BruinNav theMapDataFileName "Start Attraction" "End Attraction" -raw
// to see the sequence of NavSegments produced by Navigator::navigate()
// (Under Windows, say "BruinNav" instead of "./BruinNav")
// For example, with the mapdata.txt file we supplied you,
//  ./BruinNav mapdata.txt "Harvard-Westlake Middle School" "GreyStone Mansion"
// should produce something like
//   Routing...
//   You are starting at: Harvard-Westlake Middle School
//   Proceed 0.47 miles southeast on Brooklawn Drive
//   Turn right onto Angelo Drive
//   Proceed 0.43 miles east on Angelo Drive
//   Turn right onto Benedict Canyon Drive
//   Proceed 0.13 miles southeast on Benedict Canyon Drive
//   Turn left onto Hartford Way
//   Proceed 0.13 miles east on Hartford Way
//   Turn left onto Lexington Road
//   Proceed 0.63 miles east on Lexington Road
//   Turn right onto Alpine Drive
//   Proceed 0.07 miles southeast on Alpine Drive
//   Turn left onto West Sunset Boulevard
//   Proceed 0.20 miles northeast on West Sunset Boulevard
//   Turn left onto Mountain Drive
//   Proceed 0.15 miles northeast on Mountain Drive
//   Turn left onto Schuyler Road
//   Proceed 0.19 miles north on Schuyler Road
//   Turn right onto Stonewood Drive
//   Proceed 0.15 miles northeast on Stonewood Drive
//   You have reached your destination: GreyStone Mansion
//   Total travel distance: 2.5 miles
// and
//  ./BruinNav mapdata.txt "Harvard-Westlake Middle School" "GreyStone Mansion" -raw
// might produce 133 lines starting
//   Start: Harvard-Westlake Middle School
//   End:   GreyStone Mansion
//   34.0904161,-118.4344198 34.0905309,-118.4343340 northeast 0.0093 Brooklawn Drive
//   34.0905309,-118.4343340 34.0904815,-118.4341398 east 0.0116 Brooklawn Drive
//   34.0904815,-118.4341398 34.0903824,-118.4339467 southeast 0.0130 Brooklawn Drive
//   34.0903824,-118.4339467 34.0902310,-118.4337702 southeast 0.0145 Brooklawn Drive
//   34.0902310,-118.4337702 34.0900681,-118.4335630 southeast 0.0163 Brooklawn Drive
//   34.0900681,-118.4335630 34.0899633,-118.4334635 southeast 0.0092 Brooklawn Drive
//   34.0899633,-118.4334635 34.0897917,-118.4333739 southeast 0.0129 Brooklawn Drive
//   34.0897917,-118.4333739 34.0894654,-118.4333087 south 0.0229 Brooklawn Drive
// and ending
//   34.0904163,-118.4036377 34.0905573,-118.4036590 north 0.0098 Schuyler Road
//   34.0905573,-118.4036590 34.0908428,-118.4038080 northwest 0.0215 Schuyler Road
//   turn right Stonewood Drive
//   34.0908428,-118.4038080 34.0908832,-118.4036471 east 0.0096 Stonewood Drive
//   34.0908832,-118.4036471 34.0908732,-118.4034846 east 0.0093 Stonewood Drive
//   34.0908732,-118.4034846 34.0908807,-118.4033732 east 0.0064 Stonewood Drive
//   34.0908807,-118.4033732 34.0909505,-118.4031144 east 0.0156 Stonewood Drive
//   34.0909505,-118.4031144 34.0909630,-118.4030512 east 0.0037 Stonewood Drive
//   34.0909630,-118.4030512 34.0909256,-118.4029338 east 0.0072 Stonewood Drive
//   34.0909256,-118.4029338 34.0919749,-118.4018226 northeast 0.0964 Stonewood Drive
//
// If you build the program as is, you'll notice the turn-by-turn instructions
// say IN_SOME_DIRECTION instead of east or southwest or some actual direction.
// That's because of the template appearing a few lines below; read the comment
// before it.

#include "provided.h"
//#include "support.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

// START OF WHAT YOU CAN REMOVE ONCE YOU'VE IMPLEMENTED string directionOfLine(const GeoSegment& gs)
// If you want the turn-by-turn directions to give a real direction like
// east or southwest instead of IN_SOME_DIRECTION, you'll need to
// implement the ordinary function
//    string directionOfLine(const GeoSegment& gs)
// to return a string like "east" or "southwest" based on the angle of the
// GeoSegment gs according to the table at the bottom of page 20 of the spec.
// When you do that, you can delete this comment and the template function
// below that appears here solely to allow this main.cpp to build.
// Why didn't we just write the real function for you?  Because it's also
// a function you'd find useful in producing the NavSegments in the navigate()
// method.  Since it's useful in more than one .cpp file, its declaration
// should go in support.h and its implementation in support.cpp.


string directionOfLine(const GeoSegment& gs)
{
    double angle = angleOfLine(gs);
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
// END OF WHAT YOU CAN REMOVE ONCE YOU'VE IMPLEMENTED string directionOfLine(const GeoSegment& gs)

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments);
void printDirections(string start, string end, vector<NavSegment>& navSegments);

int main(int argc, char *argv[])
{
    bool raw = false;
    if (argc == 5  &&  strcmp(argv[4], "-raw") == 0)
    {
        raw = true;
        argc--;
    }
    if (argc != 4)
    {
        cout << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\"" << endl
        << "or" << endl
        << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\" -raw" << endl;
        return 1;
    }
    
    Navigator nav;
    
    if ( ! nav.loadMapData(argv[1]))
    {
        cout << "Map data file was not found or has bad format: " << argv[1] << endl;
        return 1;
    }
    
    if ( ! raw)
        cout << "Routing..." << flush;
    
    string start = argv[2];
    string end = argv[3];
    vector<NavSegment> navSegments;
    
    NavResult result = nav.navigate(start, end, navSegments);
    if ( ! raw)
        cout << endl;
    
    switch (result)
    {
        case NAV_NO_ROUTE:
            cout << "No route found between " << start << " and " << end << endl;
            break;
        case NAV_BAD_SOURCE:
            cout << "Start attraction not found: " << start << endl;
            break;
        case NAV_BAD_DESTINATION:
            cout << "End attraction not found: " << end << endl;
            break;
        case NAV_SUCCESS:
            if (raw)
                printDirectionsRaw(start, end, navSegments);
            else
                printDirections(start, end, navSegments);
            break;
    }
}

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments)
{
    cout << "Start: " << start << endl;
    cout << "End:   " << end << endl;
    cout.setf(ios::fixed);
    cout.precision(4);
    for (auto ns : navSegments)
    {
        switch (ns.m_command)
        {
            case NavSegment::PROCEED:
                cout << ns.m_geoSegment.start.latitudeText << ","
                << ns.m_geoSegment.start.longitudeText << " "
                << ns.m_geoSegment.end.latitudeText << ","
                << ns.m_geoSegment.end.longitudeText << " "
                << ns.m_direction << " "
                << ns.m_distance << " "
                << ns.m_streetName << endl;
                break;
            case NavSegment::TURN:
                cout << "turn " << ns.m_direction << " " << ns.m_streetName << endl;
                break;
        }
    }
}

void printDirections(string start, string end, vector<NavSegment>& navSegments)
{
    cout.setf(ios::fixed);
    cout.precision(2);
    
    cout << "You are starting at: " << start << endl;
    
    double totalDistance = 0;
    string thisStreet;
    GeoSegment effectiveSegment;
    double distSinceLastTurn = 0;
    
    for (auto ns : navSegments)
    {
        switch (ns.m_command)
        {
            case NavSegment::PROCEED:
                if (thisStreet.empty())
                {
                    thisStreet = ns.m_streetName;
                    effectiveSegment.start = ns.m_geoSegment.start;
                }
                effectiveSegment.end = ns.m_geoSegment.end;
                distSinceLastTurn += ns.m_distance;
                totalDistance += ns.m_distance;
                break;
            case NavSegment::TURN:
                if (distSinceLastTurn > 0)
                {
                    cout << "Proceed " << distSinceLastTurn << " miles "
                    << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
                    thisStreet.clear();
                    distSinceLastTurn = 0;
                }
                cout << "Turn " << ns.m_direction << " onto " << ns.m_streetName << endl;
                break;
        }
    }
    
    if (distSinceLastTurn > 0)
        cout << "Proceed " << distSinceLastTurn << " miles "
        << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
    cout << "You have reached your destination: " << end << endl;
    cout.precision(1);
    cout << "Total travel distance: " << totalDistance << " miles" << endl;
}


//////////////////////////////////////////////////////////////////////////////////

//
//
//using namespace std;
//
////#include "provided.h" // defines class AttractionMapper
//void example(const MapLoader& ml)
//{
//    AttractionMapper am;
//    am.init(ml); // let our object build its internal data structures
//    // by iterating thru all segments from the MapLoader object
//    GeoCoord fillMe;
//    string attraction = "The Coffee Bean & Tea Leaf";
//    bool found = am.getGeoCoord(attraction, fillMe);
//    if ( ! found)
//    {
//        cout << "No geolocation found for " << attraction << endl;
//        return;
//    }
//    cout << "The location of " << attraction << " is " << fillMe.latitude << ", " << fillMe.longitude << endl;
//}
//
//void foo()
//{
//    MyMap<string,double> nameToGPA; // maps student name to GPA
//    // add new items to the binary search tree-based map
//    nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
//    nameToGPA.associate("David", 3.99); // David beat Carey
//    nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
//    nameToGPA.associate("Linda", 3.1);
//    
//    double* davidsGPA = nameToGPA.find("David");
//    if (davidsGPA != nullptr)
//        *davidsGPA = 1.5; // after a re-grade of David’s exam
//    
//    nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
//    // replaces old 3.5 GPA
//    double* lindasGPA = nameToGPA.find("Linda");
//    if (lindasGPA == nullptr)
//        cout << "Linda is not in the roster!" << endl;
//    else
//        cout << "Linda’s GPA is: " << *lindasGPA << endl; 
//
//}
//
//void example2(const MapLoader& ml)
//{
//    SegmentMapper sm;
//    sm.init(ml); // let our object build its internal data structures
//    // by iterating thru all segments from the MapLoader object
//    GeoCoord lookMeUp("34.0572000", "-118.4417620");
//    std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));
//    if (vecOfAssociatedSegs.empty())
//    {
//        cout << "Error - no segments found matching this coordinate\n";
//        return;
//    }
//    cout << "Here are all the segments associated with your coordinate:" << endl;
//    for (auto s: vecOfAssociatedSegs)
//    {
//        cout << "Segment’s street: " << s.streetName << endl;
//        cout << "Segment’s start lat/long: " << s.segment.start.latitude << ", " <<
//        s.segment.start.longitude << endl;
//        cout << "Segment’s end lat/long: " << s.segment.end.latitude << ", " <<
//        s.segment.end.longitude << endl;
//        cout << "This segment has " << s.attractions.size() <<
//        " attractions on it." << endl;
//    }
//}
//
//
//void testNavSegments(vector<NavSegment> ns) {
//    for (int i = 0; i < ns.size(); i++) {
//        if (ns[i].m_command == NavSegment::PROCEED) {
//            cout << "directions[" << i << "]:" << endl;
//            cout << "   type: PROCEED" << endl;
//            cout << "   start: " << ns[i].m_geoSegment.start.latitudeText << ", " << ns[i].m_geoSegment.start.longitudeText << endl;
//            cout << "   end: " << ns[i].m_geoSegment.end.latitudeText << ", " << ns[i].m_geoSegment.end.longitudeText << endl;
//            cout << "   direction: " << ns[i].m_direction << endl;
//            cout << "   distance: " << ns[i].m_distance << endl;
//            cout << "   street: " << ns[i].m_streetName << endl;
//        } else {
//            cout << "directions[" << i << "]:" << endl;
//            cout << "   type: TURN" << endl;
//            cout << "   direction: " << ns[i].m_direction << endl;
//            cout << "   street: " << ns[i].m_streetName << endl;
//        }
//    }
//}
//
//
//
//int main()
//{
////    MapLoader ml;
////    ml.load("/Users/kubilay/Desktop/CS32/project4/project4/mapdata.txt");
////    foo();
////    example(ml);
////    example2(ml);
////    
////    GeoCoord a("-1", "-1");
////    GeoCoord b("100", "100");
////    
////    priority_queue<int, vector<int>, greater<int>> test;
////    
////    test.push(10);
////    test.push(5);
////    test.push(1);
////    
////    for (int i = 0; i < 3; i++) {
////        cout << test.top() << endl;
////        test.pop();
////    }
//    
//    vector<NavSegment> ns;
//    Navigator n;
//    //n.navigate("1031 Broxton Avenue", "The Maltz Park", ns);
//    //n.navigate("Fatburger", "Bollywood Bites", ns);
//    //n.navigate("Geffen Playhouse", "Parking 2", ns);        //repeats at tiverton
//    //n.navigate("1031 Broxton Avenue", "1061 Broxton Avenue", ns);
//    n.navigate("Fox Plaza", "Hedrick Hall", ns);
//    
//    //n.navigate("West Hollywood Branch County of Los Angeles Public Library" "Brentwood Country Mart", ns);
//    //n.navigate("The Troubadour", "1 Santa Monica Canyon", ns);
//    //n.navigate("Pierce Brothers Westwood Village Memorial Park", "CVS", ns);
//    //n.navigate("Brentwood Country Mart", "CVS", ns);
//    //n.navigate("Harold M. Williams Auditorium", "Getty Center Helipad", ns);
//    //n.navigate("Bollywood Bites", "Hedrick Hall", ns);
//    //n.navigate("Giorgio Armani", "Hedrick Hall", ns);
//    //n.navigate("1061 Broxton Avenue", "1031 Broxton Avenue", ns);
//    //n.navigate("Hedrick Hall", "Bollywood Bites", ns);
//    //n.navigate("Bollywood Bites", "Getty Center Helipad", ns);      //shouldn't work
//    //n.navigate("Geffen Playhouse", "CVS", ns);
//    //n.navigate("Easton Softball Stadium", "Hedrick Hall", ns);
//    //n.navigate("Family Pool", "Easton Softball Stadium", ns);
//    //n.navigate("CVS", "Getty Center Helipad", ns);       //shouldn't work
//    //n.navigate("1160 Barry Avenue", "Fox Plaza", ns);
//    //n.navigate("Mr. C Beverly Hills", "2049 Century Park East", ns);
//    //n.navigate("Charles E. Young Research Library", "Life Sciences", ns);
//    //n.navigate("Bollywood Bites", "Pierce Brothers Westwood Village Memorial Park", ns);
//        //n.navigate("University of California, Los Angeles", "Layla Cafe Restaurant & Catering", ns);
//    //n.navigate("Denny's", "Diddy Riese", ns);
//    //n.navigate("Brentwood Country Mart", "Siam Chan", ns);
//    //n.navigate("Triangle Fraternity", "CVS", ns);
//    //n.navigate("10929 Wilshire Boulevard", "Stage 14", ns);     //STILL DOESN'T WORK
//    //n.navigate("10929 Wilshire Boulevard", "Cheviot Hills Recreation Center", ns);    //works???
//    //n.navigate("10929 Wilshire Boulevard", "10201 West Pico Boulevard", ns);
//    //n.navigate("Micky's", "Cheviot Hills Recreation Center", ns);
//    //n.navigate("Stage 10", "10929 Wilshire Boulevard", ns);
//    //n.navigate("10929 Wilshire Boulevard", "Museum of Tolerance", ns);        //works wtf, crosses samo blvd
//    //n.navigate("13 Santa Monica Canyon", "1 Santa Monica Canyon", ns);      //not sure if this is right bc can't see the roads
//    //n.navigate("Ralphs", "Headlines!", ns);
//    
//    //n.navigate("CVS", "CVS", ns);
//    
//    
//    //testNavSegments(ns);
//    
//    
//    cout << ns.size() << endl;
//    for (int nityan = 0; nityan < ns.size(); nityan++) {
//        if (ns[nityan].m_command == NavSegment::PROCEED) {
//        cout << "Proceed " << ns[nityan].m_distance << " miles " << ns[nityan].m_direction << " on " << ns[nityan].m_streetName << endl;
//        }
//        else
//            cout << "Take a " << ns[nityan].m_direction << " turn on " << ns[nityan].m_streetName << endl;
//    }
//    
////    if (b > a)
////        cout << "yup" << endl;
//    
////    ifstream theFile;
////    
////    theFile.open("/Users/kubilay/Desktop/CS32/project4/project4/mapdata.txt");
////    string poop;
////    string fuck, shit, ass, ng, toir, woe;
////    
////
////    getline (theFile, poop);
////    getline (theFile, fuck, ',');
////    getline (theFile, shit, ' ');
////    getline(theFile, ass, ' ');
////    getline(theFile, ng, ',');
////    getline(theFile, toir);
////    getline(theFile, woe);
////
//////    getline (theFile, fuck);
//////    getline (theFile, shit);
//////    getline (theFile, ass);
//////    getline (theFile, ng);
//////    getline (theFile, toir);
//////    getline (theFile, woe);
////    
////    cout << poop << endl;
////    cout << fuck << endl;
////    //cout << shit << endl;
////    cout << ass << endl;
////    cout << ng << endl;
////    cout << toir << endl;
////    cout << woe << endl;
////    
////    theFile.close();
//    
////    MapLoader blue;
////    if (blue.load("/Users/kubilay/Desktop/CS32/project4/project4/mapdata.txt"))
////        cout << "that's right boii" << endl;
////    
////    cout << blue.getNumSegments() << endl;
//    
//
//}
