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
#include <winlib/xmlEditorChild.h>
#include <winlib/messages.h>

#include <vcl.h>

#pragma hdrstop

#include "XmlViewerFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TXmlViewerForm *XmlViewerForm;

//---------------------------------------------------------------------------
__fastcall TXmlViewerForm::TXmlViewerForm(TComponent* Owner)
	: TForm(Owner),
	myWinlibHandle( winlib::ForeignWindowFlag, (winlib::BasicWindow*)NULL ),
	xmlEditor( (winlib::BasicWindow*)NULL )
{
	viewerInstance = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewerForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );
	xmlEditor.sizeNmove(
		0, PanelToolBar->Height,
		ClientWidth, ClientHeight-PanelToolBar->Height
	);
	myWinlibHandle.setHandle( this );
	xmlEditor.create( &myWinlibHandle );
}
//---------------------------------------------------------------------------

void __fastcall TXmlViewerForm::FormResize(TObject *)
{
	xmlEditor.resize( ClientWidth, ClientHeight-PanelToolBar->Height );
}
//---------------------------------------------------------------------------
void TXmlViewerForm::XmlItemClick( const TMessage &msg )
{
	if( msg.Msg == winlib::WM_XML_ITEM_CLICK )
	{
		viewerInstance->setPosition( (void *)msg.LParam );
	}
}

//---------------------------------------------------------------------------
void TXmlViewerForm::XmlItemChanged( const TMessage &msg )
{
	if( msg.Msg == winlib::WM_XML_ITEM_CHANGED )
		viewerInstance->setChanged( (void *)msg.LParam );
}
//---------------------------------------------------------------------------
void TXmlViewerForm::DialogKeyDown( TMessage &msg )
{
	msg.Result = 0;		// do not consume key strokes
}
//---------------------------------------------------------------------------

void __fastcall TXmlViewerForm::CheckBoxXmlFramesClick(TObject *)
{
	if( CheckBoxXmlFrames->Checked )
		xmlEditor.enableXmlFrames();
	else
		xmlEditor.disableXmlFrames();
}
//---------------------------------------------------------------------------
void XML_GUI_VIEWER::handlePositionChange( void *, void *position )
{
	theViewer->showElement( position );
}
//---------------------------------------------------------------------------
void XML_GUI_VIEWER::handleChange( void *, void *item )
{
	theViewer->refresh( item );
}
//---------------------------------------------------------------------------
void XML_GUI_VIEWER::handleNew( void *, void * )
{
	theViewer->refresh( NULL );
}
//---------------------------------------------------------------------------
void XML_GUI_VIEWER::handleDelete( void *, void * )
{
	theViewer->refresh( NULL );
}
//---------------------------------------------------------------------------

void __fastcall TXmlViewerForm::FormClose(TObject *, TCloseAction &Action)
{
	if( viewerInstance )
	{
		delete viewerInstance;
		viewerInstance = NULL;
	}
	Action = caHide;
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewerForm::Dispatch(void *Message)
{
	switch( ((PMessage)Message)->Msg )
	{
		case winlib::WM_XML_ITEM_CLICK:
			XmlItemClick(*((TMessage *)Message));
			break;
		case winlib::WM_XML_ITEM_CHANGED:
			XmlItemChanged(*((TMessage *)Message));
			break;
		case CM_DIALOGKEY:
			DialogKeyDown(*((TMessage *)Message));
			break;
		default:
			TForm::Dispatch(Message);
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TXmlViewerForm::FormActivate(TObject *)
{
	xmlEditor.focus();
}
//---------------------------------------------------------------------------



