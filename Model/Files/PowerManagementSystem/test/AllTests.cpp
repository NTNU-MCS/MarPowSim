/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "gtest/gtest.h"
#include "Bus_test.cpp"
#include "CircuitBreakerOpeningDetectorTest.cpp"
#include "GeneratorSetTest.cpp"
#include "LossOfGeneratorsetDetectorTest.cpp"
#include "PowerAvailableTest.cpp"
#include "PowerManagementTest.cpp"
#include "PowerPlantTest.cpp"
#include "SwitchboardTest.cpp"
#include "FastLoadReductionTest.cpp"
#include "FaultListTest.cpp"

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
