/***************************************************************
 * Name:      quantracker_simMain.h
 * Purpose:   Defines Application Frame
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2013-03-06
 * Copyright: Andy (http://www.softpainter.org)
 * License:
 **************************************************************/

#ifndef QUANTRACKER_SIMMAIN_H
#define QUANTRACKER_SIMMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class sp_in_thread;
class splitter;
class panel;

class main_frame: public wxFrame
{
    public:
        main_frame(wxFrame *frame, const wxString& title);
        ~main_frame();
        bool Destroy();
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
            idTimer
        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnTimer(wxTimerEvent& event);
        DECLARE_EVENT_TABLE()

        splitter* m_splitter;
        wxTimer * Timer;
        wxCriticalSection m_thread_CS;
        sp_in_thread* m_sp_in_thread;
       
       friend class quantracker_simApp;
       friend class sp_in_thread;
       friend class panel;
};


#endif // QUANTRACKER_SIMMAIN_H
