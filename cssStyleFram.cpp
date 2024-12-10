//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/css.h>

#pragma hdrstop

#include "cssStyleFram.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TcssStyleFrame *cssStyleFrame;
//---------------------------------------------------------------------------
__fastcall TcssStyleFrame::TcssStyleFrame(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TcssStyleFrame::setCssStyle( gak::css::Styles *styles )
{
	this->styles = styles;

	for( size_t i=0; gak::css::Styles::theCssFieldInfo[i].cssName; i++)
	{
		StringGridcssStyles->RowCount = i+1;
		StringGridcssStyles->Cells[0][i] = gak::css::Styles::theCssFieldInfo[i].cssName;
		StringGridcssStyles->Cells[1][i] = (const char *)(
			*styles->cssValue( gak::css::Styles::theCssFieldInfo[i].offset )
		);
	}
}
//---------------------------------------------------------------------------

void __fastcall TcssStyleFrame::FrameResize(TObject *)
{
	StringGridcssStyles->ColWidths[0] = ClientWidth/2;
	StringGridcssStyles->ColWidths[1] = ClientWidth/2;
}
//---------------------------------------------------------------------------

void __fastcall TcssStyleFrame::StringGridcssStylesDrawCell(
	  TObject *, int ACol, int ARow, TRect &Rect,
	  TGridDrawState )
{
	if( ACol )
	{
		gak::css::Value *theValue = styles->cssValue(
			gak::css::Styles::theCssFieldInfo[ARow].offset
		);
		if( theValue->isInherited() || theValue->isByShortForm() )
		{

			StringGridcssStyles->Canvas->Brush->Color = theValue->isInherited()
				? clRed
				: clWindow
			;
			StringGridcssStyles->Canvas->FillRect(Rect);

			if( theValue->isByShortForm() )
				StringGridcssStyles->Canvas->Font->Style =
					StringGridcssStyles->Canvas->Font->Style <<
					fsItalic
				;

			StringGridcssStyles->Canvas->TextOut(
				Rect.Left+20, Rect.Top+3, StringGridcssStyles->Cells[ACol][ARow]
			);
		}
	}
}
//---------------------------------------------------------------------------

