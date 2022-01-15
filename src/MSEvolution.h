/*
  This file is part of the FRED system.

  Copyright (c) 2010-2015, University of Pittsburgh, John Grefenstette,
  Shawn Brown, Roni Rosenfield, Alona Fyshe, David Galloway, Nathan
  Stone, Jay DePasse, Anuroop Sriram, and Donald Burke.

  Licensed under the BSD 3-Clause license.  See the file "LICENSE" for
  more information.
*/

#ifndef _FRED_MSEVOLUTION_H
#define _FRED_MSEVOLUTION_H

#include "Evolution.h"

class Age_Map;
class Disease;
class Person;
class Piecewise_Linear;

class MSEvolution : public Evolution {

public:
  MSEvolution();
  virtual ~MSEvolution();
  virtual void setup(Disease* disease);
  virtual double antigenic_distance(int strain1, int strain2);

protected:
  virtual double residual_immunity(Person* person, int challenge_strain, int day);
  virtual double prob_inf_blocking(int old_strain, int new_strain, int time, double real_age);
  virtual double prob_vac_blocking(int old_strain, int new_strain, int time, double real_age);
  virtual double prob_blocking(int old_strain, int new_strain, int time, double halflife, double prob_blocking);
  virtual double prob_past_infections(Person* infectee, int new_strain, int day);
  virtual double prob_past_vaccinations(Person* infectee, int new_strain, int day);
  virtual double get_prob_taking(Person* infectee, int new_strain, double quantity, int day);
  virtual double prob_inoc(double quantity);

private:
  Age_Map* halflife_inf;
  Age_Map* halflife_vac;
  double prob_inoc_norm;
  double init_prot_inf;
  double init_prot_vac;
  double sat_quantity;
  Piecewise_Linear* protection;
};

#endif

