/**
 * landmarkAbstract.cpp
 *
 * \date 10/03/2010
 * \author jsola@laas.fr
 *
 *  \file landmarkAbstract.cpp
 *
 *  ## Add a description here ##
 *
 * \ingroup rtslam
 */

#include "rtslam/landmarkAbstract.hpp"
#include "rtslam/observationAbstract.hpp"
#include "rtslam/mapAbstract.hpp"

namespace jafar {
	namespace rtslam {
		using namespace std;

		IdFactory LandmarkAbstract::landmarkIds = IdFactory();

		/*
		 * constructor.
		 */
		LandmarkAbstract::LandmarkAbstract(const map_ptr_t & _mapPtr, const size_t _size) :
			MapObject(_mapPtr, _size)
		{
			category = LANDMARK;
		}

		LandmarkAbstract::LandmarkAbstract(const simulation_t dummy, const map_ptr_t & _mapPtr, const size_t _size) :
			MapObject(_mapPtr, _size, UNFILTERED)
		{
			category = LANDMARK;
		}

		LandmarkAbstract::~LandmarkAbstract() {
			for (ObservationList::iterator obsIter = observationList().begin(); obsIter != observationList().end(); obsIter++)
			{
				observation_ptr_t obsPtr = *obsIter;
				obsPtr->sensorPtr()->ParentOf<ObservationAbstract>::unregisterChild(obsPtr);
//				cout << "Should unregister obs: " << obsPtr->id() << endl;
			}
//			cout << "Deleted landmark: " << id() << endl;
		}

		bool LandmarkAbstract::needToReparametrize(DecisionMethod repMet){
			switch (repMet) {
				case ANY : {
					for (ObservationList::iterator obsIter = observationList().begin(); obsIter != observationList().end(); obsIter++)
					{
						observation_ptr_t obsPtr = *obsIter;
						// Drastic option: ANY vote for reparametrization declares the need of reparametrization.
						if (obsPtr->voteForReparametrizeLandmark()) return true;
					}
					return false;
				}
				case ALL : {
					for (ObservationList::iterator obsIter = observationList().begin(); obsIter != observationList().end(); obsIter++)
					{
						observation_ptr_t obsPtr = *obsIter;
						// Magnanimous option: ALL votes for reparametrization necessary to declare the need of reparametrization.
						if (!obsPtr->voteForReparametrizeLandmark()) return false;
					}
					return true;
				}
				case MAJORITY : {
					int nRepar = 0, nKeep = 0;
					for (ObservationList::iterator obsIter = observationList().begin(); obsIter != observationList().end(); obsIter++)
					{
						observation_ptr_t obsPtr = *obsIter;
						// Democratic option: MAJORITY votes declares the need of reparametrization.
						if (obsPtr->voteForReparametrizeLandmark()) nRepar++;
						else nKeep++;
					}
					if (nRepar > nKeep) return true;
					return false;

				}
				default : {
					cout << __FILE__ << ":" << __LINE__ << ": Bad evaluation method. Using ANY." << endl;
					return needToReparametrize(ANY);
				}
			}
			return false;
		}



		void LandmarkAbstract::reparametrize() {
			//TODO Implement reparametrize():
			// - create a new STD landmark.
			// - create its set of observations, one per sensor.
			// - Link the landmark to map and observations.
			// - Link the sensors to the new observations.
			// - call reparametrize_func()
			// - compute the new ind_array as a sub-set of the old one
			// - call filter->reparametrize()
			// - transfer info from the old lmk to the new one
			// - transfer info from old obs to new obs.
			// - delete old lmk <-- this will delete all old obs!
		}

		bool LandmarkAbstract::needToDie(DecisionMethod dieMet){
			switch (dieMet) {
				case ANY : {
					for (ObservationList::iterator obsIter = observationList().begin(); obsIter != observationList().end(); obsIter++)
					{
						observation_ptr_t obsPtr = *obsIter;
						// Drastic option: ANY vote for killing declares the need to die.
						if (obsPtr->voteForKillingLandmark()) return true;
					}
					return false;
				}
				case ALL : {
					for (ObservationList::iterator obsIter = observationList().begin(); obsIter != observationList().end(); obsIter++)
					{
						observation_ptr_t obsPtr = *obsIter;
						// Magnanimous option: ALL votes for killing necessary to declare the need to die.
						if (!obsPtr->voteForKillingLandmark()) return false;
					}
					return true;
				}
				case MAJORITY : {
					int nDie = 0, nSurvive = 0;
					for (ObservationList::iterator obsIter = observationList().begin(); obsIter != observationList().end(); obsIter++)
					{
						observation_ptr_t obsPtr = *obsIter;
						// Democratic option: MAJORITY votes declares the need to die.
						if (obsPtr->voteForKillingLandmark()) nDie++;
						else nSurvive++;
					}
					if (nDie > nSurvive) return true;
					return false;

				}
				default : {
					cout << __FILE__ << ":" << __LINE__ << ": Bad evaluation method. Using ANY." << endl;
					return needToDie(ANY);
				}
			}
			return false;
		}

		void LandmarkAbstract::suicide(){
			landmark_ptr_t selfPtr = shared_from_this();
			mapPtr()->liberateStates(state.ia()); // remove from map
			mapPtr()->ParentOf<LandmarkAbstract>::unregisterChild(selfPtr); // remove from graph
		}

	}
}
