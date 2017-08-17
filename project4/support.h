//
//  support.h
//  project4
//
//  Created by Kubilay Agi on 3/12/17.
//  Copyright © 2017 Kubilay Agi. All rights reserved.
//

#ifndef support_h
#define support_h


#include "provided.h"

bool operator==(const GeoCoord& check1, const GeoCoord& check2);
bool operator>(const GeoCoord& check1, const GeoCoord& check2);
bool operator<(const GeoCoord& check1, const GeoCoord& check2);
bool operator==(const StreetSegment& check1, const StreetSegment& check2);
bool operator>(const StreetSegment& check1, const StreetSegment& check2);
bool operator<(const StreetSegment& check1, const StreetSegment& check2);



#endif /* support_h */
