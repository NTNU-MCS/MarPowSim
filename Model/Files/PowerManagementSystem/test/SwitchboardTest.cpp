/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */


#include "Switchboard.h"


namespace {
  class SwitchboardTest : public ::testing::Test {
  protected:
	  Switchboard* s;
	  int nGensets;
	  double* ratedPower;
	  virtual void SetUp(){
		  nGensets = 2;
		  ratedPower = new double[nGensets];
		  ratedPower[0] = 2.;
		  ratedPower[1] = 3.;
		  s = new Switchboard(nGensets,ratedPower,nGensets,ratedPower,0.,1.);
	  }

	  virtual void TearDown() {
		  delete[] ratedPower;
		  delete s;
	  }

  };


  TEST_F(SwitchboardTest, CheckNumberOfSwitchboard) {
	  EXPECT_DOUBLE_EQ(nGensets,s->getNumberOfGeneratorSets());
  }

  TEST_F(SwitchboardTest, CheckAllNotNone) {
	  for (int i = 0; i<nGensets;i++)
		  EXPECT_TRUE(s->getGeneratorSet(i) != NULL);
  }

  TEST_F(SwitchboardTest, CheckRated) {
	  for (int i = 0; i<nGensets;i++)
		  EXPECT_DOUBLE_EQ(ratedPower[i],s->getGeneratorSet(i)->getPowerRated());
  }
}
