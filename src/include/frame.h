

#include "titlebar.h"
#include <wx/webview.h>
#include <wx/wx.h>

class Frame : public wxFrame {
public:
  Frame();

private:
  TitleBar *m_titlebar;
  wxWebView *m_webview;
  void InitializeCookies();
  void LoadGtkCSS();
  // void OnTabChange(wxCommandEvent &event);
};