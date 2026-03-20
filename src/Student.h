#pragma once
#include <string>
#include <vector>

struct Student {
    std::string id;
    std::string name;
    std::string dob;
    std::string className;
    int age = 0;
    float height = 0.0f;
    float weight = 0.0f;
    std::string phone;
    std::string address;
    bool active = true;

    std::string serialize(char delim = '|') const;
    static Student deserialize(const std::string& line, char delim = '|');
};