#include "LionWindow.h"

#include <wx/frame.h>
#include <wx/app.h>
#include <wx/menu.h>
#include <wx/intl.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>

namespace GUI {
    
    /// Main window of the application.
    class MainFrame: public wxFrame {
        public:
        
        /// Create a new window.
        MainFrame(wxFrame* parent,
                  const wxString& title = wxT(""), 
                  const wxPoint& pos = wxDefaultPosition, 
                  const wxSize& size = wxDefaultSize);
        
        /// Clean up resources owned by the window.
        virtual ~MainFrame();
        
        /// Initialize the main window 
        void init(int argc, wxChar** argv);
        
        /// Change the status bar message.
        void updateStatus(const wxString& s);
        
        protected:

        /// Exit event
        void onExit(wxCommandEvent& event);
            
        /// "About" event
        void onAbout(wxCommandEvent& event);
        
        /// File menu events
        void onFileMenu(wxCommandEvent& event);
        
        /// Window close event.
        void onClose(wxCloseEvent& event);
        
        /// Window paint event.
        void onPaint(wxPaintEvent& event);
        
        /// Open a file
        void open();
        
        /// Setup the status bar and menu bar.
        void initStandardGUI();

        
        LionWindow    panel;            ///< The AGG bitmap display panel

        wxMenuBar*  menuBar;            ///< Menu bar
        wxMenu*     fileMenu;           ///< File menu
        wxMenu*     helpMenu;           ///< Help menu
        
        DECLARE_EVENT_TABLE()           /// Allocate wxWidgets storage for event handlers
    };
        
    /// The top-most interface to wxWidgets.
    class Application: public wxApp {
        public:
        Application();
        
        /// wxWidgets calls this to initialize the user interface
        bool OnInit();
        
        protected:
        MainFrame* frame;
    };
        
    // Declare the event handlers for the menu items, etc.
    BEGIN_EVENT_TABLE(MainFrame, wxFrame)
        EVT_MENU(wxID_EXIT,        MainFrame::onExit)
        EVT_MENU(wxID_ABOUT,       MainFrame::onAbout)
        EVT_MENU(wxID_OPEN,        MainFrame::onFileMenu)
        EVT_CLOSE(                 MainFrame::onClose)
    END_EVENT_TABLE()
        
    MainFrame::MainFrame(wxFrame* parent,
                         const wxString& title, 
                         const wxPoint& pos, const wxSize& size) : 
        wxFrame(parent, wxID_ANY, title, pos, size),
        panel(this),
        menuBar(NULL), 
        fileMenu(NULL),
        helpMenu(NULL) {
            
        // All menus are deleted by wxMenuBar
        fileMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
        fileMenu->Append(wxID_OPEN, _("&Open\tCtrl-O"), _("Open..."));
        fileMenu->Append(wxID_EXIT, _("&Quit\tCtrl-Q"), _("Quit the application"));
        
        helpMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
        helpMenu->Append(wxID_ABOUT, _("About"), _("About"));
        
        // Deleted by wxFrame 
        menuBar = new wxMenuBar;
        menuBar->Append(fileMenu,   _("&File"));
        menuBar->Append(helpMenu,   _("&Help"));
        
        CreateStatusBar(2);
        SetMenuBar(menuBar);
        updateStatus(_("Nous sommes tous les pamplemousses."));
    }

    MainFrame::~MainFrame() {
    }
    
    void MainFrame::onClose(wxCloseEvent& WXUNUSED(event)) {
        Destroy();
    }
    
    void MainFrame::onFileMenu(wxCommandEvent& event) {
        switch (event.GetId()) {
            case wxID_OPEN:
                open();
                break;
                
            default:
                assert(!"Unknown event ID");
                break;
        }
    }

    void MainFrame::onExit(wxCommandEvent& WXUNUSED(event)) {
            Close(TRUE);
    }
    
    void MainFrame::onAbout(wxCommandEvent& WXUNUSED(event)) {
        wxMessageBox(_("Example of combining wxWidgets and the Anti-Grain Geometry renderer."), 
                     _("About wxAGG"));
    }
    
    void MainFrame::updateStatus(const wxString& s) {
        if (GetStatusBar())
            SetStatusText(s);
    }
    
    void MainFrame::open() {
// Commented because of compile error:
// ‘wxOPEN’ (wxMULTIPLE, wxFILE_MUST_EXIST) was not declared in this scope
//        // Get a file or a set of files.
//        wxFileDialog dlg(this, _("Open"), wxT(""), wxT(""), wxT("*"), wxOPEN | wxMULTIPLE | wxFILE_MUST_EXIST);
//
//        if (dlg.ShowModal() == wxID_CANCEL)
//            return;
//
//        wxArrayString files;
//        dlg.GetFilenames(files);
//
//        if (files.GetCount() == 0)
//            return;
//
//        // Do something intelligent with the files.
//        wxString msg;
//        for (int i = 0; i < (int) files.GetCount(); ++i) {
//            msg += files[i] + (i == files.GetCount() - 1 ? wxT("") : wxT("\n"));
//        }
//        wxMessageBox(msg, _("Selected files"), wxICON_INFORMATION | wxOK, this);
    }

    Application::Application():
        frame(NULL) {
        // empty
    }
    
    // Application initialization.
    bool Application::OnInit() {
        
        // Create a new window
        frame = new MainFrame((wxFrame *) NULL, _("wxAGG: A Cute Lion"),
                              wxDefaultPosition, wxSize(600, 600));
        
        SetTopWindow(frame);
        frame->Show(true);
        
        // Can do something application-specific here with the wxApp member
        // variables argc and argv.
        
        return true;
    }
    
} // namespace

// "allows wxWindows to dynamically create an instance of the application 
//  object at the appropriate point in wxWindows initialization"
IMPLEMENT_APP(GUI::Application)
