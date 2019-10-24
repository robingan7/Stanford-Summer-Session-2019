#include "DoctorsWithoutOrders.h"
#include "map.h"
#include <climits>
#include <iostream>
using namespace std;

/* * * * Doctors Without Orders * * * */
Vector< Vector<string> > sumFinder(Vector<Patient> &patients, int sum);

void sumFinderHelp(Vector<Patient> &patients, int sum, Vector< Patient >& chosen,
                                       int numOfPatient,Vector< Vector<string> >& result);

int sumOf(Vector< Patient > patients_);

void findValidSchedule(Vector<Vector< Vector<string> >>& possible_schedule,
                       Vector<Vector<string>>& result, int index, Vector<Patient> &patients);

bool valid(Vector<Vector<string>>& result, Vector<Patient> &patients);

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool canAllPatientsBeSeen(Vector<Doctor> &doctors,
                          Vector<Patient> &patients,
                          Map<string, Set<string>>& schedule) {

    Vector<Vector< Vector<string> >> possible_schedule;//contains possible schdule for each doctor

    for (Doctor d : doctors) {
        possible_schedule.add(sumFinder(patients, d.hoursFree));// assignment valid schdule for each doctor
    }

    Vector<Vector<string>> result;
    findValidSchedule(possible_schedule, result, 0, patients);// find a schdule that works for every doctor

    if(result.size() ==0) {
        return false;
    }

    //assign values to schedule variable
    for (int i = 0; i < doctors.size(); i++) {
        Set<string> current;
        for (string element : result[i]) {
            current.add(element);
        }

        schedule.put(doctors[i].name, current);
        current.clear();
    }
    return true;
}

/**
 * @brief valid check if the result contains a valid result in it
 * @param result: a vector contains vectors of string refer to a possible result
 * @param patient_: the original list of patient_
 * @return if the reuslt is valid that has no same patients, and contains all patients
 */
bool valid(Vector<Vector<string>>& result, Vector<Patient> &patient_) {
    Vector<string> single;// decompose result to a 1-d vector
    Vector<string> names;//contains all patient names

    for (Vector<string> v_str : result) {
        for (string str : v_str) {
            if (!single.contains(str)) {// if single contains same patient return false
                single.add(str);
            } else {
                return false;
            }
        }
    }

    for (Patient p : patient_) {
        if (!single.contains(p.name)) {// if one of the patient is not included return false
            return false;
        }
    }

    return true;
}

/**
 * @brief findValidSchedule: use backtracing find the valid schedule
 * @param possible_schedule: vector that contains possible valid schedule
 * @param result: the result of this method maybe empty if no valid schedule find
 * @param index: the current index of possible_schedule we are testing
 * @param patients: original list of patients
 */
void findValidSchedule(Vector<Vector< Vector<string> >>& possible_schedule,
                       Vector<Vector<string>>& result, int index, Vector<Patient> &patients) {

    if(index < possible_schedule.size()) {
        for (int i = 0; i < possible_schedule[index].size(); i++) {
            //explore
            result.add(possible_schedule[index][i]);

            findValidSchedule(possible_schedule, result,index + 1,patients);//goes on to the next doctor

            //unexplored
            if(!valid(result, patients)) {
                result.removeBack();
            }
        }
    }
}

/**
 * @brief permuteHelper: return all possible permutation of patient groups that have total less or equal to sum
 * @param patients: The list of the patients that need to be seen
 * @param sum: total hour the doctor has.
 * @return all possible permutation of patient groups that have total less or equal to sum
 */
Vector< Vector<string> > sumFinder(Vector<Patient> &patients, int sum) {
    Vector<Patient> chosen;
    Vector< Vector<string> > result;

    for (int i = 1; i <= patients.size(); i++) {
        sumFinderHelp(patients, sum, chosen, i, result);
    }
    return result;
}

/**
 * @brief sumFinderHelp: return all possible permutation of patient groups that have total less or equal to sum
 * @param patients: The list of the patients that need to be seen
 * @param sum: the total hourn the doctor has
 * @param chosen: the patient chosen in current list in calculating the sum
 */
void sumFinderHelp(Vector<Patient> &patients, int sum, Vector< Patient > &chosen,
                                       int numOfPatient, Vector< Vector<string> > &result) {

    if (numOfPatient == 0) {
        if (sumOf(chosen) <= sum) {
            Vector<string> names;

            for (Patient p : chosen) {
                if(!names.contains(p.name)){
                    names.add(p.name);
                }
            }

            if(!result.contains(names)){
                result.add(names);
            }
        }
    } else {
        for (int i = 0; i < patients.size(); i++) {

            chosen.add(patients[i]);//explore

            sumFinderHelp(patients, sum, chosen, numOfPatient -1 , result);

            //unexplored
            if(!chosen.isEmpty()){
                chosen.removeBack();
            }
        }
    }
}

/**
 * @brief the function return the total hour of the vector of patients
 * @param patients_: the vector contains patients
 * @return the sum of hour needed of tyhe group of patients
 */
int sumOf(Vector< Patient > patients_) {
    int sum = 0;

    for (Patient p : patients_) {
        sum += p.hoursNeeded;
    }

    return sum;
}
