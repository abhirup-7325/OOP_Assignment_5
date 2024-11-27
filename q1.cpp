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

class System {
public:
    static void run() {
        StudentList studentList;
        SubjectList subjectList;

        while (true) {
            std::cout << "\nMenu:\n";
            std::cout << "1. Add Student\n";
            std::cout << "2. Add Subject\n";
            std::cout << "3. Enroll Student in Subject\n";
            std::cout << "4. List Students and their Subjects\n";
            std::cout << "5. List Subjects and their Students\n";
            std::cout << "6. Exit\n";
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    int roll;
                    std::string name;
                    std::cout << "Enter student roll number: ";
                    std::cin >> roll;
                    std::cin.ignore();  // Ignore newline character from previous input
                    std::cout << "Enter student name: ";
                    std::getline(std::cin, name);
                    studentList.addStudent(roll, name);
                    break;
                }
                case 2: {
                    int code;
                    std::string name;
                    std::cout << "Enter subject code: ";
                    std::cin >> code;
                    std::cin.ignore();  // Ignore newline character from previous input
                    std::cout << "Enter subject name: ";
                    std::getline(std::cin, name);
                    subjectList.addSubject(code, name);
                    break;
                }
                case 3: {
                    int roll, code;
                    std::cout << "Enter student roll number: ";
                    std::cin >> roll;
                    std::cout << "Enter subject code: ";
                    std::cin >> code;

                    Student* student = studentList.findStudentByRoll(roll);
                    Subject* subject = subjectList.findSubjectByCode(code);

                    if (student && subject) {
                        student->enrollSubject(subject);
                        std::cout << "Enrolled successfully.\n";
                    } else {
                        std::cout << "Error: Invalid student or subject.\n";
                    }
                    break;
                }
                case 4: {
                    const std::map<int, Student*>& students = studentList.getStudents();
                    std::cout << "\nStudent -> Subjects:\n";
                    for (std::map<int, Student*>::const_iterator it = students.begin(); it != students.end(); ++it) {
                        Student* student = it->second;
                        std::cout << "Student: " << student->getName() << " (Roll: " << student->getRoll() << ")\nSubjects:\n";
                        const std::vector<Subject*>& subjects = student->getEnrolledSubjects();
                        for (std::vector<Subject*>::const_iterator sit = subjects.begin(); sit != subjects.end(); ++sit) {
                            std::cout << "  - " << (*sit)->getName() << "\n";
                        }
                    }
                    break;
                }
                case 5: {
                    const std::map<int, Subject*>& subjects = subjectList.getSubjects();
                    std::cout << "\nSubject -> Students:\n";
                    for (std::map<int, Subject*>::const_iterator it = subjects.begin(); it != subjects.end(); ++it) {
                        Subject* subject = it->second;
                        std::cout << "Subject: " << subject->getName() << " (Code: " << subject->getCode() << ")\nStudents:\n";
                        const std::vector<Student*>& students = subject->getEnrolledStudents();
                        for (std::vector<Student*>::const_iterator sit = students.begin(); sit != students.end(); ++sit) {
                            std::cout << "  - " << (*sit)->getName() << "\n";
                        }
                    }
                    break;
                }
                case 6: {
                    std::cout << "Exiting program.\n";
                    return;
                }
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main() {
    System::run();
    return 0;
}
