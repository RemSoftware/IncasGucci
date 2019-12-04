//----------------------------------------------------------------------------
#ifndef ChildWinH
#define ChildWinH
//----------------------------------------------------------------------------
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <StdCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TMDIChild : public TForm
{
__published:
	TPanel *PanelButtons;
	TPanel *PanelClose;
	TBitBtn *BitBtnClose;
	TPanel *PanelTitle;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BitBtnCloseClick(TObject *Sender);
protected:
	void __fastcall WmEraseBkgnd(TWMEraseBkgnd &Message);
private:
public:
	virtual __fastcall TMDIChild(TComponent *Owner);
	virtual __fastcall void AfterConstruction(void);
	void  SetVisible();
	void  SetInvisible();
	void Mostra();
};
//----------------------------------------------------------------------------
#endif	
