//
//  support.cpp
//  project4
//
//  Created by Kubilay Agi on 3/12/17.
//  Copyright © 2017 Kubilay Agi. All rights reserved.
//

#include <stdio.h>
#include "provided.h"
#include "support.h"

bool operator==(const GeoCoord& check1, const GeoCoord& check2)  {
    if (check1.latitude == check2.latitude) {
        if (check1.longitude == check2.longitude){
            return true;
        }
    }
    return false;
}

bool operator>(const GeoCoord& check1, const GeoCoord& check2)  {
    if (check1.latitude > check2.latitude) {
        return true;
    }
    else if (check1.latitude == check2.latitude){
        if (check1.longitude > check2.longitude)
            return true;
    }
    return false;
}


bool operator<(const GeoCoord& check1, const GeoCoord& check2) {
    if (check1.latitude < check2.latitude) {
        return true;
    }
    else if (check1.latitude == check2.latitude){
        if (check1.longitude < check2.longitude)
            return true;
    }
    return false;
}

bool operator==(const StreetSegment& check1, const StreetSegment& check2) {
    if (check1.streetName == check2.streetName) {
        if (check1.segment.start == check2.segment.start && check1.segment.end == check2.segment.end){
            return true;
        }
    }
    return false;
}

bool operator>(const StreetSegment& check1, const StreetSegment& check2) {
    if (check1.streetName > check2.streetName) {
        return true;
    }
    else if (check1.streetName == check2.streetName){
        if (check1.segment.start == check2.segment.start && check1.segment.end == check2.segment.end)
            return false;
    }
    return true;
}


bool operator<(const StreetSegment& check1, const StreetSegment& check2) {
    if (check1.streetName < check2.streetName) {
        return true;
    }
    else if (check1.streetName == check2.streetName){
        if (check1.segment.start == check2.segment.start && check1.segment.end == check2.segment.end)
            return false;
    }
    return true;
}
