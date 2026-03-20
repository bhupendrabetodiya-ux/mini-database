#pragma once
#include "Student.h"
#include <string>
#include <unordered_map>
#include <vector>

class Database {
public:
    Database(const std::string& filePath);

    void load();
    bool addStudent(const Student& s);
    bool updateStudent(const Student& s);
    bool deleteStudent(const std::string& id);
    bool getStudentById(const std::string& id, Student& out);

    std::vector<Student> getAllActiveStudents() const;

private:
    std::string filePath;
    std::unordered_map<std::string, long long> index; // id -> file offset

    void rebuildIndex();
    void rewriteFile(const std::vector<Student>& students);
};