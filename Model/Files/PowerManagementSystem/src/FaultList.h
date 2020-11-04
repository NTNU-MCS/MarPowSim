/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#ifndef FAULTLIST_H_
#define FAULTLIST_H_

#include <list>
#include "Fault.h"

using namespace std;

/** \brief List for storing faults
 *
 */
class FaultList{
public:
	FaultList();
	virtual ~FaultList();
	void push_back(const Fault* x) {_faults.push_back(x);}
	void push_front(const Fault* x) {_faults.push_front(x);}
	typedef std::list<const Fault*>::const_iterator const_iterator;
	typedef std::list<const Fault*>::iterator iterator;
	const_iterator begin() const {return _faults.begin();}
	const_iterator end() const {return _faults.end();}

	/** Sets how many seconds a fault should be saved
	 *
	 *  @param[in] removeTime seconds between the fault have been recorded and it should removed
	 */
	void setRemoveTime(double removeTime) {_removeTime = removeTime;}

	/** Remove faults older than remove time	 */
	void removeOldFaults(double time);
	unsigned int size() {return _faults.size();}
private:
    iterator begin() {return _faults.begin();}
    iterator end() {return _faults.end();}
	list<const Fault*> _faults;
	double _removeTime;
	void erase(iterator f){
		delete *f;
		_faults.erase(f);
	}
};

#endif /* FAULTLIST_H_ */
