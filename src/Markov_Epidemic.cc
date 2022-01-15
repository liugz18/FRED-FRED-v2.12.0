/*
  This file is part of the FRED system.

  Copyright (c) 2010-2015, University of Pittsburgh, John Grefenstette,
  Shawn Brown, Roni Rosenfield, Alona Fyshe, David Galloway, Nathan
  Stone, Jay DePasse, Anuroop Sriram, and Donald Burke.

  Licensed under the BSD 3-Clause license.  See the file "LICENSE" for
  more information.
*/

// File Markov_Epidemic.cc

#include "Events.h"
#include "Date.h"
#include "Disease.h"
#include "Global.h"
#include "Health.h"
#include "Markov_Epidemic.h"
#include "Markov_Natural_History.h"
#include "Markov_Model.h"
#include "Person.h"
#include "Population.h"
#include "Place.h"
#include "Random.h"
#include "Utils.h"

Markov_Epidemic::Markov_Epidemic(Disease* _disease) :
  Epidemic(_disease) {
}


void Markov_Epidemic::setup() {

  Epidemic::setup();

  // initialize Markov specific-variables here:

  this->markov_model = static_cast<Markov_Natural_History*>(this->disease->get_natural_history())->get_markov_model();
  FRED_VERBOSE(0, "Markov_Epidemic(%s)::setup\n", this->disease->get_disease_name());
  this->number_of_states = this->markov_model->get_number_of_states();
  FRED_VERBOSE(0, "Markov_Epidemic::setup states = %d\n", this->number_of_states);

  // this->people_in_state = new person_vector_t [this->number_of_states];
  this->count = new int [this->number_of_states];
  this->transition_to_state_event_queue = new Events* [this->number_of_states];

  for (int i = 0; i < this->number_of_states; i++) {
    this->transition_to_state_event_queue[i] = new Events;
  }

  FRED_VERBOSE(0, "Markov_Epidemic(%s)::setup finished\n", this->disease->get_disease_name());
}


void Markov_Epidemic::prepare() {

  FRED_VERBOSE(0, "Markov_Epidemic(%s)::prepare\n", this->disease->get_disease_name());

  for (int i = 0; i < this->number_of_states; i++) {
    // this->people_in_state[i].reserve(Global::Pop.get_pop_size());
    // this->people_in_state[i].clear();
    this->count[i] = 0;
  }

  // initialize the population
  int day = 0;
  int popsize = Global::Pop.get_pop_size();
  for(int p = 0; p < Global::Pop.get_index_size(); ++p) {
    Person* person = Global::Pop.get_person_by_index(p);
    if(person == NULL) {
      continue;
    }
    double age = person->get_real_age();
    int state = this->markov_model->get_initial_state(age);
    transition_person(person, day, state);
  }

  FRED_VERBOSE(0, "Markov_Epidemic(%s)::prepare: state/size: \n", this->disease->get_disease_name());
  for (int i = 0; i < this->number_of_states; i++) {
    FRED_VERBOSE(0, " | %d %s = %d", i, this->markov_model->get_state_name(i).c_str(), this->count[i]);
  }
  FRED_VERBOSE(0, "\n");

  FRED_VERBOSE(0, "Markov_Epidemic(%s)::prepare finished\n", this->disease->get_disease_name());
}


void Markov_Epidemic::markov_updates(int day) {
  FRED_VERBOSE(0, "Markov_Epidemic(%s)::update for day %d\n", this->disease->get_disease_name(), day);

  // handle scheduled transitions to each state
  for (int state = 0; state < this->number_of_states; state++) {

    int size = this->transition_to_state_event_queue[state]->get_size(day);
    FRED_VERBOSE(0, "MARKOV_TRANSITION_TO_STATE %d day %d %s size %d\n", state, day, Date::get_date_string().c_str(), size);
    
    for(int i = 0; i < size; ++i) {
      Person* person = this->transition_to_state_event_queue[state]->get_event(day, i);
      transition_person(person, day, state);
    }

    this->transition_to_state_event_queue[state]->clear_events(day);
  }
  FRED_VERBOSE(0, "Markov_Epidemic(%s)::markov_update finished for day %d\n", this->disease->get_disease_name(), day);
  return;
}



void Markov_Epidemic::update(int day) {
  Epidemic::update(day);
}


void Markov_Epidemic::transition_person(Person* person, int day, int state) {

  int old_state = person->get_health_state(this->id);
  double age = person->get_real_age();

  if (state == old_state && 1 <= age &&
      this->markov_model->get_age_group(age) == this->markov_model->get_age_group(age-1)) { 
    // this is a birthday check-in and no age group change has occurred.
    return;
  }

  // cancel any scheduled transition
  int next_state = person->get_next_health_state(this->id);
  int transition_day = person->get_next_health_transition_day(this->id);
  if (0 <= next_state && day < transition_day) {
    this->transition_to_state_event_queue[next_state]->delete_event(transition_day, person);
  }
  
  // change active list if necessary
  if (old_state != state) {
    if (0 <= old_state) {
      // delete from old list
      count[old_state]--;
      /*
      for (int j = 0; j < people_in_state[old_state].size(); j++) {
	if (people_in_state[old_state][j] == person) {
	  people_in_state[old_state][j] = people_in_state[old_state].back();
	  people_in_state[old_state].pop_back();
	}
      }
      */
    }
    if (0 <= state) {
      // add to active people list
      count[state]++;
      // people_in_state[state].push_back(person);
    }
  }

  // update person's state
  if (old_state != state) {
    person->set_health_state(this->id, state, day);
  }

  // update next event list
  this->markov_model->get_next_state_and_time(day, age, state, &next_state, &transition_day);
  this->transition_to_state_event_queue[next_state]->add_event(transition_day, person);
    
  // update person's next state
  person->set_next_health_state(this->id, next_state, transition_day);

  FRED_VERBOSE(0,"MARKOV TRANSITION day %d %s person %d age %.0f from old_state %d to state %d, next_state %d on day %d\n",
	       day, Date::get_date_string().c_str(), person->get_id(), age, old_state, state, next_state, transition_day);

  // update epidemic counters and person's health chart

  if (old_state <= 0 && state != 0) {

    FRED_VERBOSE(0,"MARKOV TRANSITION day %d %s person %d age %.0f from old_state %d to state %d => become_exposed\n",
		 day, Date::get_date_string().c_str(), person->get_id(), age, old_state, state);

    // infect the person
    person->become_exposed(this->id, NULL, NULL, day);

    // notify the epidemic
    Epidemic::become_exposed(person, day);
  }
  
  if (this->disease->get_natural_history()->get_symptoms(state) > 0.0 && person->is_symptomatic(this->id)==false) {
    // update epidemic counters
    this->people_with_current_symptoms++;
    this->people_becoming_symptomatic_today++;

    // update person's health chart
    person->become_symptomatic(this->disease);
  }

  if (this->disease->get_natural_history()->get_infectivity(state) > 0.0 && person->is_infectious(this->id)==false) {
    // add to active people list
    this->potentially_infectious_people.insert(person);

    // update epidemic counters
    this->exposed_people--;

    // update person's health chart
    person->become_infectious(this->disease);
  }

  if (this->disease->get_natural_history()->get_symptoms(state) == 0.0 && person->is_symptomatic(this->id)) {
    // update epidemic counters
    this->people_with_current_symptoms--;

    // update person's health chart
    person->resolve_symptoms(this->disease);
  }

  if (this->disease->get_natural_history()->get_infectivity(state) == 0.0 && person->is_infectious(this->id)) {
    // update person's health chart
    person->become_noninfectious(this->disease);
  }

  if (old_state > 0 && state == 0) {
    // notify the epidemic
    recover(person, day);
  }

  if (this->disease->get_natural_history()->is_fatal(state)) {
    // update person's health chart
    person->become_case_fatality(day, this->disease);
  }
}


void Markov_Epidemic::report_disease_specific_stats(int day) {
  FRED_VERBOSE(0, "Markov Epidemic %s report day %d \n",this->disease->get_disease_name(),day);
  for (int i = 0; i < this->number_of_states; i++) {
    char str[80];
    strcpy(str,this->markov_model->get_state_name(i).c_str());
    Utils::track_value(day, str, count[i]);
  }
}


void Markov_Epidemic::end_of_run() {

  // print end-of-run statistics here:
  printf("Markov_Epidemic finished\n");

}


void Markov_Epidemic::terminate_person(Person* person, int day) {
  FRED_VERBOSE(0, "MARKOV EPIDEMIC TERMINATE person %d day %d %s\n",
	       person->get_id(), day, Date::get_date_string().c_str());

  // delete from state list
  int state = person->get_health_state(this->id);
  if (0 <= state) {
    count[state]--;
    /*
    for (int j = 0; j < people_in_state[state].size(); j++) {
      if (people_in_state[state][j] == person) {
	people_in_state[state][j] = people_in_state[state].back();
	people_in_state[state].pop_back();
      }
    }
    */
    FRED_VERBOSE(0, "MARKOV EPIDEMIC TERMINATE person %d day %d %s removed from state %d\n",
		 person->get_id(), day, Date::get_date_string().c_str(), state);
  }

  // cancel any scheduled transition
  int next_state = person->get_next_health_state(this->id);
  int transition_day = person->get_next_health_transition_day(this->id);
  if (0 <= next_state && day <= transition_day) {
    printf("person %d delete_event for state %d transition_day %d\n", person->get_id(), next_state, transition_day);
    this->transition_to_state_event_queue[next_state]->delete_event(transition_day, person);
  }

  person->set_health_state(this->id, -1, day);

  // notify Epidemic
  Epidemic::terminate_person(person, day);

  FRED_VERBOSE(0, "MARKOV EPIDEMIC TERMINATE finished\n");
}


