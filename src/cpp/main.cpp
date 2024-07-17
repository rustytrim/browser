#include "frame.h"
#include <wx/wx.h>

class App : public wxApp {
public:
  virtual bool OnInit() {
    Frame *frame = new Frame();
    frame->Show(true);
    return true;
  }
};

wxIMPLEMENT_APP(App);