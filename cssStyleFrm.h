//---------------------------------------------------------------------------

#ifndef cssStyleFrmH
#define cssStyleFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cssStyleFram.h"
//---------------------------------------------------------------------------
class TcssStyleForm : public TForm
{
__published:	// IDE-managed Components
	TcssStyleFrame *cssStyleFrame;
private:	// User declarations
public:		// User declarations
	__fastcall TcssStyleForm(TComponent* Owner);
	void setCssStyle( gak::css::Styles *styles )
	{
		cssStyleFrame->setCssStyle( styles );
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TcssStyleForm *cssStyleForm;
//---------------------------------------------------------------------------
#endif

