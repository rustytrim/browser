#pragma once
#include <vector>
#include <wx/notebook.h>
#include <wx/webview.h>
#include <wx/wx.h>

class TitleBar : public wxPanel {
public:
  TitleBar(wxFrame *parent);
  void AddTab(const wxString &label, wxWebView *webview);
  std::vector<wxButton *> m_tabs;
  std::vector<wxWebView *> m_webviews;

private:
  bool m_dragging;
  wxPoint m_dragStart;
  wxSizer *m_sizer;
  wxSizer *m_tabSizer;

  int m_selectedIndex = 0;
  int m_previousIndex = 0;

  void OnMinimize(wxCommandEvent &event);
  void OnMaximize(wxCommandEvent &event);
  void OnClose(wxCommandEvent &event);
  void OnLeftDown(wxMouseEvent &event);
  void OnLeftUp(wxMouseEvent &event);
  void OnMouseMove(wxMouseEvent &event);
  void OnTabSelect(wxCommandEvent &event);
  void OnFullScreenChange(wxWebViewEvent &event);
};