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
