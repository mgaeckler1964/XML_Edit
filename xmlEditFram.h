//---------------------------------------------------------------------------


#ifndef xmlEditFramH
#define xmlEditFramH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>

#include <iostream>
#include <fstream>
#include <ActnList.hpp>

#include <gak/string.h>
#include <gak/stringStream.h>
#include <gak/xmlValidator.h>
#include <gak/ChangeManager.h>
#include <gak/directory.h>

//---------------------------------------------------------------------------
class TxmlEditorFrame;
//---------------------------------------------------------------------------
#pragma option -RT-

typedef gak::xml::Document * (*TXmlDocLoad)( const gak::STRING &pathName );
typedef gak::STRING (*TPlainTextLoad)( const gak::STRING &pathName );
typedef gak::STRING (*TSchemaFile4NamespaceCB)( const gak::STRING &nameSpace );

class XML_MY_SCHEMA_MANAGER : public gak::xml::SchemaManager
{
	static TXmlDocLoad					xmlDocLoadFunction;
	static TSchemaFile4NamespaceCB		schemaFile4NamespaceCB;

	public:
//	XML_MY_SCHEMA_MANAGER()	{}
	virtual gak::STRING getSchemaFile4Namespace( const gak::STRING &nameSpace );
	virtual gak::xml::Document *loadSchemaFile( const gak::STRING &schemaFile );

	static void setXmlDocLoadFunction( TXmlDocLoad newFunction )
	{
		xmlDocLoadFunction = newFunction;
	}
	static void setSchemaFile4NamespaceCB( TSchemaFile4NamespaceCB newCB )
	{
		schemaFile4NamespaceCB = newCB;
	}
};

class XML_TREE_VIEWER : public gak::DocumentViewer
{
	TxmlEditorFrame	*theViewer;

	public:
	XML_TREE_VIEWER( gak::ChangeManager *manager, TxmlEditorFrame *theViewer)
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
//---------------------------------------------------------------------------
typedef void (__closure *TStyleChanged)(
	const gak::STRING &style, const gak::STRING &stylesheetType
);
typedef void (__closure *TXmlSchemaChanged)( const gak::STRING &schema );
//---------------------------------------------------------------------------

class TxmlEditorFrame : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
	TTreeView *XmlTreeView;
	TSplitter *Splitter2;
	TPanel *Panel1;
	TStringGrid *AttributeGrid;
	TMemo *ValueMemo;
	TPopupMenu *TreeViewPopupMenu;
	TMenuItem *ChangePopup;
	TMenuItem *N2;
	TMenuItem *DeleteChildrenItem;
	TMenuItem *N1;
	TMenuItem *InsElement;
	TMenuItem *InsTextPCDATA;
	TMenuItem *InsTextCDATA;
	TMenuItem *InsComment;
	TMenuItem *ProcInstruction;
	TMenuItem *DOCTYPE;
	TPopupMenu *AttributePopupMenu;
	TSplitter *Splitter1;
	TStatusBar *StatusBar;
	TMenuItem *insXmlStyleSheet;
	TPopupMenu *ValuePopupMenu;
	TMenuItem *CreateElement;
	TMenuItem *DeleteWithoutChildren;
	TMenuItem *N3;
	TMenuItem *Expand;
	TMenuItem *Collapse;
	TMenuItem *CloneElement;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *ShowStyles;
	void __fastcall XmlTreeViewChange(TObject *Sender, TTreeNode *Node);
	void __fastcall XmlTreeViewEdited(TObject *Sender, TTreeNode *Node,
		  AnsiString &S);
	void __fastcall XmlTreeViewDragOver(TObject *Sender, TObject *Source,
		  int X, int Y, TDragState State, bool &Accept);
	void __fastcall XmlTreeViewDragDrop(TObject *Sender, TObject *Source,
		  int X, int Y);
	void __fastcall ValueMemoChange(TObject *Sender);
	void __fastcall AttributeGridSetEditText(TObject *Sender, int ACol,
		  int ARow, const AnsiString Value);
	void __fastcall AttributeGridKeyUp(TObject *Sender, WORD &Key,
		  TShiftState Shift);
	void __fastcall AttributeGridGetEditText(TObject *Sender, int ACol,
		  int ARow, AnsiString &Value);
	void __fastcall AttributeGridDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State);
	void __fastcall TreeViewPopupClick(TObject *Sender);
	void __fastcall XmlTreeViewContextPopup(TObject *Sender, TPoint &MousePos,
		  bool &Handled);
	void __fastcall ChangePopupClick(TObject *Sender);
	void __fastcall AttributePopupClick(TObject *Sender);
	void __fastcall AttributeGridContextPopup(TObject *Sender,
		  TPoint &MousePos, bool &Handled);
	void __fastcall XmlTreeViewEditing(TObject *Sender, TTreeNode *Node,
		  bool &AllowEdit);
	void __fastcall XmlTreeViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
		  TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
		  bool &PaintImages, bool &DefaultDraw);
	void __fastcall XmlTreeViewKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);
	void __fastcall CreateElementClick(TObject *Sender);
	void __fastcall CollapseClick(TObject *Sender);
	void __fastcall ExpandClick(TObject *Sender);
	void __fastcall ShowStylesClick(TObject *Sender);
private:	// Anwender-Deklarationen
	bool					htmlMode;
	gak::xml::Document		*theDocument;
	gak::STRING				filename, schemaFile;
	gak::STRING				stylesheetFile, stylesheetType;

	gak::DocumentViewer		*viewerInstance;

	static TPlainTextLoad	fileLoader;

	TStyleChanged			styleChangedCB;
	TXmlSchemaChanged		schemaChangedCB;
	XML_MY_SCHEMA_MANAGER	schemaManager;
	gak::css::Rules			*cssRules;

	void addAttributeRow( void )
	{
		int newIdx = AttributeGrid->RowCount;
		AttributeGrid->RowCount++;
		AttributeGrid->Cells[0][newIdx] = "";
		AttributeGrid->Cells[1][newIdx] = "";
	}
	void OpenStream(std::istream *theInput, const gak::STRING &fileName);
	void setStylesheetFile(
		const gak::STRING &stylesheetFile, const gak::STRING &stylesheetType
	)
	{
		this->stylesheetType = stylesheetType;
		if( stylesheetFile.isEmpty() )
			this->stylesheetFile = stylesheetFile;
		else
			this->stylesheetFile = makeFullPath( filename, stylesheetFile );
		if( styleChangedCB )
			styleChangedCB( this->stylesheetFile, stylesheetType );

		theDocument->setCssRules( *cssRules );

		gak::STRING	tmpName = makeFullPath( filename, "editor.css" );

		if( fileLoader )
		{
			gak::STRING css = fileLoader( tmpName );
			theDocument->readCssRules( css, false );
		}
		else if( exists( tmpName ) )
		{
			std::ifstream istream( tmpName );
			theDocument->readCssRules( &istream, false );
		}
		if( stylesheetType == "text/css" )
		{
			if( fileLoader )
			{
				gak::STRING css = fileLoader( stylesheetFile );
				theDocument->readCssRules( css, false );
			}
			else if( exists( stylesheetFile ) )
			{
				std::ifstream istream( stylesheetFile );
				theDocument->readCssRules( &istream, false );
			}
		}
		theDocument->applyCssRules();
	}
	static gak::STRING getTag( gak::xml::Element *xmlElement );
	TTreeNode *addNode( TTreeNode *parentNode, gak::xml::Element *newElement );

	void setChanged( void )
	{
		*viewerInstance = true;
	}
	bool hasChanged( void ) const
	{
		return *viewerInstance;
	}
public:		// Anwender-Deklarationen

	bool refreshValue( gak::xml::Element *theItem=NULL );
	__fastcall TxmlEditorFrame(TComponent* Owner);
	void initShow( void )
	{
		AttributeGrid->ColWidths[0] = AttributeGrid->Width/3;
		AttributeGrid->ColWidths[1] = AttributeGrid->Width - AttributeGrid->ColWidths[0] - 25;
	}
	void initFrame( gak::ChangeManager *manager, gak::css::Rules *cssRules )
	{
		doEnterFunction("TxmlEditorFrame::initViewer");
		if( viewerInstance )
			delete viewerInstance;
		viewerInstance = new XML_TREE_VIEWER( manager, this );
		this->cssRules = cssRules;
	}
	void clear( void );
	void CreateDoc( void );
	void OpenFile( const gak::STRING &fileName )
	{
		std::ifstream theInput((const char *)fileName);

		OpenStream( &theInput, fileName );
	}
	void setText( const gak::STRING &xmlData, const gak::STRING &fileName )
	{
		// filename = "";
		gak::iSTRINGstream	theInput( xmlData );
		OpenStream( &theInput, fileName );
	}
	gak::STRING getText( void )
	{
		return theDocument->generate( htmlMode ? gak::xml::HTML_MODE : gak::xml::XML_MODE );
	}
	gak::STRING getSchemaFile( void ) const
	{
		return schemaFile;
	}
	gak::STRING getStylesheetFile( gak::STRING *stylesheetType ) const
	{
		*stylesheetType = this->stylesheetType;
		return stylesheetFile;
	}
	void SaveFile(const gak::STRING &fileName);
	void buildTree(TTreeNode *parent, gak::xml::Element *xmlElem );
	gak::STRING testDocument( void );
	void refreshSchema( void )
	{
		schemaFile = "";
		if( theDocument )
			schemaFile = schemaManager.refreshDocument( theDocument, true );
		else
			schemaManager.clearValidators();
	}
	void Cut( TWinControl *active );
	void Copy( TWinControl *active );
	void Paste( TWinControl *active );
	gak::xml::Document *getDocument( void ) const
	{
		return theDocument;
	}
	void setDocument(gak::xml::Document *newDocument, bool htmlMode );

	void setStyleChangedCB( TStyleChanged newCB )
	{
		styleChangedCB = newCB;
	}
	void setSchemaChangedCB( TXmlSchemaChanged newCB )
	{
		schemaChangedCB = newCB;
	}
	static void setPlainTextLoader( TPlainTextLoad fileLoader )
	{
		TxmlEditorFrame::fileLoader = fileLoader;
	}
	void selectXmlElement( gak::xml::Element *theElement );
};
//---------------------------------------------------------------------------
extern PACKAGE TxmlEditorFrame *xmlEditorFrame;
//---------------------------------------------------------------------------
#endif
