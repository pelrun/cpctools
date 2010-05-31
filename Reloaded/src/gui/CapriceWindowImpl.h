#ifndef _CAPRICEWINDOWIMPL_H_
#define _CAPRICEWINDOWIMPL_H_


#include "CapriceUI.h"
#include "CapriceRegistersDialogImpl.h"
#include "CapriceAboutImpl.h"
#include "WXEmulator.h"

#include <wx/dnd.h>
#include <wx/image.h>

#if ENABLE_FILEDROP
class CapriceDNDHandler : public wxFileDropTarget
{
	public:
		CapriceDNDHandler(WXEmulator* emu);
	protected:
    	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
	private:
		WXEmulator* emulator;
};
#endif

class CapriceWindowImpl : public CapriceWindow
{
  public:

	CapriceWindowImpl(WXEmulator* emu);
	virtual ~CapriceWindowImpl();

    void SetEmulator(WXEmulator *emulator);   
    Emulator* GetEmulator() {return emulator;};   
    /**
     * Get the emulator panel
     */
    wxPanel* getPanel();
    /**
     * Set the image to show
     */
    inline void SetEmuImage(wxImage* img)
    {
      this->image = img ;
    };

	void Pause();

	void fdcLed(bool on);

    protected:

    virtual void onExit1( wxCloseEvent& );
    virtual void OnIdle( wxIdleEvent& event );

    virtual void onInsertDiscA( wxCommandEvent& event );
    virtual void onInsertDiscB( wxCommandEvent& event );

	virtual void menu_editDiskA( wxCommandEvent& event );

    virtual void onLoadSNA( wxCommandEvent& event );
    virtual void onSaveSNA( wxCommandEvent& event );
    virtual void onSaveScreen( wxCommandEvent& event );

	virtual void insertTape( wxCommandEvent& event );
	virtual void pressPlayOnTape( wxCommandEvent& event );

    virtual void onExit2( wxCommandEvent& );
    
    
    virtual void OnFullScreen( wxCommandEvent& event);
    virtual void OnReset( wxCommandEvent& event);

    virtual void OnRun( wxCommandEvent& event) ;
    virtual void OnStep( wxCommandEvent& event) ;
    virtual void OnPause( wxCommandEvent& event) ;

    virtual void OnRegister( wxCommandEvent& event);
    virtual void OnDebugMemory( wxCommandEvent& event );
    /**
     * Open the IDE
     */
    virtual void OnShowAssembler( wxCommandEvent& event );

    virtual void OnAbout( wxCommandEvent& event);
    virtual void onMenuInput( wxCommandEvent& event);
    virtual void onMenuMemory( wxCommandEvent& event);
    virtual void onMenuVideo( wxCommandEvent& event);
    virtual void onMenuSound( wxCommandEvent& event);
    virtual void onMenuMisc( wxCommandEvent& event);
	virtual void onMultifaceStop( wxCommandEvent& event );

    virtual void windowKeyDown( wxKeyEvent& event);
    virtual void windowKeyUp( wxKeyEvent& event );

    private:
    /**
     * Pointer to the emu
     */
    WXEmulator*   emulator ;
    /**
     * Pointer to the emu display
     */
    wxImage*    image;

#if ENABLE_FILEDROP
	CapriceDNDHandler* dndhandler;
#endif
};
#endif
