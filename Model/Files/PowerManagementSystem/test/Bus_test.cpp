/*
 * Bus_test.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include "Bus.h"
#include "Switchboard.h"

namespace {
class BusTest: public ::testing::Test {
protected:
	Bus b;
	unsigned int nSwitchboard;
	unsigned int nGensets;
	vector<Switchboard> s;
	virtual void SetUp() {
		nGensets = 2;
		nSwitchboard = 3;
		double ratedPower[] = {10., 11.};
		s.resize(nSwitchboard, Switchboard(nGensets,ratedPower,nGensets,ratedPower,0.,1.));
		for (unsigned int i = 0; i < nSwitchboard; i++)
			b.addSwitchboard(&s[i]);
	}
};
TEST_F(BusTest, BusTest) {
	EXPECT_EQ(nSwitchboard, b.getNumberOfSwitchboard());
}

TEST_F(BusTest, BusTestNGensets) {
	for (unsigned int i = 0; i < nSwitchboard; i++)
		EXPECT_EQ(nGensets, b.getSwitchboard(i)->getNumberOfGeneratorSets());
}

TEST_F(BusTest, BusTestNGensetsTot) {
	EXPECT_EQ(nGensets * nSwitchboard, b.getNumberOfGeneratorSets());
}

TEST_F(BusTest, BusTestGetGensets) {
	int gen = 0;
	for (unsigned int  i = 0; i < nSwitchboard; i++) {
		for (unsigned int j = 0; j < nGensets; j++){
		  const GeneratorSet* gb = static_cast<const Bus&>(b).getGeneratorSet(gen++);
			EXPECT_EQ(s[i].getGeneratorSet(j), gb);
		}
	}
}
}
