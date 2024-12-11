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

#ifndef xmlEditFrmH
#define xmlEditFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "xmlEditFram.h"

#include <gak/ChangeManager.h>

//---------------------------------------------------------------------------
class TXmlViewerForm;

class TXmlEditForm : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenDialog;
	TMainMenu *MainMenu;
	TMenuItem *File1;
	TMenuItem *New1;
	TMenuItem *Open1;
	TMenuItem *MenuFileSave;
	TMenuItem *MenuFileSaveAs;
	TMenuItem *Exit1;
	TSaveDialog *SaveDialog;
	TMenuItem *Edit1;
	TMenuItem *Copy1;
	TMenuItem *Cut1;
	TMenuItem *Paste1;
	TMenuItem *MenuSchema;
	TMenuItem *MenuSchemaTestDocument;
	TxmlEditorFrame *xmlEditorFrame;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *MenuSchemaOpen;
	TMenuItem *MenuTransform;
	TMenuItem *Transform2;
	TMenuItem *OpenStylesheet1;
	TMenuItem *MenuSchemaRefreshSchema;
	TMenuItem *N3;
	TMenuItem *Renderer1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall New1Click(TObject *Sender);
	void __fastcall MenuFileSaveAsClick(TObject *Sender);
	void __fastcall MenuFileSaveClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Cut1Click(TObject *Sender);
	void __fastcall Copy1Click(TObject *Sender);
	void __fastcall Paste1Click(TObject *Sender);
	void __fastcall MenuSchemaTestDocumentClick(TObject *Sender);
	void __fastcall MenuSchemaOpenClick(TObject *Sender);
	void __fastcall OpenStylesheet1Click(TObject *Sender);
	void __fastcall Transform2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall MenuSchemaRefreshSchemaClick(TObject *Sender);
	void __fastcall Renderer1Click(TObject *Sender);
private:	// User declarations
	gak::STRING			filename;
	gak::ChangeManager	theManager;
	gak::css::Rules		editorCssRules;

	void styleChangedCB(
		const gak::STRING &styleSheetFile, const gak::STRING &styleSheetType
	);
	void schemaChangedCB( const gak::STRING &schemaFile );
	void OpenFile( const gak::STRING &fileName );

	bool hasChanged( void ) const
	{
		return theManager.isChanged();
	}

public:		// User declarations
	__fastcall TXmlEditForm(TComponent* Owner);

	void selectXmlElement( gak::xml::Element *theElement )
	{
		xmlEditorFrame->selectXmlElement( theElement );
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TXmlEditForm *XmlEditForm;
//---------------------------------------------------------------------------
#endif
