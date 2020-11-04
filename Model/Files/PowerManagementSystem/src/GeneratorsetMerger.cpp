/*
 * GeneratorsetMerger.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "GeneratorsetMerger.h"

GeneratorSetMerger::GeneratorSetMerger() {
	// TODO Auto-generated constructor stub
	//_generatorSet = vector<Generatorset*>();
}

GeneratorSetMerger::~GeneratorSetMerger() {
	// TODO Auto-generated destructor stub
}

void GeneratorSetMerger::add(GeneratorSet* g) {
	_generatorSet.push_back(g);
}

GeneratorSet* GeneratorSetMerger::get(const unsigned int i) {
	return _generatorSet[i];
}

const GeneratorSet* GeneratorSetMerger::get(const unsigned int i) const{
    return _generatorSet[i];
}

void GeneratorSetMerger::clear() {
	_generatorSet.clear();
}

unsigned long GeneratorSetMerger::size() const{
	return _generatorSet.size();
}

double GeneratorSetMerger::getPowerAvailable() const{
	double r = 0;
	for (unsigned int i = 0; i < size(); i++){
		GeneratorSet* g = _generatorSet[i];
		if(g->isConnected())
			r += g->getPowerAvailable();
	}
	return r;
}

double GeneratorSetMerger::getPowerGenerated() const{
	double r = 0;
	for (unsigned int i = 0; i < size(); i++){
		GeneratorSet* g = _generatorSet[i];
		if(g->isConnected())
			r += g->getPowerGenerated();
	}
	return r;
}

double GeneratorSetMerger::getPowerGeneratedPrevious() const{
	double r = 0;
	for (unsigned int i = 0; i < size(); i++){
		GeneratorSet* g = _generatorSet[i];
		if(g->isConnected())
			r += g->getPowerGeneratedPrevious();
	}
	return r;
}

double GeneratorSetMerger::getPowerRated() const{
	double r = 0;
	for (unsigned int i = 0; i < size(); i++){
		GeneratorSet* g = _generatorSet[i];
		if(g->isConnected())
			r += g->getPowerRated();
	}
	return r;
}
