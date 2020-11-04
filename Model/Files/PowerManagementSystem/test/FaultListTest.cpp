/*
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 */

#include <vector>
#include "FaultList.h"
#include "Fault.h"
using namespace std;

namespace {
class FaultListTest: public ::testing::Test {
protected:
	double removeTime;
	FaultList list;

	virtual void SetUp() {
		removeTime = 1423.;
		list.setRemoveTime(removeTime);
	}
};

TEST_F(FaultListTest, removeFaults_Exipred) {
    vector<double> powerA(2);
    powerA[0] = 1.;
    powerA[1] = 2.;
    vector<double> tmp(0.);
    Fault* f = new Fault(0., powerA, tmp, tmp);
    list.push_front(f);
    list.removeOldFaults(removeTime);
    ASSERT_EQ(0U, list.size());
  }

  TEST_F(FaultListTest, removeFaults_NotExipred) {
    vector<double> powerA(2);
    powerA[0] = 1.;
    powerA[1] = 2.;
    vector<double> tmp(0.);
    Fault* f = new Fault(0., powerA, tmp, tmp);
    list.push_back(f);
    list.removeOldFaults(removeTime * .99);
    ASSERT_EQ(1U, list.size());
  }

  TEST_F(FaultListTest, removeFaults_Multiple) {
    vector<double> powerA(2);
    powerA[0] = 1.;
    powerA[1] = 2.;
    vector<double> tmp(0.);
    Fault* f = new Fault(0., powerA, tmp, tmp);
    list.push_back(f);
    f = new Fault(removeTime * 1.5, powerA, tmp, tmp);
    list.push_back(f);
    f = new Fault(-removeTime * .5, powerA, tmp, tmp);
    list.push_back(f);
    list.removeOldFaults(removeTime);
    ASSERT_EQ(1U, list.size());
  }
}
