#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../src/MyVector.hpp"
#include "../src/Student.hpp"

// Тест за конструкторите
TEST_CASE("MyVector - Default constructor creates empty vector") {
    MyVector<int> vec;
    REQUIRE(vec.getSize() == 0);
    REQUIRE(vec.getCapacity() == 1);
    REQUIRE(vec.isEmpty() == true);
}

// Тест за push_back
TEST_CASE("MyVector - push_back adds elements") {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);

    REQUIRE(vec.getSize() == 2);
    REQUIRE(vec.getCapacity() >= 2);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
}

// Тест за push_front
TEST_CASE("MyVector - push_front adds elements at the beginning") {
    MyVector<int> vec;
    vec.push_front(30);
    vec.push_front(40);

    REQUIRE(vec.getSize() == 2);
    REQUIRE(vec[0] == 40);
    REQUIRE(vec[1] == 30);
}

// Тест за pop_back
TEST_CASE("MyVector - pop_back removes elements from the end") {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.pop_back();

    REQUIRE(vec.getSize() == 1);
    REQUIRE(vec[0] == 10);
}

// Тест за pop_front
TEST_CASE("MyVector - pop_front removes elements from the beginning") {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.pop_front();

    REQUIRE(vec.getSize() == 1);
    REQUIRE(vec[0] == 20);
}

// Тест за resize
TEST_CASE("MyVector - resizing doubles capacity when full") {
    MyVector<int> vec;
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }

    REQUIRE(vec.getSize() == 10);
    REQUIRE(vec.getCapacity() >= 10);
    REQUIRE(vec[9] == 9);
}

// Тест за оператор []
TEST_CASE("MyVector - operator[] gives access to elements") {
    MyVector<int> vec;
    vec.push_back(100);
    vec.push_back(200);

    REQUIRE(vec[0] == 100);
    REQUIRE(vec[1] == 200);
}

// Тест за insert
TEST_CASE("MyVector - insert adds element at specified index") {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(30);
    vec.insert(1, 20);

    REQUIRE(vec.getSize() == 3);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
    REQUIRE(vec[2] == 30);
}

// Тест за erase
TEST_CASE("MyVector - erase removes element at specified index") {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.erase(1);

    REQUIRE(vec.getSize() == 2);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 30);
}

// Тест за shrink_to_fit
TEST_CASE("MyVector - shrink_to_fit adjusts capacity to size") {
    MyVector<int> vec;
    for (int i = 0; i < 5; ++i) {
        vec.push_back(i);
    }
    vec.pop_back();
    vec.shrink_to_fit();

    REQUIRE(vec.getSize() == 4);
    REQUIRE(vec.getCapacity() == 4);
}

// Тест за копиращ конструктор
TEST_CASE("MyVector - copy constructor creates independent copy") {
    MyVector<int> vec1;
    vec1.push_back(10);
    vec1.push_back(20);

    MyVector<int> vec2 = vec1;
    vec2.push_back(30);

    REQUIRE(vec1.getSize() == 2);
    REQUIRE(vec2.getSize() == 3);
}

// Тест за оператор =
TEST_CASE("MyVector - assignment operator works correctly") {
    MyVector<int> vec1;
    vec1.push_back(10);
    vec1.push_back(20);

    MyVector<int> vec2;
    vec2 = vec1;

    REQUIRE(vec2.getSize() == 2);
    REQUIRE(vec2[0] == 10);
    REQUIRE(vec2[1] == 20);
}


TEST_CASE("Student - Constructor initializes correctly") {
    Student student(1, 90, 3, 1000, 1090);

    REQUIRE(student.getId() == 1);
    REQUIRE(student.getExamDuration() == 90);
    REQUIRE(student.getCourse() == 3);
    REQUIRE(student.getArrivalTime() == 1000);
    REQUIRE(student.getExitTime() == 1090);
}

// Тест за метода isFinished
TEST_CASE("Student - isFinished returns correct status") {
    Student student(1, 90, 3, 1000, 1090);

    SECTION("Test when exam is not finished") {
        REQUIRE(student.isFinished(1089) == false);
    }

    SECTION("Test when exam is finished") {
        REQUIRE(student.isFinished(1090) == true);
        REQUIRE(student.isFinished(1200) == true);
    }
}

// Тест за копиращ конструктор
TEST_CASE("Student - Copy constructor creates correct copy") {
    Student original(2, 120, 4, 1500, 1620);
    Student copy = original;

    REQUIRE(copy.getId() == 2);
    REQUIRE(copy.getExamDuration() == 120);
    REQUIRE(copy.getCourse() == 4);
    REQUIRE(copy.getArrivalTime() == 1500);
    REQUIRE(copy.getExitTime() == 1620);
}

// Тест за оператор =
TEST_CASE("Student - Assignment operator works correctly") {
    Student student1(3, 60, 2, 900, 960);
    Student student2;

    student2 = student1;

    REQUIRE(student2.getId() == 3);
    REQUIRE(student2.getExamDuration() == 60);
    REQUIRE(student2.getCourse() == 2);
    REQUIRE(student2.getArrivalTime() == 900);
    REQUIRE(student2.getExitTime() == 960);
}

// Тест за гетърите
TEST_CASE("Student - Getters return correct values") {
    Student student(4, 45, 1, 800, 845);

    REQUIRE(student.getId() == 4);
    REQUIRE(student.getExamDuration() == 45);
    REQUIRE(student.getCourse() == 1);
    REQUIRE(student.getArrivalTime() == 800);
    REQUIRE(student.getExitTime() == 845);
}


