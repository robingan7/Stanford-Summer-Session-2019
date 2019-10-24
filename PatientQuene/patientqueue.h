/*
 * Robin Gan | Tommy Young
 * This is the class interface that states the instance variable and method head of member
 * functions and private helper functions.
 */

#ifndef _patientqueue_h
#define _patientqueue_h

#include <iostream>
#include <string>
#include "patient.h"
using namespace std;

class PatientQueue {
public:
    PatientQueue(bool isMinHeap = true);
    ~PatientQueue();
    void changePriority(string value, int newPriority);
    void clear();
    void debug();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const PatientQueue& queue);

private:
    bool isMin;//if it is the min heap or not
    int current_size;//current size of the array
    int array_size;//total size of array
    Patient* list;//the list of patient

    /**
     * @brief Swap the value of two patient elements in the list
     * @param first: the first patient
     * @param second: the second patient
     */
    void swap(Patient& first, Patient& second);

    /**
     * @brief doubleList: double the list size and fill tyhe existed elements
     */
    void doubleList();

    /**
     * @brief findMostUrgent: find the most urgent child element when dequeneing. used in dequene
     * hepler
     * @param curr: the parent index
     */
    void findMostUrgent(int& curr);

    /**
     * @brief enqueneHelper: implments the enquene with starting index. It carries out the
     * "bubble up" part
     * @param index: the starting index, it will be changed as the elements is moved
     */
    void enqueneHelper(int& index);

    /**
     * @brief dequeneHelper: implments the dequene algorithm with starting index. It carries out the
     * "bubble down" part
     * @param index: the starting index, it will be changed as the elements is moved
     */
    void dequeneHelper(int& index);

    /**
      * @brief compare: compare two patient based inMin
      * @param first: first patient in comparsion
      * @param second: second patient in comparsion
      * @return the result we want depend on if the list is min heap or not
      */
    bool compare(Patient first, Patient second);
};

#endif
