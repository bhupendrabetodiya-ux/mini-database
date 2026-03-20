#include "Student.h"
#include <sstream>

std::string Student::serialize(char delim) const {
    std::ostringstream oss;
    oss << id << delim
        << name << delim
        << dob << delim
        << className << delim
        << age << delim
        << height << delim
        << weight << delim
        << phone << delim
        << address << delim
        << (active ? 1 : 0);
    return oss.str();
}

Student Student::deserialize(const std::string& line, char delim) {
    Student s;
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> fields;
    fields.reserve(10);

    while (std::getline(ss, token, delim)) {
        fields.push_back(std::move(token));
    }

    if (fields.size() >= 10) {
        s.id        = std::move(fields[0]);
        s.name      = std::move(fields[1]);
        s.dob       = std::move(fields[2]);
        s.className = std::move(fields[3]);
        s.age       = std::stoi(fields[4]);
        s.height    = std::stof(fields[5]);
        s.weight    = std::stof(fields[6]);
        s.phone     = std::move(fields[7]);
        s.address   = std::move(fields[8]);
        s.active    = (fields[9] == "1");
    }
    return s;
}