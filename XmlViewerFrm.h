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
