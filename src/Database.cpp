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
        // Parse only the ID field (before the first delimiter) — much faster
        // than deserializing all 10 fields just to obtain the record offset.
        const auto delimPos = line.find('|');
        if (delimPos != std::string::npos && delimPos > 0) {
            index[line.substr(0, delimPos)] = offset;
        }
        offset = static_cast<long long>(in.tellg());
    }
}

bool Database::addStudent(const Student& s) {
    if (index.count(s.id)) return false;

    // Open in read+write mode positioned at end-of-file (ios::ate).
    // Recording tellp() and writing happen on the same open stream, so the
    // offset is always consistent with the actual write position.
    std::fstream file(filePath, std::ios::in | std::ios::out | std::ios::ate);
    if (!file.is_open()) {
        // File does not exist yet — create it.
        file.open(filePath, std::ios::in | std::ios::out | std::ios::trunc);
        if (!file.is_open()) return false;
    }

    const long long offset = static_cast<long long>(file.tellp());
    file << s.serialize() << "\n";
    if (!file) return false;

    index[s.id] = offset;
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
    result.reserve(index.size()); // avoid repeated reallocations
    std::ifstream in(filePath);
    if (!in.is_open()) return result;

    std::string line;
    while (std::getline(in, line)) {
        Student s = Student::deserialize(line);
        if (s.active) result.push_back(std::move(s));
    }
    return result;
}

void Database::rewriteFile(const std::vector<Student>& students) {
    // Build the entire file content in memory first, then write it in one shot
    // to reduce the number of individual I/O operations.
    static constexpr std::size_t ESTIMATED_BYTES_PER_RECORD = 64;
    std::string buffer;
    buffer.reserve(students.size() * ESTIMATED_BYTES_PER_RECORD);
    for (const auto& s : students) {
        buffer += s.serialize();
        buffer += '\n';
    }
    std::ofstream out(filePath, std::ios::trunc);
    out.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
}