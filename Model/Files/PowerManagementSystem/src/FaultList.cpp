/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "FaultList.h"
#include <float.h>

FaultList::FaultList() {
	_removeTime = DBL_MAX;
}

FaultList::~FaultList() {
	  for (iterator f = begin(); f != end(); ++f) {
	      delete *f;
	  }
}

void FaultList::removeOldFaults(double time) {
  for (iterator f = begin(); f != end(); ++f) {
    if ((*f)->getTime() + _removeTime <= time) {
      erase(f--);
    }
  }
}
