#include "Database.h"
#include <fstream>

Database::Database(const std::string& filePath) : filePath(filePath) {}

void Database::load() {
    rebuildIndex();
}

void Database::rebuildIndex() {
    index.clear();
    std::ifstream in(filePath);
    if (!in.is_open()) return;

    std::string line;
    long long offset = 0;
    while (std::getline(in, line)) {
        Student s = Student::deserialize(line);
        if (!s.id.empty()) {
            index[s.id] = offset;
        }
        offset = in.tellg();
    }
}

bool Database::addStudent(const Student& s) {
    if (index.find(s.id) != index.end()) return false;

    std::ofstream out(filePath, std::ios::app);
    if (!out.is_open()) return false;

    out << s.serialize() << "\n";
    out.flush();
    rebuildIndex();
    return true;
}

bool Database::getStudentById(const std::string& id, Student& out) {
    auto it = index.find(id);
    if (it == index.end()) return false;

    std::ifstream in(filePath);
    if (!in.is_open()) return false;

    in.seekg(it->second);
    std::string line;
    std::getline(in, line);
    out = Student::deserialize(line);
    return out.active;
}

bool Database::updateStudent(const Student& s) {
    std::vector<Student> all = getAllActiveStudents();
    bool found = false;

    for (auto& st : all) {
        if (st.id == s.id) {
            st = s;
            found = true;
            break;
        }
    }
    if (!found) return false;

    rewriteFile(all);
    rebuildIndex();
    return true;
}

bool Database::deleteStudent(const std::string& id) {
    std::vector<Student> all = getAllActiveStudents();
    bool found = false;

    for (auto& st : all) {
        if (st.id == id) {
            st.active = false;
            found = true;
            break;
        }
    }
    if (!found) return false;

    rewriteFile(all);
    rebuildIndex();
    return true;
}

std::vector<Student> Database::getAllActiveStudents() const {
    std::vector<Student> result;
    std::ifstream in(filePath);
    if (!in.is_open()) return result;

    std::string line;
    while (std::getline(in, line)) {
        Student s = Student::deserialize(line);
        if (s.active) result.push_back(s);
    }
    return result;
}

void Database::rewriteFile(const std::vector<Student>& students) {
    std::ofstream out(filePath, std::ios::trunc);
    for (const auto& s : students) {
        out << s.serialize() << "\n";
    }
}