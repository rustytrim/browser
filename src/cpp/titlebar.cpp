#include "titlebar.h"
#include "constants.h"
#include "frame.h"

TitleBar::TitleBar(wxFrame *parent) : wxPanel(parent, wxID_ANY) {
  m_tabs.clear();
  m_webviews.clear();

  m_sizer = new wxBoxSizer(wxHORIZONTAL);
  m_tabSizer = new wxBoxSizer(wxHORIZONTAL);

  wxButton *minimizeButton = new wxButton(this, wxID_ICONIZE_FRAME, "-");
  wxButton *maximizeButton = new wxButton(this, wxID_MAXIMIZE_FRAME, "[]");
  wxButton *closeButton = new wxButton(this, wxID_CLOSE, "X");

  m_sizer->Add(m_tabSizer, 1, wxALIGN_LEFT | wxALL, 0);
  m_sizer->Add(minimizeButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  m_sizer->Add(maximizeButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  m_sizer->Add(closeButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  SetSizer(m_sizer);

  Bind(wxEVT_BUTTON, &TitleBar::OnTabSelect, this, wxID_ANY);

  Bind(wxEVT_BUTTON, &TitleBar::OnMinimize, this, wxID_ICONIZE_FRAME);
  Bind(wxEVT_BUTTON, &TitleBar::OnMaximize, this, wxID_MAXIMIZE_FRAME);
  Bind(wxEVT_BUTTON, &TitleBar::OnClose, this, wxID_CLOSE);

  Bind(wxEVT_LEFT_DOWN, &TitleBar::OnLeftDown, this);
  Bind(wxEVT_LEFT_UP, &TitleBar::OnLeftUp, this);
  Bind(wxEVT_MOTION, &TitleBar::OnMouseMove, this);

  m_dragging = false;
}

void TitleBar::AddTab(const wxString &label, wxWebView *webview) {
  wxButton *tab = new wxButton(this, wxID_ANY, label);
  wxSize textSize = tab->GetTextExtent(label);
  tab->SetMinSize(textSize + wxSize(10, 10));

  m_tabSizer->Add(tab, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  m_tabs.push_back(tab);
  m_webviews.push_back(webview);
  webview->Hide();

  webview->Bind(wxEVT_WEBVIEW_FULLSCREEN_CHANGED, &TitleBar::OnFullScreenChange,
                this);
}

void TitleBar::OnMinimize(wxCommandEvent &event) {
  wxFrame *frame = dynamic_cast<wxFrame *>(GetParent());

  if (!frame)
    return;

  frame->Iconize();
  event.Skip();
}

void TitleBar::OnMaximize(wxCommandEvent &event) {
  wxFrame *frame = dynamic_cast<wxFrame *>(GetParent());

  if (!frame)
    return;

  frame->Maximize(!frame->IsMaximized());
  event.Skip();
}

void TitleBar::OnClose(wxCommandEvent &event) {
  GetParent()->Close();
  event.Skip();
}

void TitleBar::OnLeftDown(wxMouseEvent &event) {
  m_dragging = true;
  m_dragStart = event.GetPosition();
  event.Skip();
}

void TitleBar::OnLeftUp(wxMouseEvent &event) {
  m_dragging = false;
  event.Skip();
}

void TitleBar::OnMouseMove(wxMouseEvent &event) {
  if (m_dragging) {
    wxPoint pos = GetParent()->ClientToScreen(event.GetPosition());
    wxPoint origin = pos - m_dragStart;
    GetParent()->Move(origin);
    event.Skip();
  }
}

void TitleBar::OnTabSelect(wxCommandEvent &event) {
  wxButton *selectedTab = dynamic_cast<wxButton *>(event.GetEventObject());

  if (!selectedTab)
    return;

  int selectedIndex = -1;
  for (std::size_t i = 0; i < m_tabs.size(); ++i) {
    if (m_tabs[i]->GetId() == selectedTab->GetId()) {
      selectedIndex = i;
      break;
    }
  }

  if (selectedIndex == -1 ||
      selectedIndex >= static_cast<int>(m_webviews.size()))
    return;

  for (std::size_t i = 0; i < m_webviews.size(); ++i) {
    if (static_cast<int>(i) == selectedIndex)
      m_webviews[i]->Show();
    else
      m_webviews[i]->Hide();
  }

  m_selectedIndex = selectedIndex;

  Frame *frame = dynamic_cast<Frame *>(GetParent());
  if (frame) {
    wxSizer *frameSizer = frame->GetSizer();
    if (frameSizer) {
      frameSizer->Replace(m_webviews[m_previousIndex],
                          m_webviews[selectedIndex]);
      frame->Layout();
    }
  }

  m_previousIndex = selectedIndex;
  event.Skip();
  Layout();
}

void TitleBar::OnFullScreenChange(wxWebViewEvent &event) {
  Frame *frame = static_cast<Frame *>(GetParent());
  if (!frame->IsFullScreen())
    Hide();
  else
    Show();

  event.Skip();
  Layout();
  frame->Layout();
}
