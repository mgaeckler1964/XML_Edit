/*
		Project:		XML Editor
		Module:			
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

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

