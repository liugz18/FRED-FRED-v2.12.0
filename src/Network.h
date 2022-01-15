/*
  This file is part of the FRED system.

  Copyright (c) 2010-2015, University of Pittsburgh, John Grefenstette,
  Shawn Brown, Roni Rosenfield, Alona Fyshe, David Galloway, Nathan
  Stone, Jay DePasse, Anuroop Sriram, and Donald Burke.

  Licensed under the BSD 3-Clause license.  See the file "LICENSE" for
  more information.
*/

//
//
// File: Network.h
//

#ifndef _FRED_NETWORK_H
#define _FRED_NETWORK_H

#include "Mixing_Group.h"
class Disease;

class Network : public Mixing_Group {
public: 

  // place type codes
  static char TYPE_NETWORK;

  static char SUBTYPE_NONE;
  static char SUBTYPE_TRANSMISSION;
  static char SUBTYPE_SEXUAL_PARTNER;

  Network(const char* lab);
  ~Network() {}

  static void get_parameters();

  /**
   * Get the transmission probability for a given disease between two Person objects.
   *
   * @see Mixing_Group::get_transmission_probability(int disease_id, Person* i, Person* s)
   */
  double get_transmission_probability(int disease_id, Person* i, Person* s) {
    return 1.0;
  }

  /**
   * @see Mixing_Group::get_transmission_prob(int disease_id, Person* i, Person* s)
   *
   * This method returns the value from the static array <code>Household::Household_contact_prob</code> that
   * corresponds to a particular age-related value for each person.<br />
   * The static array <code>Household_contact_prob</code> will be filled with values from the parameter
   * file for the key <code>household_prob[]</code>.
   */
  double get_transmission_prob(int disease_id, Person* i, Person* s);
  double get_contacts_per_day(int disease_id);

  double get_contact_rate(int day, int disease_id);
  int get_contact_count(Person* infector, int disease_id, int sim_day, double contact_rate);

  int get_group(int disease, Person* per) {
    return 0;
  }

  void print();
  void print_stdout();
  bool is_connected_to(Person* p1, Person* p2);
  bool is_connected_from(Person* p1, Person* p2);
  double get_mean_degree();
  void test();
  void create_random_network(double mean_degree);
  void infect_random_nodes(double pct, Disease* disease);

private:
  static double contacts_per_day;
  static double** prob_transmission_per_contact;
};

#endif // _FRED_NETWORK_H

