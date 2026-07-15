/*
		Project:		XML Editor
		Module:			xmlEditFram.h
		Description:	The xml editor frame with tree view, attribute view and
						text view
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2010-2026 Martin Gäckler

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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


#ifndef xmlEditFramH
#define xmlEditFramH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include <memory>

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
#include <gak/shared.h>

//---------------------------------------------------------------------------
class TxmlEditorFrame;
//---------------------------------------------------------------------------
#pragma option -RT-

typedef gak::xml::Document * (*TXmlDocLoad)( const gak::STRING &pathName );
typedef gak::STRING (*TPlainTextLoad)( const gak::STRING &pathName );
typedef gak::STRING (*TSchemaFile4NamespaceCB)( const gak::STRING &nameSpace );

class XmlMySchemaManager : public gak::xml::SchemaManager
{
	static TXmlDocLoad					s_xmlDocLoadFunction;
	static TSchemaFile4NamespaceCB		s_schemaFile4NamespaceCB;

	public:
//	XmlMySchemaManager() {}		/// TODO check why?
	virtual gak::STRING getSchemaFile4Namespace( const gak::STRING &nameSpace );
	virtual gak::xml::Document *loadSchemaFile( const gak::STRING &schemaFile );

	static void setXmlDocLoadFunction( TXmlDocLoad newFunction )
	{
		s_xmlDocLoadFunction = newFunction;
	}
	static void setSchemaFile4NamespaceCB( TSchemaFile4NamespaceCB newCB )
	{
		s_schemaFile4NamespaceCB = newCB;
	}
};

class XmlTreeViewer : public gak::DocumentViewer
{
	TxmlEditorFrame	*m_theViewer;

	public:
	XmlTreeViewer( gak::ChangeManager *manager, TxmlEditorFrame *theViewer)
	: gak::DocumentViewer( manager )
	{
		m_theViewer = theViewer;
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
typedef gak::XSharedPointer<gak::xml::Document> XmlDocPtr;

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
	bool					m_htmlMode;
	XmlDocPtr				m_theDocument;
	gak::STRING				m_filename, m_schemaFile;
	gak::STRING				m_stylesheetFile, m_stylesheetType;

	std::unique_ptr<gak::DocumentViewer>	m_viewerInstance;

	static TPlainTextLoad	s_fileLoader;

	TStyleChanged			m_styleChangedCB;
	TXmlSchemaChanged		m_schemaChangedCB;
	XmlMySchemaManager		m_schemaManager;
	gak::css::Rules			*m_cssRules;

	void addAttributeRow()
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
		m_stylesheetType = stylesheetType;
		if( stylesheetFile.isEmpty() )
			m_stylesheetFile = stylesheetFile;
		else
			m_stylesheetFile = makeFullPath( m_filename, stylesheetFile );
		if( m_styleChangedCB )
			m_styleChangedCB( m_stylesheetFile, stylesheetType );

		m_theDocument->setCssRules( *m_cssRules );

		gak::STRING	tmpName = makeFullPath( m_filename, "editor.css" );

		if( s_fileLoader )
		{
			gak::STRING css = s_fileLoader( tmpName );
			m_theDocument->readCssRules( css, false );
		}
		else if( exists( tmpName ) )
		{
			std::ifstream istream( tmpName );
			m_theDocument->readCssRules( &istream, false );
		}
		if( stylesheetType == "text/css" )
		{
			if( s_fileLoader )
			{
				gak::STRING css = s_fileLoader( stylesheetFile );
				m_theDocument->readCssRules( css, false );
			}
			else if( exists( stylesheetFile ) )
			{
				std::ifstream istream( stylesheetFile );
				m_theDocument->readCssRules( &istream, false );
			}
		}
		m_theDocument->applyCssRules();
	}
	static gak::STRING getTag( gak::xml::Element *xmlElement );
	TTreeNode *addNode( TTreeNode *parentNode, gak::xml::Element *newElement );

	void setChanged()
	{
		*m_viewerInstance = true;
	}
	bool hasChanged() const
	{
		return *m_viewerInstance;
	}
public:		// Anwender-Deklarationen

	bool refreshValue( gak::xml::Element *theItem=nullptr );
	__fastcall TxmlEditorFrame(TComponent* Owner);
	void initShow()
	{
		AttributeGrid->ColWidths[0] = AttributeGrid->Width/3;
		AttributeGrid->ColWidths[1] = AttributeGrid->Width - AttributeGrid->ColWidths[0] - 25;
	}
	void initFrame( gak::ChangeManager *manager, gak::css::Rules *cssRules )
	{
		doEnterFunction("TxmlEditorFrame::initViewer");
		m_viewerInstance.reset( new XmlTreeViewer( manager, this ) );
		m_cssRules = cssRules;
	}
	void clear();
	void CreateDoc();
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
	gak::STRING getText()
	{
		return m_theDocument->generate( m_htmlMode ? gak::xml::HTML_MODE : gak::xml::XML_MODE );
	}
	const gak::STRING &getSchemaFile() const
	{
		return m_schemaFile;
	}
	const gak::STRING &getStylesheetFile( gak::STRING *stylesheetType ) const
	{
		*stylesheetType = m_stylesheetType;
		return m_stylesheetFile;
	}
	void SaveFile(const gak::STRING &fileName);
	void buildTree(TTreeNode *parent, gak::xml::Element *xmlElem );
	gak::STRING testDocument();
	void refreshSchema()
	{
		m_schemaFile.release();
		if( m_theDocument )
			m_schemaFile = m_schemaManager.refreshDocument( m_theDocument, true );
		else
			m_schemaManager.clearValidators();
	}
	void Cut( TWinControl *active );
	void Copy( TWinControl *active );
	void Paste( TWinControl *active );
	const XmlDocPtr &getDocument() const
	{
		return m_theDocument;
	}
	void setDocument(const XmlDocPtr &newDocument, bool htmlMode );

	void setStyleChangedCB( TStyleChanged newCB )
	{
		m_styleChangedCB = newCB;
	}
	void setSchemaChangedCB( TXmlSchemaChanged newCB )
	{
		m_schemaChangedCB = newCB;
	}
	static void setPlainTextLoader( TPlainTextLoad fileLoader )
	{
		TxmlEditorFrame::s_fileLoader = fileLoader;
	}
	void selectXmlElement( gak::xml::Element *theElement );
};
//---------------------------------------------------------------------------
extern PACKAGE TxmlEditorFrame *xmlEditorFrame;
//---------------------------------------------------------------------------
#endif
