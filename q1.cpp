/*
There are number of students. For every student roll (unique), name is to be
stored. For each subject, subject code and name is to be stored. A student can
opt for number of subjects. System should be able to maintain student list,
subject list and will be able to answer: i) which student has selected which
subjects and ii) for a subjects who are the students.
Design the classes and implement. For list consider memory data structure.
*/


#include <iostream>
#include <vector>
#include <map>
#include <string>

class Subject;

class Student {
private:
    int roll;
    std::string name;
    std::vector<Subject*> enrolledSubjects;

public:
    Student(int roll, const std::string& name) : roll(roll), name(name) {}

    int getRoll() const { return roll; }
    std::string getName() const { return name; }

    void enrollSubject(Subject* subject);
    const std::vector<Subject*>& getEnrolledSubjects() const {
        return enrolledSubjects;
    }
};

class Subject {
private:
    int code;
    std::string name;
    std::vector<Student*> enrolledStudents;

public:
    Subject(int code, const std::string& name) : code(code), name(name) {}

    int getCode() const { return code; }
    std::string getName() const { return name; }

    void addStudent(Student* student) {
        enrolledStudents.push_back(student);
    }

    const std::vector<Student*>& getEnrolledStudents() const {
        return enrolledStudents;
    }
};

void Student::enrollSubject(Subject* subject) {
    enrolledSubjects.push_back(subject);
    subject->addStudent(this);
}

class StudentList {
private:
    std::map<int, Student*> students;

public:
    ~StudentList() {
        for (std::map<int, Student*>::iterator it = students.begin(); it != students.end(); ++it) {
            delete it->second;
        }
    }

    void addStudent(int roll, const std::string& name) {
        if (students.find(roll) != students.end()) {
            std::cout << "Error: A student with roll number " << roll << " already exists.\n";
            return;
        }
        students[roll] = new Student(roll, name);
    }

    Student* findStudentByRoll(int roll) {
        std::map<int, Student*>::iterator it = students.find(roll);
        return it != students.end() ? it->second : NULL;
    }

    void listAllStudents() const {
        for (std::map<int, Student*>::const_iterator it = students.begin(); it != students.end(); ++it) {
            std::cout << "Roll: " << it->first << ", Name: " << it->second->getName() << "\n";
        }
    }

    const std::map<int, Student*>& getStudents() const { return students; }
};

class SubjectList {
private:
    std::map<int, Subject*> subjects;

public:
    ~SubjectList() {
        for (std::map<int, Subject*>::iterator it = subjects.begin(); it != subjects.end(); ++it) {
            delete it->second;
        }
    }

    void addSubject(int code, const std::string& name) {
        if (subjects.find(code) != subjects.end()) {
            std::cout << "Error: A subject with code " << code << " already exists.\n";
            return;
        }
        subjects[code] = new Subject(code, name);
    }

    Subject* findSubjectByCode(int code) {
        std::map<int, Subject*>::iterator it = subjects.find(code);
        return it != subjects.end() ? it->second : NULL;
    }

    void listAllSubjects() const {
        for (std::map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
            std::cout << "Code: " << it->first << ", Name: " << it->second->getName() << "\n";
        }
    }

    const std::map<int, Subject*>& getSubjects() const { return subjects; }
};

int main() {
    StudentList studentList;
    SubjectList subjectList;

    studentList.addStudent(1, "Alice");
    studentList.addStudent(2, "Bob");
    studentList.addStudent(1, "DuplicateAlice");  // Duplicate roll check

    subjectList.addSubject(101, "Mathematics");
    subjectList.addSubject(102, "Physics");
    subjectList.addSubject(101, "DuplicateMath");  // Duplicate subject code check

    Student* alice = studentList.findStudentByRoll(1);
    Student* bob = studentList.findStudentByRoll(2);
    Subject* math = subjectList.findSubjectByCode(101);
    Subject* physics = subjectList.findSubjectByCode(102);

    if (alice && math) alice->enrollSubject(math);
    if (bob && physics) bob->enrollSubject(physics);
    if (bob && math) bob->enrollSubject(math);

    std::cout << "\nStudent -> Subjects:\n";
    const std::map<int, Student*>& students = studentList.getStudents();
    for (std::map<int, Student*>::const_iterator it = students.begin(); it != students.end(); ++it) {
        Student* student = it->second;
        std::cout << "Student: " << student->getName() << " (Roll: " << student->getRoll() << ")\nSubjects:\n";
        const std::vector<Subject*>& subjects = student->getEnrolledSubjects();
        for (std::vector<Subject*>::const_iterator sit = subjects.begin(); sit != subjects.end(); ++sit) {
            std::cout << "  - " << (*sit)->getName() << "\n";
        }
    }

    std::cout << "\nSubject -> Students:\n";
    const std::map<int, Subject*>& subjects = subjectList.getSubjects();
    for (std::map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
        Subject* subject = it->second;
        std::cout << "Subject: " << subject->getName() << " (Code: " << subject->getCode() << ")\nStudents:\n";
        const std::vector<Student*>& students = subject->getEnrolledStudents();
        for (std::vector<Student*>::const_iterator sit = students.begin(); sit != students.end(); ++sit) {
            std::cout << "  - " << (*sit)->getName() << "\n";
        }
    }

    return 0;
}
