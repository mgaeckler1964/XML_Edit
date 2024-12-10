//---------------------------------------------------------------------------
#include <winlib/xmlEditorChild.h>

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
	if( msg.Msg == WM_XML_ITEM_CLICK )
	{
		viewerInstance->setPosition( (void *)msg.LParam );
	}
}

//---------------------------------------------------------------------------
void TXmlViewerForm::XmlItemChanged( const TMessage &msg )
{
	if( msg.Msg == WM_XML_ITEM_CHANGED )
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
		case WM_XML_ITEM_CLICK:
			XmlItemClick(*((TMessage *)Message));
			break;
		case WM_XML_ITEM_CHANGED:
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



