/*
 * MergeLoad.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "MergeLoad.h"

template<class T> LoadMerger<T>::LoadMerger() {
	// TODO Auto-generated constructor stub
	
}

template<class T> LoadMerger<T>::~LoadMerger() {
	// TODO Auto-generated destructor stub
}

template<class T> void LoadMerger<T>::addLoad(T* load) {
	_load.push_back(load);
}

template<class T> void LoadMerger<T>::clear() {
	_load.clear();
}

template<class T> double LoadMerger<T>::getDesiredLoad() const{
	double r = 0;
	for(unsigned int i = 0; i<_load.size(); i++)
		r += _load[i]->getDesiredLoad();
	return r;
}

template<class T> double LoadMerger<T>::getLoad() const{
	double r = 0;
	for(unsigned int i = 0; i<_load.size(); i++)
		r += _load[i]->getLoad();
	return r;
}

template<class T> double LoadMerger<T>::getPowerAvailable() const{
	double r = 0;
	for(unsigned int i = 0; i<_load.size(); i++)
		r += _load[i]->getPowerAvailable();
	return r;
}

template<class T> const T* LoadMerger<T>::getPartLoad(const unsigned int i) const {
	return _load[i];
}

template<class T> T* LoadMerger<T>::getPartLoad(const unsigned int i) {
	return _load[i];
}

template class LoadMerger<Load>;
#include "DPLoad.h"
template class LoadMerger<DPLoad>;
