#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;

struct Subject {
    string name;
    int marks;
    int maxMarks;
};

struct Student {
    string rollNo;
    string name;
    vector<Subject> subjects;
    float gpa;
    string grade;
    
    // Calculate GPA and Grade
    void calculateGPAandGrade() {
        if (subjects.empty()) {
            gpa = 0.0;
            grade = "N/A";
            return;
        }
        
        int totalMarks = 0;
        int totalMaxMarks = 0;
        
        for (const auto &sub : subjects) {
            totalMarks += sub.marks;
            totalMaxMarks += sub.maxMarks;
        }
        
        float percentage = (float)totalMarks / totalMaxMarks * 100;
        
        // Convert percentage to GPA (10-point scale)
        gpa = percentage / 10.0;
        
        // Assign grade based on percentage
        if (percentage >= 90) grade = "A+";
        else if (percentage >= 80) grade = "A";
        else if (percentage >= 70) grade = "B+";
        else if (percentage >= 60) grade = "B";
        else if (percentage >= 50) grade = "C";
        else if (percentage >= 40) grade = "D";
        else grade = "F";
    }
    
    float getPercentage() const {
        if (subjects.empty()) return 0.0;
        
        int totalMarks = 0;
        int totalMaxMarks = 0;
        
        for (const auto &sub : subjects) {
            totalMarks += sub.marks;
            totalMaxMarks += sub.maxMarks;
        }
        
        return (float)totalMarks / totalMaxMarks * 100;
    }
};

vector<Student> students;

// Predefined subjects
const vector<string> SUBJECT_NAMES = {
    "Mathematics", "Physics", "Chemistry", "English", "Computer Science"
};
const int MAX_MARKS_PER_SUBJECT = 100;

bool compareStudents(const Student &a, const Student &b) {
    return a.rollNo < b.rollNo;
}

void saveData() {
    ofstream file("students.txt");
    for (auto &s : students) {
        file << s.rollNo << "|" << s.name << "|";
        
        // Save subjects
        for (size_t i = 0; i < s.subjects.size(); i++) {
            file << s.subjects[i].name << ":" 
                 << s.subjects[i].marks << ":" 
                 << s.subjects[i].maxMarks;
            if (i < s.subjects.size() - 1) file << ",";
        }
        
        file << "|" << s.gpa << "|" << s.grade << "\n";
    }
    file.close();
}

void loadData() {
    students.clear();
    ifstream file("students.txt");
    string line;

    while (getline(file, line)) {
        Student s;
        stringstream ss(line);
        string token;
        int fieldCount = 0;
        
        while (getline(ss, token, '|')) {
            if (fieldCount == 0) {
                s.rollNo = token;
            }
            else if (fieldCount == 1) {
                s.name = token;
            }
            else if (fieldCount == 2) {
                // Parse subjects
                stringstream subStream(token);
                string subToken;
                while (getline(subStream, subToken, ',')) {
                    Subject sub;
                    size_t pos1 = subToken.find(':');
                    size_t pos2 = subToken.rfind(':');
                    
                    if (pos1 != string::npos && pos2 != string::npos) {
                        sub.name = subToken.substr(0, pos1);
                        sub.marks = stoi(subToken.substr(pos1 + 1, pos2 - pos1 - 1));
                        sub.maxMarks = stoi(subToken.substr(pos2 + 1));
                        s.subjects.push_back(sub);
                    }
                }
            }
            else if (fieldCount == 3) {
                s.gpa = stof(token);
            }
            else if (fieldCount == 4) {
                s.grade = token;
            }
            fieldCount++;
        }
        
        students.push_back(s);
    }
    file.close();
    
    sort(students.begin(), students.end(), compareStudents);
}

int binarySearch(const string &roll) {
    int left = 0;
    int right = students.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (students[mid].rollNo == roll) {
            return mid;
        }
        else if (students[mid].rollNo < roll) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    
    return -1;
}

void displayStudents() {
    if (students.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    cout << "\n" << string(100, '=') << "\n";
    cout << "                          STUDENT RECORDS (Sorted by Roll No)\n";
    cout << string(100, '=') << "\n\n";
    
    for (const auto &s : students) {
        cout << "Roll No: " << s.rollNo << " | Name: " << s.name << "\n";
        cout << string(100, '-') << "\n";
        
        cout << left << setw(25) << "Subject" 
             << setw(15) << "Marks" 
             << setw(15) << "Max Marks" << "\n";
        cout << string(100, '-') << "\n";
        
        for (const auto &sub : s.subjects) {
            cout << left << setw(25) << sub.name
                 << setw(15) << sub.marks
                 << setw(15) << sub.maxMarks << "\n";
        }
        
        cout << string(100, '-') << "\n";
        cout << "Percentage: " << fixed << setprecision(2) << s.getPercentage() << "%"
             << " | GPA: " << s.gpa 
             << " | Grade: " << s.grade << "\n";
        cout << string(100, '=') << "\n\n";
    }
}

void addStudent() {
    Student s;
    cout << "\nEnter Roll No: ";
    cin >> s.rollNo;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, s.name);

    if (binarySearch(s.rollNo) != -1) {
        cout << "\nRoll number already exists!\n";
        return;
    }

    cout << "\n--- Enter marks for each subject (out of " << MAX_MARKS_PER_SUBJECT << ") ---\n";
    
    for (const auto &subName : SUBJECT_NAMES) {
        Subject sub;
        sub.name = subName;
        sub.maxMarks = MAX_MARKS_PER_SUBJECT;
        
        do {
            cout << subName << ": ";
            cin >> sub.marks;
            
            if (sub.marks < 0 || sub.marks > MAX_MARKS_PER_SUBJECT) {
                cout << "Invalid marks! Enter between 0 and " << MAX_MARKS_PER_SUBJECT << "\n";
            }
        } while (sub.marks < 0 || sub.marks > MAX_MARKS_PER_SUBJECT);
        
        s.subjects.push_back(sub);
    }
    
    s.calculateGPAandGrade();
    
    students.push_back(s);
    sort(students.begin(), students.end(), compareStudents);
    saveData();
    
    cout << "\n✓ Student added successfully!\n";
    cout << "GPA: " << fixed << setprecision(2) << s.gpa << " | Grade: " << s.grade << "\n";
}

void addMultipleStudents() {
    int numStudents;
    cout << "\nHow many students do you want to add? ";
    cin >> numStudents;
    
    if (numStudents <= 0) {
        cout << "\n✗ Invalid number!\n";
        return;
    }
    
    vector<Student> newStudents;
    int successCount = 0;
    int skipCount = 0;
    
    for (int i = 1; i <= numStudents; i++) {
        cout << "\n" << string(80, '=') << "\n";
        cout << "         STUDENT " << i << " OF " << numStudents << "\n";
        cout << string(80, '=') << "\n";
        
        Student s;
        cout << "Enter Roll No: ";
        cin >> s.rollNo;
        cin.ignore();
        
        // Check if roll number already exists
        if (binarySearch(s.rollNo) != -1) {
            cout << "✗ Roll number already exists! Skipping this entry.\n";
            skipCount++;
            continue;
        }
        
        // Check in current batch
        bool duplicateInBatch = false;
        for (const auto &ns : newStudents) {
            if (ns.rollNo == s.rollNo) {
                cout << "✗ Roll number already entered in this batch! Skipping.\n";
                skipCount++;
                duplicateInBatch = true;
                break;
            }
        }
        
        if (duplicateInBatch) continue;
        
        cout << "Enter Name: ";
        getline(cin, s.name);
        
        cout << "\n--- Enter marks for each subject (out of " << MAX_MARKS_PER_SUBJECT << ") ---\n";
        
        for (const auto &subName : SUBJECT_NAMES) {
            Subject sub;
            sub.name = subName;
            sub.maxMarks = MAX_MARKS_PER_SUBJECT;
            
            do {
                cout << subName << ": ";
                cin >> sub.marks;
                
                if (sub.marks < 0 || sub.marks > MAX_MARKS_PER_SUBJECT) {
                    cout << "Invalid marks! Enter between 0 and " << MAX_MARKS_PER_SUBJECT << "\n";
                }
            } while (sub.marks < 0 || sub.marks > MAX_MARKS_PER_SUBJECT);
            
            s.subjects.push_back(sub);
        }
        
        s.calculateGPAandGrade();
        newStudents.push_back(s);
        successCount++;
        
        cout << "\n✓ Student " << i << " recorded! GPA: " << fixed << setprecision(2) 
             << s.gpa << " | Grade: " << s.grade << "\n";
    }
    
    // Add all new students at once
    if (!newStudents.empty()) {
        for (auto &s : newStudents) {
            students.push_back(s);
        }
        sort(students.begin(), students.end(), compareStudents);
        saveData();
    }
    
    // Summary
    cout << "\n" << string(80, '=') << "\n";
    cout << "                   BATCH ADDITION SUMMARY\n";
    cout << string(80, '=') << "\n";
    cout << "✓ Successfully added: " << successCount << " students\n";
    if (skipCount > 0) {
        cout << "✗ Skipped (duplicates): " << skipCount << " students\n";
    }
    cout << "Total students in system: " << students.size() << "\n";
    cout << string(80, '=') << "\n";
}

void searchStudent() {
    string roll;
    cout << "\nEnter Roll No to search: ";
    cin >> roll;

    int index = binarySearch(roll);
    
    if (index != -1) {
        const Student &s = students[index];
        cout << "\n" << string(100, '=') << "\n";
        cout << "Roll No: " << s.rollNo << " | Name: " << s.name << "\n";
        cout << string(100, '-') << "\n";
        
        cout << left << setw(25) << "Subject" 
             << setw(15) << "Marks" 
             << setw(15) << "Max Marks" << "\n";
        cout << string(100, '-') << "\n";
        
        for (const auto &sub : s.subjects) {
            cout << left << setw(25) << sub.name
                 << setw(15) << sub.marks
                 << setw(15) << sub.maxMarks << "\n";
        }
        
        cout << string(100, '-') << "\n";
        cout << "Percentage: " << fixed << setprecision(2) << s.getPercentage() << "%"
             << " | GPA: " << s.gpa 
             << " | Grade: " << s.grade << "\n";
        cout << string(100, '=') << "\n";
    } else {
        cout << "\nStudent not found!\n";
    }
}

void deleteStudent() {
    string roll;
    cout << "\nEnter Roll No to delete: ";
    cin >> roll;

    int index = binarySearch(roll);
    
    if (index != -1) {
        students.erase(students.begin() + index);
        saveData();
        cout << "\n✓ Record deleted successfully!\n";
    } else {
        cout << "\nStudent not found!\n";
    }
}

void updateStudent() {
    string roll;
    cout << "\nEnter Roll No to update: ";
    cin >> roll;

    int index = binarySearch(roll);
    
    if (index != -1) {
        cout << "\n--- Update marks for each subject ---\n";
        
        for (auto &sub : students[index].subjects) {
            int newMarks;
            do {
                cout << sub.name << " (Current: " << sub.marks << "): ";
                cin >> newMarks;
                
                if (newMarks < 0 || newMarks > sub.maxMarks) {
                    cout << "Invalid marks! Enter between 0 and " << sub.maxMarks << "\n";
                }
            } while (newMarks < 0 || newMarks > sub.maxMarks);
            
            sub.marks = newMarks;
        }
        
        students[index].calculateGPAandGrade();
        saveData();
        
        cout << "\n✓ Marks updated successfully!\n";
        cout << "New GPA: " << fixed << setprecision(2) << students[index].gpa 
             << " | Grade: " << students[index].grade << "\n";
    } else {
        cout << "\nStudent not found!\n";
    }
}

void viewToppers() {
    if (students.empty()) {
        cout << "\nNo records found.\n";
        return;
    }
    
    vector<Student> sortedByGPA = students;
    sort(sortedByGPA.begin(), sortedByGPA.end(), 
         [](const Student &a, const Student &b) { return a.gpa > b.gpa; });
    
    cout << "\n" << string(100, '=') << "\n";
    cout << "                          TOP PERFORMERS\n";
    cout << string(100, '=') << "\n";
    
    cout << left << setw(15) << "Roll No" 
         << setw(25) << "Name" 
         << setw(15) << "GPA"
         << setw(10) << "Grade"
         << setw(15) << "Percentage" << "\n";
    cout << string(100, '-') << "\n";
    
    int count = min(10, (int)sortedByGPA.size());
    for (int i = 0; i < count; i++) {
        cout << left << setw(15) << sortedByGPA[i].rollNo
             << setw(25) << sortedByGPA[i].name
             << setw(15) << fixed << setprecision(2) << sortedByGPA[i].gpa
             << setw(10) << sortedByGPA[i].grade
             << setw(15) << sortedByGPA[i].getPercentage() << "%\n";
    }
    cout << string(100, '=') << "\n";
}

void classStatistics() {
    if (students.empty()) {
        cout << "\nNo records found.\n";
        return;
    }
    
    float totalGPA = 0;
    float totalPercentage = 0;
    map<string, int> gradeCount;
    
    for (const auto &s : students) {
        totalGPA += s.gpa;
        totalPercentage += s.getPercentage();
        gradeCount[s.grade]++;
    }
    
    cout << "\n" << string(100, '=') << "\n";
    cout << "                          CLASS STATISTICS\n";
    cout << string(100, '=') << "\n";
    
    cout << "Total Students: " << students.size() << "\n";
    cout << "Average GPA: " << fixed << setprecision(2) << (totalGPA / students.size()) << "\n";
    cout << "Average Percentage: " << (totalPercentage / students.size()) << "%\n";
    
    cout << "\n--- Grade Distribution ---\n";
    for (const auto &pair : gradeCount) {
        cout << "Grade " << pair.first << ": " << pair.second << " students\n";
    }
    
    cout << string(100, '=') << "\n";
}

void clearAll() {
    char confirm;
    cout << "\nThis will delete all records! Continue? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        students.clear();
        saveData();
        cout << "\n✓ All records deleted!\n";
    }
}

int main() {
    loadData();
    int choice;

    do {
        cout << "\n" << string(60, '=') << "\n";
        cout << "     STUDENT RECORD MANAGEMENT SYSTEM (Multi-Subject)\n";
        cout << string(60, '=') << "\n";
        cout << "  1. Add Single Student\n";
        cout << "  2. Add Multiple Students (Batch Entry)\n";
        cout << "  3. Display All Students\n";
        cout << "  4. Search Student\n";
        cout << "  5. Update Student Marks\n";
        cout << "  6. Delete Student\n";
        cout << "  7. View Top Performers\n";
        cout << "  8. Class Statistics\n";
        cout << "  9. Clear All Records\n";
        cout << "  0. Exit\n";
        cout << string(60, '=') << "\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: addMultipleStudents(); break;
            case 3: displayStudents(); break;
            case 4: searchStudent(); break;
            case 5: updateStudent(); break;
            case 6: deleteStudent(); break;
            case 7: viewToppers(); break;
            case 8: classStatistics(); break;
            case 9: clearAll(); break;
            case 0: cout << "\n✓ Exiting... Thank you!\n"; break;
            default: cout << "\n✗ Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}