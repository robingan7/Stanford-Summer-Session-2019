/*
 * Robin Gan | Tommy Young
 * This program implements the methods and private variable that are declares in the interface
 * It contains the actual algorithm of PatientQueue including enquene, dequene and so on
 */

#include "patientqueue.h"
#include "error.h"

PatientQueue::PatientQueue(bool isMinHeap) {
   array_size = 10;
   current_size = 0;
   list = new Patient[array_size];
   isMin = isMinHeap;
}

PatientQueue::~PatientQueue() {
   delete[] list;
}

void PatientQueue::changePriority(string value, int newPriority) {
    int targetIndex = 0;
    Patient newPatient = Patient(value, newPriority);

    //find if the name exists in the list
    for(int i = 1; i <= current_size; i++) {
        if(list[i].name == value) {
            targetIndex = i;
            break;
        }
    }

    if(targetIndex == 0) {
        throw "name does not exist";
    }else {
        Patient formerPatient = list[targetIndex];//stores the former index for comparsion
        list[targetIndex] = newPatient;//update the value to new patient

        //check if the formerPatient is greater or smaller than new patient
        if(compare(formerPatient, newPatient)) {
            enqueneHelper(targetIndex);//re-sued enquene code
        } else {
            dequeneHelper(targetIndex);//re-sued dequene code
        }
    }
}

void PatientQueue::clear() {
    //reset the sizes and list
    array_size = 0;
    current_size = 0;

    list = new Patient[array_size];
}

void PatientQueue::debug() {
    //cout << debugg << endl;
}

string PatientQueue::dequeue() {
    //if the current size is error, throw the exception
    if(current_size == 0) {
        throw  "list is empty";
    }else {
        //frontmost index before editing the list
        string returnVal = list[1].name;

        //swap the first and last value
        swap(list[1],list[current_size]);

        //clear the last element
        list[current_size] = Patient();

        current_size --;//decrease the current size

        //first child index, it will change as the first element swap with the childs
        int childIndex = 1;

        dequeneHelper(childIndex);
        return returnVal;
    }

}
void PatientQueue::dequeneHelper(int& index) {
    //loops if the index and index times two is smaller or equal to currrent size
    while(index <= current_size && index * 2 <= current_size) {
        int former = index;//initial value of index when it just got into loop

        findMostUrgent(index);//change the index to the next child index

        if(compare(list[former], list[index])){//check if original element is greater than next chlid
            swap(list[former], list[index]);//swap those values
        } else {
            break;//if not breaks the loops
        }
    }
}

void PatientQueue::enqueue(string value, int priority) {
    current_size++;//add one to the current size

    if(current_size >= array_size){//check if we have more element than the array can contains
        doubleList();//double the list size
    }
    list[current_size] = Patient(value, priority);//put the new value in the back
    int swapIndex = current_size;//the index will change as the element swap with other elements
    enqueneHelper(swapIndex);
}

void PatientQueue::enqueneHelper(int& index) {
    while(index > 1) {
        int parentIndex = index / 2;//get the parent index of the pass in index

        //check if the parent is greater than former element
        if(compare(list[parentIndex] , list[index])){
            swap(list[parentIndex],list[index]);//if it is true swap the element
            index = parentIndex;//update the index
        } else {
            break;//else breaks the loop
        }
    }
}

bool PatientQueue::isEmpty() const {
    return current_size == 0;
}

string PatientQueue::peek() const {
    return list[1].name;
}

int PatientQueue::peekPriority() const {
    return list[1].priority;
}

int PatientQueue::size() const {
    return current_size;
}

ostream& operator <<(ostream& out, const PatientQueue& queue) {
    out << "{";
    for(int i = 1; i <= queue.size(); i++) {
        if(i != queue.size()) {
            out << queue.list[i] << ", ";
        }else {
            out << queue.list[i];
        }
    }

    return out << "}";
}

void PatientQueue::doubleList(){
    Patient* bigger = new Patient[2 * array_size];

    //fill the bigger array with existed elements
    for (unsigned int i = 1; i < array_size; i++) {
        bigger[i] = list[i];
    }

    delete[] list;
    list = bigger;
    array_size *= 2;
}

void PatientQueue::swap(Patient& first, Patient& second){
    Patient tem;
    tem.name = first.name;
    tem.priority = first.priority;

    first.name = second.name;
    first.priority = second.priority;

    second.name = tem.name;
    second.priority = tem.priority;
}

void PatientQueue::findMostUrgent(int& curr) {
    Patient child1 = list[curr * 2];
    if(curr * 2 + 1 > current_size){//check if the index of second child is in range
        curr = curr * 2;//if not we just update to the first child index
    }else{
        //else we get the index of child 2 and compare them
        Patient child2 = list[curr * 2 + 1];

        if(compare(child2, child1)) {
            curr = curr * 2;
        }else {
            curr = curr * 2 + 1;
        }
    }
}

bool PatientQueue::compare(Patient first, Patient second) {
    if(isMin) {
        return first > second;
    } else {
        return first < second;
    }
}

