/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "GeneratorSet.h"
namespace {
class GeneratorSetTest: public ::testing::Test {
protected:

};
TEST_F(GeneratorSetTest, GeneratorSet) {
	//			on, generated, power available
	GeneratorSet g = GeneratorSet(100.);
	g.updateStatus(1, 2, 3);
	g.updateStatus(1, 4, 5);
	ASSERT_DOUBLE_EQ(4, g.getPowerGenerated());
	ASSERT_DOUBLE_EQ(2, g.getPowerGeneratedPrevious());
	ASSERT_DOUBLE_EQ(5, g.getPowerAvailable());
	ASSERT_TRUE(g.isConnected());
	ASSERT_TRUE(g.wasConnected());
	ASSERT_FALSE(g.hasDisconnected());
}
TEST_F(GeneratorSetTest, GeneratorSetDisconnect) {
	GeneratorSet g = GeneratorSet(100.);
	g.updateStatus(1, 6, 7);
	ASSERT_FALSE(g.hasDisconnected());

	g.updateStatus(0, 6, 7);
	ASSERT_TRUE(g.hasDisconnected());
	g.updateStatus(0, 6, 7);
	ASSERT_FALSE(g.hasDisconnected());
}
}
