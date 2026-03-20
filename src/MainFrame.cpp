#include "MainFrame.h"

enum {
    ID_ADD = 1,
    ID_UPDATE,
    ID_DELETE,
    ID_SEARCH
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_ADD, MainFrame::OnAdd)
    EVT_BUTTON(ID_UPDATE, MainFrame::OnUpdate)
    EVT_BUTTON(ID_DELETE, MainFrame::OnDelete)
    EVT_BUTTON(ID_SEARCH, MainFrame::OnSearch)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "School Mini Database", wxDefaultPosition, wxSize(900, 600)),
      db("data/students.dat")
{
    db.load();

    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* formSizer = new wxGridSizer(2, 10, 10);

    idCtrl = new wxTextCtrl(panel, wxID_ANY);
    nameCtrl = new wxTextCtrl(panel, wxID_ANY);
    dobCtrl = new wxTextCtrl(panel, wxID_ANY);
    classCtrl = new wxTextCtrl(panel, wxID_ANY);
    ageCtrl = new wxTextCtrl(panel, wxID_ANY);
    heightCtrl = new wxTextCtrl(panel, wxID_ANY);
    weightCtrl = new wxTextCtrl(panel, wxID_ANY);
    phoneCtrl = new wxTextCtrl(panel, wxID_ANY);
    addressCtrl = new wxTextCtrl(panel, wxID_ANY);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "ID"));
    formSizer->Add(idCtrl);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Name"));
    formSizer->Add(nameCtrl);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "DOB"));
    formSizer->Add(dobCtrl);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Class"));
    formSizer->Add(classCtrl);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Age"));
    formSizer->Add(ageCtrl);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Height"));
    formSizer->Add(heightCtrl);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Weight"));
    formSizer->Add(weightCtrl);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Phone"));
    formSizer->Add(phoneCtrl);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Address"));
    formSizer->Add(addressCtrl);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(panel, ID_ADD, "Add"));
    buttonSizer->Add(new wxButton(panel, ID_UPDATE, "Update"));
    buttonSizer->Add(new wxButton(panel, ID_DELETE, "Delete"));
    buttonSizer->Add(new wxButton(panel, ID_SEARCH, "Search"));

    listCtrl = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(850, 300),
                              wxLC_REPORT | wxLC_SINGLE_SEL);

    listCtrl->InsertColumn(0, "ID");
    listCtrl->InsertColumn(1, "Name");
    listCtrl->InsertColumn(2, "DOB");
    listCtrl->InsertColumn(3, "Class");
    listCtrl->InsertColumn(4, "Age");
    listCtrl->InsertColumn(5, "Height");
    listCtrl->InsertColumn(6, "Weight");
    listCtrl->InsertColumn(7, "Phone");
    listCtrl->InsertColumn(8, "Address");

    mainSizer->Add(formSizer, 0, wxALL, 10);
    mainSizer->Add(buttonSizer, 0, wxALL, 10);
    mainSizer->Add(listCtrl, 1, wxALL | wxEXPAND, 10);

    panel->SetSizer(mainSizer);

    RefreshList();
}

void MainFrame::RefreshList() {
    listCtrl->DeleteAllItems();
    auto students = db.getAllActiveStudents();
    long index = 0;
    for (const auto& s : students) {
        listCtrl->InsertItem(index, s.id);
        listCtrl->SetItem(index, 1, s.name);
        listCtrl->SetItem(index, 2, s.dob);
        listCtrl->SetItem(index, 3, s.className);
        listCtrl->SetItem(index, 4, std::to_string(s.age));
        listCtrl->SetItem(index, 5, std::to_string(s.height));
        listCtrl->SetItem(index, 6, std::to_string(s.weight));
        listCtrl->SetItem(index, 7, s.phone);
        listCtrl->SetItem(index, 8, s.address);
        index++;
    }
}

void MainFrame::OnAdd(wxCommandEvent&) {
    Student s;
    s.id = idCtrl->GetValue().ToStdString();
    s.name = nameCtrl->GetValue().ToStdString();
    s.dob = dobCtrl->GetValue().ToStdString();
    s.className = classCtrl->GetValue().ToStdString();
    s.age = std::stoi(ageCtrl->GetValue().ToStdString());
    s.height = std::stof(heightCtrl->GetValue().ToStdString());
    s.weight = std::stof(weightCtrl->GetValue().ToStdString());
    s.phone = phoneCtrl->GetValue().ToStdString();
    s.address = addressCtrl->GetValue().ToStdString();
    s.active = true;

    if (db.addStudent(s)) {
        RefreshList();
        wxMessageBox("Student added!");
    } else {
        wxMessageBox("ID already exists!");
    }
}

void MainFrame::OnUpdate(wxCommandEvent&) {
    Student s;
    s.id = idCtrl->GetValue().ToStdString();
    s.name = nameCtrl->GetValue().ToStdString();
    s.dob = dobCtrl->GetValue().ToStdString();
    s.className = classCtrl->GetValue().ToStdString();
    s.age = std::stoi(ageCtrl->GetValue().ToStdString());
    s.height = std::stof(heightCtrl->GetValue().ToStdString());
    s.weight = std::stof(weightCtrl->GetValue().ToStdString());
    s.phone = phoneCtrl->GetValue().ToStdString();
    s.address = addressCtrl->GetValue().ToStdString();
    s.active = true;

    if (db.updateStudent(s)) {
        RefreshList();
        wxMessageBox("Student updated!");
    } else {
        wxMessageBox("Student not found!");
    }
}

void MainFrame::OnDelete(wxCommandEvent&) {
    std::string id = idCtrl->GetValue().ToStdString();
    if (db.deleteStudent(id)) {
        RefreshList();
        wxMessageBox("Student deleted!");
    } else {
        wxMessageBox("Student not found!");
    }
}

void MainFrame::OnSearch(wxCommandEvent&) {
    std::string id = idCtrl->GetValue().ToStdString();
    Student s;
    if (db.getStudentById(id, s)) {
        nameCtrl->SetValue(s.name);
        dobCtrl->SetValue(s.dob);
        classCtrl->SetValue(s.className);
        ageCtrl->SetValue(std::to_string(s.age));
        heightCtrl->SetValue(std::to_string(s.height));
        weightCtrl->SetValue(std::to_string(s.weight));
        phoneCtrl->SetValue(s.phone);
        addressCtrl->SetValue(s.address);
    } else {
        wxMessageBox("Student not found!");
    }
}