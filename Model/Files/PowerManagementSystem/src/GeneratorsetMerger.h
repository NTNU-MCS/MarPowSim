/*
 * GeneratorsetMerger.h
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 14, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 *
 *     \brief A class for collecting multiple generator sets
 *
 *     Collects multiple generator sets, such that combined data can easier be gotten.
 *     Only pointers are stored.
 */

#ifndef GENERATORSETMERGER_H_
#define GENERATORSETMERGER_H_

#include "GeneratorSet.h"
#include <vector>

using namespace std;

/** \brief Merger class for virtually group together GeneratorSet */
class GeneratorSetMerger {
public:
	GeneratorSetMerger();
	virtual ~GeneratorSetMerger();
	/// Add a generator set to the container
	void add(GeneratorSet*);

	/// Get pointer to generator set i
	GeneratorSet* get(const unsigned int i);
    /// Get pointer to generator set i
    const GeneratorSet* get(const unsigned int i) const;

	/// Removes all generator sets from container
	void clear();

	/// number of generator sets
	unsigned long size() const;

	/// Total power available for all connected generator sets in container
	double getPowerAvailable() const;
	/// Total power generated for all connected generator sets in container
	double getPowerGenerated() const;
	/// Total power generated for all connected generator sets in container before previous update
	double getPowerGeneratedPrevious() const;
	/// Total power rated for all connected generator sets in container
	double getPowerRated() const;
private:
	vector<GeneratorSet*> _generatorSet;
};

#endif /* GENERATORSETMERGER_H_ */
