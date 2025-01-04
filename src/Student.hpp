#pragma once

class Student
{
private:
    unsigned id; //unikalen id nomer
    unsigned examDuration; // kolko vreme mu e otnel testa
    unsigned course; // kurs na studenta

    unsigned arrivalTime; //moje da chaka na opashkata

    unsigned exitTime;

public:
    Student() = default; // trqbva ni zaredi default ctor vuv MyVector;
    Student(unsigned _id, unsigned _examDuration, unsigned _course, unsigned _arrivalTime, unsigned _exitTime) : id(_id), 
    examDuration(_examDuration),course(_course), arrivalTime(_arrivalTime), exitTime(_exitTime){};

    //moje i da gi zabranim, no v sluchaq default vurshi rabota, tui kato znaem
    // che ne rabotim s dynamic memory, koeto oznachava che prosto prisvoqvame vseki membur ot ediniq na drugiq
    Student(const Student& other) = default;
    Student& operator=(const Student& other) = default;

    //getters
    unsigned getId() const{
        return id;
    }

    unsigned getExamDuration() const{
        return examDuration;
    }

    unsigned getCourse() const {
        return course;
    }
    
    unsigned getArrivalTime() const {
        return arrivalTime;
    }

    unsigned getExitTime() const {
        return exitTime;
    }

    // nqmame nikakvi dynamic allocations
    ~Student() = default;

    void setExitTime(unsigned val){
        exitTime = val;
    }

    bool isFinished(unsigned currentTime) const 
    {
        return currentTime >= exitTime;
    }
};
