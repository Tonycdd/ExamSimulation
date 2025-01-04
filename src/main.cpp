#include <iostream>
#include "MyVector.hpp"
#include "Student.hpp"
#include <fstream>
#include <sstream>
#include <deque> //govorili sme za nego
#include <stack>
#include <algorithm> 
#include <climits>


bool isValid(const int& row, const int& col, const int& pf, const int& tf){
    if(row <= 0 || col <= 0 ){
        std::cerr << "Rows nor cols can be zero or negative!" << std::endl;
        return false;
    }
    else if(row*col > 50){
        std::cerr << "Only 50 available seats in this hall!" << std::endl;
        return false;
    }
    else if(pf < 0){
        std::cerr << "Minute of arriving professor cannot be negative" << std::endl;
        return false;
    }
    else if(tf < 0){
        std::cerr<<"Time for checking tests cannot be negative" << std::endl;
        return false;
    }
    return true;
}

bool isValidPosition(const MyVector<MyVector<int>>& room, int row, int col, int N, int M) {
    if (row < 0 || row >= N || col < 0 || col >= M) 
        return false;
    
    if (room[row][col] == -1)
        return false;

    int rows[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int cols[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < 8; i++) {
        int newRow = row + rows[i];
        int newCol = col + cols[i];
        
        if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < M) {
            if (room[newRow][newCol] == 1) {
                return false;
            }
        }
    }
    return true;
}

void arrangeStudents(MyVector<MyVector<int>>& room, int row, int col, int currentStudents, 
                    int& maxStudents, MyVector<MyVector<int>>& bestArrangement, int N, int M) {
    if (row >= N) {
        if (currentStudents > maxStudents) {
            maxStudents = currentStudents;
            bestArrangement = room;
        }
        return;
    }
    
    if (col >= M) {
        arrangeStudents(room, row + 1, 0, currentStudents, maxStudents, bestArrangement, N, M);
        return;
    }
    
    if (room[row][col] == -1) {
        arrangeStudents(room, row, col + 1, currentStudents, maxStudents, bestArrangement, N, M);
        return;
    }
    
    if (isValidPosition(room, row, col, N, M)) {
        room[row][col] = 1;
        arrangeStudents(room, row, col + 1, currentStudents + 1, maxStudents, bestArrangement, N, M);
        room[row][col] = 0;
    }

    arrangeStudents(room, row, col + 1, currentStudents, maxStudents, bestArrangement, N, M);
}

void createRoom(std::ifstream& file, int& professorArriving, int& timeForTests, int& maxStudents) {
    std::string currentLine;
    std::getline(file, currentLine);
    std::istringstream ss(currentLine);

    int row, col;
    ss >> row >> col >> professorArriving >> timeForTests;
    
    if(!isValid(row, col, professorArriving, timeForTests))
        return;
    
    std::getline(file, currentLine);
    std::istringstream s2(currentLine);
    int brokenChairs;
    s2 >> brokenChairs;
    
    if(brokenChairs < 0 || brokenChairs > row * col) {
        std::cerr << "Invalid number of broken chairs!" << std::endl;
        return;
    }

    MyVector<MyVector<int>> room;
    for (int i = 0; i < row; ++i) {
        MyVector<int> currentRow;
        for (int j = 0; j < col; ++j) {
            currentRow.push_back(0);
        }
        room.push_back(currentRow);
    }

    std::getline(file, currentLine);
    std::istringstream s3(currentLine);
    int currentIndex;
    while(s3 >> currentIndex) {
        if(currentIndex < 0 || currentIndex >= row * col) {
            std::cerr << "Invalid broken chair index!" << std::endl;
            return;
        }
        int brokenRow = currentIndex / col;
        int brokenCol = currentIndex % col;
        room[brokenRow][brokenCol] = -1;
    }

    MyVector<MyVector<int>> bestArrangement = room;
    maxStudents = 0;
    arrangeStudents(room, 0, 0, 0, maxStudents, bestArrangement, row, col);

    // Print room layout
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if(bestArrangement[i][j] == 1)
                std::cout << "O";
            else if(bestArrangement[i][j] == 0)
                std::cout << "F";
            else
                std::cout << "B";
            if (j < col - 1) std::cout << "";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool valid(const int& ar, const int& id, const int& d, const int& c){
    //qsno e kakvo proverqvame - info za student
    return (ar >= 0 && id > 0 && id <= 256 && d > 0 && c >= 2 && c <= 4);
}

void addStudentInQueue(std::deque<Student>& queue, const Student& newStudent) {
    // ako e prazna q dobavqme v kraq
    if (queue.empty()) {
        queue.push_back(newStudent);
        return;
    }

    // tursim pravilnata poziciq
    auto it = queue.begin();
    while (it != queue.end()) {
        // ot vtori kurs imat predimstvo
        if (newStudent.getCourse() == 2 && it->getCourse() != 2) {
            break;
        }
        // ako sa ot edno i sushto po vreme na pristigane
        if (newStudent.getCourse() == it->getCourse() && 
            newStudent.getArrivalTime() < it->getArrivalTime()) {
            break;
        }
        // ako noviqt ne e ot 2, no segashniqt e
        if (newStudent.getCourse() != 2 && it->getCourse() == 2) {
            ++it;
            continue;
        }
        // inache vinagi sortirame po vreme na pristigane
        if (newStudent.getCourse() == it->getCourse() || 
            (newStudent.getCourse() != 2 && it->getCourse() != 2)) {
            if (newStudent.getArrivalTime() < it->getArrivalTime()) {
                break;
            }
        }
        ++it;
    }
    
    queue.insert(it, newStudent);
}

void addStudent(MyVector<Student>& hall, Student& st) {
    //tuk poddurjame  vektora ni sortiran, za burzo namirane na sledvashto subitie, ima i po-burz nachin na tursene v 'pochti' sortiran vektor, 
    // no tova vurshi rabota za nashiq primer i v sluchaq;
    for (int i = 0; i < hall.getSize(); i++) {
        if(st.getExitTime() < hall[i].getExitTime()) {
            hall.insert(i, st);
            return;
        }
    }
    hall.push_back(st);
}

void processTeacherChecking(int& lastCheck, int& currentTime, int& neededTime, 
                          std::stack<int>& toCheck, std::stack<int>& done, int& f) {
    //proverqvame samo ako imame izpiti na masata, kakto i trqbva da sledim slednoto:
    // trqbva da znaem koga e purvviqt put v koito profesorut proverqva, ako kogato pristigne
    // v moment 23, no vse oshte nqma izpiti na masata, to trqbva pri purviq slojen izpit da vzeme tozi izpit i da proverqva nego,
    // tozi moment ni trqbva,za da izchislqvame pravilno koi shte bude sledvasht izpit koito shte vzeme;

    if(!toCheck.empty() && (f == 1 || currentTime >= lastCheck + neededTime)) {
        int examToCheck = toCheck.top(); // vzimame tozi na vurxa
        toCheck.pop(); // i go maxame
        done.push(examToCheck); //slagame v proverenite
        
        // tuk aktualizirame nasheto vreme
        if(f == 1) {
            currentTime = std::max(currentTime, lastCheck);
            lastCheck = currentTime;
            f = 2; // glupav nachin, no zatova se setix purvo, realno e otnovo za korektno sledene na nachalnoto vreme za proverka
        } else {
            // za vseki sluchai da se podsigurim da aktualizirame vremeto
            lastCheck = std::max(currentTime, lastCheck + neededTime);
            currentTime = lastCheck;
        }
        
       // std::cout << "Professor checked exam: " << examToCheck 
        //         << " at time " << currentTime << std::endl;
    }
}

void processEndedExams(MyVector<Student>& studentsInHall, std::stack<int>& toCheck, 
                      int& currentTime, int& maxStudents, int& lastCh,int& flag) {
  // za da sledim dali e purviqt imame nqkakuv flag, koito proverqvame vednaga, ne kostva nishto
    for (int i = 0; i < studentsInHall.getSize(); i++) {

        if (studentsInHall[i].getExitTime() <= currentTime) {
           // std::cout << "Student: " << studentsInHall[i].getId() 
              //       << " finished exam at time: " << studentsInHall[i].getExitTime() << std::endl;
            toCheck.push(studentsInHall[i].getId());
            ++flag;
            studentsInHall.erase(i);
            maxStudents++;
            i--;
        }
    }
    // pak flagut e malko glupavo. no tova se setix purvonachalno, tuk ideqta e da vidim dali tova mu e purva proverka, za da mojem da 
    // sledim korektno purvoto proverqvane
    if(flag == 1){
        lastCh = currentTime;
    }
}

//kato cqlo iskax da razdelq turseneto na sledvashti subitiq kogato processvame enter i end, kogato e end ne vzimame predvid vremeto na vlizane,tui kato
// veche shte sme obrabotili tozi sluchai v enter
//kato logika sa sxodni
void findNextEventTime(int& currentTime, const MyVector<Student>& studentsInHall,
                      const std::deque<Student>& queue, int professorArrival,
                      int lastCheck, int neededTime, bool hasUnfinishedTests,const std::stack<int>& toCheck) {
    int nextTime = INT_MAX;

    if(studentsInHall.getSize() != 0){
        nextTime = std::min(nextTime, (int)studentsInHall[0].getExitTime());
    }    
    
    //samo ako imame testove i profesorut e veche doshul;
    if (currentTime >= professorArrival && hasUnfinishedTests) {
        int nextCheckTime = lastCheck + neededTime;
        nextTime = std::min(nextTime, nextCheckTime);
    }

    //da vidim dali ima sledvashto subitie
    if (nextTime != INT_MAX) {
        currentTime = nextTime;
        //std::cout << "Time advanced to: " << currentTime << std::endl;
    }
    return;
}                    

//sxodna sxema, no ideqta e razlichna, toest kontekstut v koito q izvikvame
void findSignificantEvents(int& targetTime, int& currentTime, 
                          MyVector<Student>& studentsInHall,
                          std::deque<Student>& queue, 
                          int& professorArrival, 
                          int& lastCheck,
                          int& neededTime,
                          std::stack<int>& toCheck,
                          std::stack<int>& done,
                          int& maxStudents,
                          int& f,bool& flag) {
    // iskame vsichki subitiq do target da se izpulnqt predi target
    while (currentTime < targetTime) {
        int nextEventTime = INT_MAX;
        
       //zatova vektora ni beshe sortiran,za da ne vikame std::min za vseki element na nego
        if(studentsInHall.getSize() != 0){
            nextEventTime = std::min(nextEventTime,(int) studentsInHall[0].getExitTime());
        }

        //dali e doshul profesorut
        if (currentTime < professorArrival && professorArrival < targetTime) {
            nextEventTime = std::min(nextEventTime, professorArrival);
        }
        
        // sledvashta proverka ot nego
        if (currentTime >= professorArrival && !toCheck.empty()) {
            int nextCheckTime = lastCheck + neededTime;
            if (nextCheckTime < targetTime) {
                nextEventTime = std::min(nextEventTime, nextCheckTime);
            }
        }
        
        //  ako nqma predi target nishto
        if (nextEventTime == INT_MAX || nextEventTime >= targetTime) {
            break;
        }
        
        //inache predvijvame i obrabotvame v tekushtiq moment
        currentTime = nextEventTime;
        
        //obrabotvame za tekushtiq moment
        processEndedExams(studentsInHall, toCheck, currentTime, maxStudents, lastCheck, f);
        
        if (currentTime >= professorArrival) {
            processTeacherChecking(lastCheck, currentTime, neededTime, toCheck, done,f);
        }
        
        //ako ima svoboni mesta dobavqme ot opashkata
        while (!queue.empty() && maxStudents > 0 && 
               queue.front().getArrivalTime() <= currentTime) {
            Student nextStudent = queue.front();
            queue.pop_front();

            //pak pravim tova inache nqma da ni izleznat vremenata pravilno za izlizane
            nextStudent.setExitTime(currentTime + nextStudent.getExamDuration());

            addStudent(studentsInHall, nextStudent);
           // std::cout << "Student " << nextStudent.getId() 
                 //    << " from queue entered hall at time " << currentTime << std::endl;
            maxStudents--;
        }
    }
}

//kogato chetem vseki nov red do end
void processEnter(std::istringstream& ss, int& currentTime, 
                 int professorArrival, int& neededTime, int& lastCheck,
                 int& maxStudents, MyVector<Student>& studentsInHall,
                 std::deque<Student>& queue, std::stack<int>& toCheck,
                 std::stack<int>& done, int& f,bool& flag3) {
    
    std::string command;
    int arrivalTime, id, duration, course;
    ss >> command >> arrivalTime >> id >> duration >> course;
    
    if (!valid(arrivalTime, id, duration, course)) {
        std::cerr << "Invalid student data!" << std::endl;
        return;
    }
    
    // da namerim vsichki znachimi subitiq predin nasheto
    findSignificantEvents(arrivalTime, currentTime, studentsInHall, queue, 
                         professorArrival, lastCheck, neededTime, toCheck, 
                         done, maxStudents, f,flag3);
    
    // aktualizirame vremeto
    currentTime = arrivalTime;
    
    /*
    std::cout << std::endl;
    std::cout << "Current time: " << currentTime << std::endl;
    std::cout << "Students in hall: " << studentsInHall.getSize() << std::endl;
    std::cout << "Queue size: " << queue.size() << std::endl;
    std::cout << "Tests to check: " << toCheck.size() << std::endl;
    std::cout << std::endl;   
    */
    
    // Obrabotka na noviq student
    Student newStudent(id, duration, course, arrivalTime, arrivalTime + duration);
    
    //flag3 e malko tupo napisano, obache ideqta my e kogato go zapulnim za purvi put samata zala
    // togava iskame vsichko da mine purvo prez opashkata, sled tova da doide v zalata
    if (maxStudents > 0 && !flag3) {
        addStudent(studentsInHall, newStudent);
        //std::cout << "Student " << id << " entered hall at time " << currentTime << std::endl;
        maxStudents--;
        if (maxStudents == 0) flag3 = true;
    } else {
        addStudentInQueue(queue, newStudent);
        //std::cout << "Student " << id << " added to queue at time " << currentTime << std::endl;
    }
    
    // ako sme napulnili za purvi put zalata
    while (!queue.empty() && maxStudents > 0 && flag3) {
        if (queue.front().getArrivalTime() <= currentTime) {
            Student& nextStudent = queue.front();
            queue.pop_front();
            
            //tuk tova beshe greshkata,, debugval sum go cql den za da izleze
            //realno prednoto dobaveno vreme moje da e grehsno tui kato toi shte vleze v razlichna minuta, i taka moje da izleze
            // v nqkoe greshno vreme otkudeto shte se razburkat i nashite vremena
            nextStudent.setExitTime(currentTime + nextStudent.getExamDuration());
            addStudent(studentsInHall, nextStudent);
            
            //std::cout << "Student " << nextStudent.getId() 
              //       << " from queue entered hall at time " << currentTime << std::endl;
            maxStudents--;
        } else {
            break;
        }
    }
}

//analogichno vmesto end, mojeshe i da e end of file, no v primera e s end
void processEnd(int& currentTime, MyVector<Student>& studentsInHall,
               std::deque<Student>& queue, int& maxStudents,
               int professorArrival, int& neededTime, int& lastCheck,
               std::stack<int>& toCheck, std::stack<int>& done,int& f,bool& flag3) {
    //da otpechatame podredbata za da vidim
    /*
    for (size_t i = 0; i < studentsInHall.getSize(); i++) {
        std::cout << "Position " << i << ": "<<studentsInHall[i].getId() << "  ";
    }
    
    std::cout << std::endl;
    for (const auto& element : queue) {
        std::cout << element.getId() << " ";
    }
    */

    while (!studentsInHall.isEmpty() || !queue.empty() || !toCheck.empty()) {
        findNextEventTime(currentTime, studentsInHall, queue, professorArrival,
                         lastCheck, neededTime, !toCheck.empty(),toCheck);

        processEndedExams(studentsInHall, toCheck, currentTime, maxStudents,lastCheck,f);
       
        if (currentTime >= professorArrival) {
            processTeacherChecking(lastCheck, currentTime, neededTime, toCheck, done,f);
        }
        
        // Fix: Obnovqvame exitTime spored currentTime
        while (!queue.empty() && maxStudents > 0 && 
               queue.front().getArrivalTime() <= currentTime) {
            Student& nextStudent = queue.front();
            queue.pop_front();
            // Fix: Izpolzvame currentTime pri izchislenie na exitTime, pravim go i tuk inache shte se razburkat vremenata za izlizane!
            nextStudent.setExitTime(currentTime + nextStudent.getExamDuration());
            addStudent(studentsInHall, nextStudent);
            maxStudents--;
            //std::cout << "Student " << nextStudent.getId() << " enter after command end to hall at time: " << currentTime << std::endl;
        }
    }
}

void printResults(std::stack<int>& done, int& currentTime, int& timeForTest) {
    while (!done.empty()) {
        std::cout << done.top() << std::endl;
        done.pop();
    }
    
    // tova e tui kato ne vzimame predid vremeto za proverka ot posledniq vzet izpit - t.e smqtame che kato  vzeme vednaga go slaga v kupchinata s provereni,
    // no v 13:12 toi go vzima i go slaga,no chak sled 15 moje da prikluchi izpita, tui kato ima 15 minuti za proverka 12+15=13:27;
    
    currentTime += timeForTest;
    int hours = 8 + currentTime / 60;
    int minutes = currentTime % 60;
    std::cout << "Finish time: " << hours << ":" 
              << (minutes < 10 ? "0" : "") << minutes << std::endl;
    std::cout << "—------------------------------" << std::endl;
}

int main() {
    try
    {
        
        std::ifstream file("input.txt");
        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return -1;
        }
    
        int f = 0;
        int professorArriving, timeForTest, maxStudents;
        MyVector<Student> studentsInHall;
        std::deque<Student> queue; // dequeue e pozvoleno  da se polzva,govorili sme go na uprajneniq
        std::stack<int> toCheck;
        std::stack<int> done;

        createRoom(file, professorArriving, timeForTest, maxStudents);

        int currentTime = 0;
        int lastCheck = professorArriving;
        bool flag = false;
        
        std::string command;
        while (std::getline(file,command)) {
            if (command.empty()) {
                continue;
            }
        
            else if (command == "end") {
                processEnd(currentTime, studentsInHall, queue, maxStudents,
                        professorArriving, timeForTest, lastCheck, toCheck, done,f,flag);
                printResults(done, currentTime, timeForTest);
                break;
            }
            else if (command.substr(0, 5) == "enter") {
                std::istringstream ss(command);
            
                processEnter(ss,currentTime, professorArriving, timeForTest, lastCheck,
                            maxStudents, studentsInHall, queue, toCheck, done,f,flag);
            }
        }

        file.close();
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    
}