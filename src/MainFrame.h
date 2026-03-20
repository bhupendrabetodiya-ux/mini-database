#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "Database.h"

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    Database db;

    wxTextCtrl *idCtrl, *nameCtrl, *dobCtrl, *classCtrl, *ageCtrl,
               *heightCtrl, *weightCtrl, *phoneCtrl, *addressCtrl;

    wxListCtrl* listCtrl;

    void OnAdd(wxCommandEvent& event);
    void OnUpdate(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnSearch(wxCommandEvent& event);
    void RefreshList();

    wxDECLARE_EVENT_TABLE();
};