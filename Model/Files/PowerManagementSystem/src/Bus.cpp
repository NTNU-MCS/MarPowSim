/*
 * Bus.cpp
 *
 *  Copyright: 	NTNU, Trondheim, Norway
 *  Licensed under GPL-3.0
 *  Created:  	May 13, 2013	Torstein Aarseth Bø <torstein.bo@sintef.no>
 *  Revised:	<date>	<author> <description>
 *		   		<date>	<author> <description>
 *
 */

#include "Bus.h"

Bus::Bus(bool hasBusTieBreakerOpened) {
  _hasBusTieBreakerOpened = hasBusTieBreakerOpened;
}

Bus::Bus() {
  _hasBusTieBreakerOpened = false;
}

Bus::~Bus() {
}

void Bus::addSwitchboard(Switchboard* s) {
  _switchboard.push_back(s);
  for (unsigned int i = 0; i < s->getNumberOfGeneratorSets(); i++)
    _generatorsets.add(s->getGeneratorSet(i));

  for (unsigned int i = 0; i < s->getNThrusters(); i++)
    _thrusters.addThruster(s->getThruster(i));

  for (unsigned int i = 0; i < s->getNEmergencyBatteries(); i++)
    _batteries.add(s->getEmergencyBattery(i));

  _loadLP.addLoad(s->getLoadLowPriority());
  _loadHP.addLoad(s->getLoadHighPriority());
  _loadDP.addLoad(s->getLoadDP());
}

void Bus::clear() {
  _switchboard.clear();
  _generatorsets.clear();
  _thrusters.clear();
  _batteries.clear();
  _loadLP.clear();
  _loadHP.clear();
  _loadDP.clear();
}

Switchboard* Bus::getSwitchboard(unsigned int i) {
  return _switchboard[i];
}

unsigned long Bus::getNumberOfSwitchboard() const {
  return _switchboard.size();
}

unsigned long Bus::getNumberOfGeneratorSets() const {
  return _generatorsets.size();
}

GeneratorSet* Bus::getGeneratorSet(unsigned int i) {
  return _generatorsets.get(i);
}

const GeneratorSet* Bus::getGeneratorSet(unsigned int i) const {
  return _generatorsets.get(i);
}

LoadMerger<Load>* Bus::getLoadLowPriority() {
  return &_loadLP;
}

const LoadMerger<Load>* Bus::getLoadLowPriority() const {
  return &_loadLP;
}

LoadMerger<Load>* Bus::getLoadHighPriority() {
  return &_loadHP;
}

const LoadMerger<Load>* Bus::getLoadHighPriority() const {
  return &_loadHP;
}

LoadMerger<DPLoad>* Bus::getLoadDP() {
  return &_loadDP;
}

const LoadMerger<DPLoad>* Bus::getLoadDP() const {
  return &_loadDP;
}

unsigned long Bus::getNumberOfThrusters() const {
  return _thrusters.size();
}

Thruster* Bus::getThruster(unsigned int i) {
  return _thrusters.getThruster(i);
}

const Thruster* Bus::getThruster(unsigned int i) const {
  return _thrusters.getThruster(i);
}

ThrusterMerger* Bus::getThrusters() {
  return &_thrusters;
}

const ThrusterMerger* Bus::getThrusters() const {
  return &_thrusters;
}

GeneratorSetMerger* Bus::getGeneratorsetMerged() {
  return &_generatorsets;
}

const GeneratorSetMerger* Bus::getGeneratorsetMerged() const {
  return &_generatorsets;
}

double Bus::getPowerRated() const {
  return _generatorsets.getPowerRated();
}

double Bus::getPowerAvailable() const {
  return _generatorsets.getPowerAvailable() + _batteries.getPowerAvailable();
}

double Bus::getPowerGenerated() const {
  return _generatorsets.getPowerGenerated() + _batteries.getPowerGenerated();
}
double Bus::getPowerPreviusGenerated() const {
  return _generatorsets.getPowerGeneratedPrevious()
      + _batteries.getPowerGeneratedPrevious();
}

double Bus::getDesiredPower() const {
  return getLoadDP()->getDesiredLoad() + getLoadLowPriority()->getDesiredLoad()
      + getLoadHighPriority()->getDesiredLoad();
}

double Bus::getConsumedPower() const {
  return getLoadDP()->getLoad() + getLoadHighPriority()->getLoad()
      + getLoadLowPriority()->getLoad();
}
