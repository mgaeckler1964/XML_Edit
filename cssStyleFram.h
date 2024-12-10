//---------------------------------------------------------------------------


#ifndef cssStyleFramH
#define cssStyleFramH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>


//---------------------------------------------------------------------------
#pragma option -RT-
namespace gak
{
namespace css
{
	class Styles;
}
}
#pragma option -RT+

class TcssStyleFrame : public TFrame
{
__published:	// IDE-managed Components
	TStringGrid *StringGridcssStyles;
	void __fastcall FrameResize(TObject *Sender);
	void __fastcall StringGridcssStylesDrawCell(TObject *Sender, int ACol,
		  int ARow, TRect &Rect, TGridDrawState State);
private:	// User declarations
	gak::css::Styles *styles;

public:		// User declarations
	__fastcall TcssStyleFrame(TComponent* Owner);
	void setCssStyle( gak::css::Styles *styles );
};
//---------------------------------------------------------------------------
extern PACKAGE TcssStyleFrame *cssStyleFrame;
//---------------------------------------------------------------------------
#endif

