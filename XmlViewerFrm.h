//---------------------------------------------------------------------------

#ifndef XmlViewerFrmH
#define XmlViewerFrmH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include <winlib/xmlEditorChild.h>

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <gak/ChangeManager.h>
//---------------------------------------------------------------------------

class TXmlViewerForm;

#pragma option -RT-

class XML_GUI_VIEWER : public gak::DocumentViewer
{
	TXmlViewerForm	*theViewer;

	public:
	XML_GUI_VIEWER( gak::ChangeManager *manager, TXmlViewerForm *theViewer)
	: gak::DocumentViewer( manager )
	{
		this->theViewer = theViewer;
	}
	void handlePositionChange( void *document, void *position );
	void handleChange( void *document, void *item );
	void handleNew( void *document, void *item );
	void handleDelete( void *document, void *item );
};
#pragma option -RT.

class TXmlViewerForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelToolBar;
	TCheckBox *CheckBoxXmlFrames;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CheckBoxXmlFramesClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
	XML_GUI_VIEWER				*viewerInstance;
	winlib::BasicWindow			myWinlibHandle;
	winlib::XMLeditorChild		xmlEditor;
	gak::xml::Document			*theDocument;

	void XmlItemClick( const TMessage &msg );
	void XmlItemChanged( const TMessage &msg );
	void DialogKeyDown( TMessage &msg );
public:		// User declarations
	__fastcall TXmlViewerForm(TComponent* Owner);

	void setDocument( gak::xml::Document *newDocument, gak::ChangeManager *manager )
	{
		doEnterFunction("TXmlViewerForm::setDocument");
		theDocument = newDocument;

		if( viewerInstance )
			delete viewerInstance;
		viewerInstance = new XML_GUI_VIEWER( manager, this );

		AnsiString newCaption = "XML Viewer ";
		newCaption += (const char *)newDocument->getFilename();
		Caption = newCaption;

		xmlEditor.setDocument( newDocument );
	}
	void refresh( void *item )
	{
		if( item
		&& dynamic_cast<gak::xml::XmlText*>( static_cast<gak::xml::Element*>(item ) ) )
			xmlEditor.refresh();
		else
			xmlEditor.setDocument( theDocument );
	}
	void showElement( void *item )
	{
		if( item )
			xmlEditor.showElement( static_cast<gak::xml::Element*>( item  ) );
	}
	virtual void __fastcall Dispatch(void *Message);
};
//---------------------------------------------------------------------------
extern PACKAGE TXmlViewerForm *XmlViewerForm;
//---------------------------------------------------------------------------
#endif
