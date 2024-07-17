#include "frame.h"
#include "constants.h"
#include <glib.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

Frame::Frame()
    : wxFrame(nullptr, wxID_ANY, WindowConstants::kTitle, wxDefaultPosition,
              wxSize(800, 600),
              wxNO_BORDER | wxMAXIMIZE_BOX | wxRESIZE_BORDER) {
  gtk_init(nullptr, nullptr);
  InitializeCookies();
  LoadGtkCSS();

  m_titlebar = new TitleBar(this);

  m_titlebar->AddTab("YouTube",
                     wxWebView::New(this, wxID_ANY, "https://youtube.com"));
  m_titlebar->AddTab("Google",
                     wxWebView::New(this, wxID_ANY, "https://google.com"));
  m_titlebar->AddTab("Strafe",
                     wxWebView::New(this, wxID_ANY, "https://strafechat.dev"));

  m_titlebar->m_webviews[0]->Show();

  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

  sizer->Add(m_titlebar, 0, wxEXPAND);
  sizer->Add(m_titlebar->m_webviews[0], 1, wxEXPAND);

  SetSizer(sizer);

  // m_webview->Bind(wxEVT_WEBVIEW_FULLSCREEN_CHANGED,
  // &Frame::OnFullScreenChange,
  //                 this);
}

void Frame::InitializeCookies() {
  const gchar *cookiePath = "/home/rusty/dev/browser/bin/cookies.db";

  WebKitWebContext *context = webkit_web_context_get_default();
  WebKitCookieManager *cookieManager =
      webkit_web_context_get_cookie_manager(context);

  webkit_cookie_manager_set_persistent_storage(
      cookieManager, cookiePath, WEBKIT_COOKIE_PERSISTENT_STORAGE_TEXT);

  WebKitCookieAcceptPolicy policy = WEBKIT_COOKIE_POLICY_ACCEPT_ALWAYS;
  webkit_cookie_manager_set_accept_policy(cookieManager, policy);

  // Debugging to check if the cookie manager is set correctly
  std::cout << "Initialized cookies with path: " << cookiePath << std::endl;
  std::cout << "Cookie accept policy: "
            << (policy == WEBKIT_COOKIE_POLICY_ACCEPT_ALWAYS ? "ACCEPT_ALWAYS"
                                                             : "OTHER_POLICY")
            << std::endl;
}

void Frame::LoadGtkCSS() {

  GtkCssProvider *provider = gtk_css_provider_new();
  GdkDisplay *display = gdk_display_get_default();
  GdkScreen *screen = gdk_display_get_default_screen(display);

  const gchar *cssData = "* {background-color: black; color: white;}";

  GError *error = NULL;

  if (!gtk_css_provider_load_from_file(
          provider, g_file_new_for_path("/home/rusty/dev/browser/styles.css"),
          &error)) {
    g_warning("Error loading CSS file: %s", error->message);
    g_clear_error(&error);
  }

  gtk_style_context_add_provider_for_screen(
      screen, GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  g_object_unref(provider);
}