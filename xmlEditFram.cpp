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

#define STRICT 1

//---------------------------------------------------------------------------
#include <fstream>

#include <vcl.h>
#include <vcl/Clipbrd.hpp>

#include <gak/htmlParser.h>
#include <gak/xmlValidator.h>
#include <gak/sortedArray.h>

#pragma hdrstop

#include "cssStyleFrm.h"
#include "xmlEditFram.h"
#include "XmlViewerFrm.h"
//---------------------------------------------------------------------------
using namespace gak;
using namespace xml;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TxmlEditorFrame *xmlEditorFrame;
//---------------------------------------------------------------------------
TXmlDocLoad					XML_MY_SCHEMA_MANAGER::xmlDocLoadFunction = NULL;
TSchemaFile4NamespaceCB		XML_MY_SCHEMA_MANAGER::schemaFile4NamespaceCB = NULL;
TPlainTextLoad				TxmlEditorFrame::fileLoader = NULL;

//---------------------------------------------------------------------------
class MyStringGrid : public TStringGrid
{
	public:
	TInplaceEdit *GetEditor( void ) const
	{
		return InplaceEditor;
	}
};
//---------------------------------------------------------------------------
Document *XML_MY_SCHEMA_MANAGER::loadSchemaFile( const STRING &schemaFile )
{
	doEnterFunction("XML_MY_SCHEMA_MANAGER::loadSchemaFile");
	doLogValue( schemaFile );

	xml::Document	*theSchemaDoc = NULL;
	if( xmlDocLoadFunction )
		theSchemaDoc = xmlDocLoadFunction( schemaFile );

	if( !theSchemaDoc )
		theSchemaDoc = xml::SchemaManager::loadSchemaFile( schemaFile );

	return theSchemaDoc;
}
//---------------------------------------------------------------------------
STRING XML_MY_SCHEMA_MANAGER::getSchemaFile4Namespace( const STRING &nameSpace )
{
	STRING schemaFile = xml::SchemaManager::getSchemaFile4Namespace( nameSpace );

	if( schemaFile.isEmpty() && schemaFile4NamespaceCB )
		schemaFile = schemaFile4NamespaceCB( nameSpace );

	return schemaFile;
}
//---------------------------------------------------------------------------
__fastcall TxmlEditorFrame::TxmlEditorFrame(TComponent* Owner)
	: TFrame(Owner)
{
	htmlMode = false;
	theDocument = NULL;
	viewerInstance = NULL;
	styleChangedCB = NULL;
	schemaManager.setDefaultPath( Application->ExeName.c_str() );
}
//---------------------------------------------------------------------------
bool TxmlEditorFrame::refreshValue( xml::Element *theItem )
{
	doEnterFunction("TxmlEditorFrame::refreshValue");

	ValueMemo->Lines->Clear();
	TTreeNode *Node = XmlTreeView->Selected;
	if( Node )
	{
		xml::Element	*theElement = static_cast<xml::Element *>(Node->Data);
		if( theElement && (theElement == theItem || theItem == NULL) )
		{
			bool wantValue =
				!dynamic_cast<xml::Document*>(theElement)
				&& (
					theElement->getTag().isEmpty()
					|| dynamic_cast<xml::ProcInstruction *>(theElement)
				)
			;
			STRING value =
				wantValue ?
					theElement->getValue( xml::PLAIN_MODE ) :
					theElement->generate( htmlMode ? xml::HTML_MODE : xml::XML_MODE )
			;

			if( !value.isEmpty() )
			{
				ValueMemo->Tag = 1;
				value.replaceLineEnds( RL_END_CRLF );
				ValueMemo->Text = (const char *)value;
				ValueMemo->Tag = 0;
			}

			ValueMemo->ReadOnly = !wantValue;

			/***/		return wantValue;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
STRING TxmlEditorFrame::getTag( xml::Element *theElem )
{
	STRING	tag;

	if( dynamic_cast<xml::Comment *>(theElem) )
		tag = "<!-- -->";
	else if( dynamic_cast<xml::DocType *>(theElem) )
		tag = "!DOCTYPE";
	else if( dynamic_cast<xml::Mark *>(theElem) )
	{
		tag = "?";
		tag += theElem->getTag();
	}
	else if( dynamic_cast<xml::PCData *>(theElem) )
		tag = "<pcdata>";
	else if( dynamic_cast<xml::CData *>(theElem) )
		tag = "<cdata>";
	else
	{
		tag = theElem->getTag();
		if( tag.isEmpty() )
			tag = "<text>";
	}

	return tag;
}
//---------------------------------------------------------------------------
TTreeNode *TxmlEditorFrame::addNode(
	TTreeNode *parentNode, xml::Element *newElement
)
{
	xml::Element	*theParent = static_cast<xml::Element *>(parentNode->Data);
	if( theParent )
	{
		theParent->addObject( newElement, true );
		STRING	tag = getTag( newElement );
		AnsiString tmp = (const char *)tag;
		TTreeNode *newNode = XmlTreeView->Items->AddChild( parentNode, tmp );
		newNode->Data = newElement;

		return newNode;
	}

	return NULL;
}

//---------------------------------------------------------------------------
void TxmlEditorFrame::buildTree( TTreeNode *parent, xml::Element *xmlElem )
{
	doEnterFunction("TXmlEditForm::buildTree");
	STRING	tag;

	for( size_t i=0; i<xmlElem->getNumObjects(); i++ )
	{
		xml::Element *theElem = xmlElem->getElement( i );
		if( !theElem->getTag().isEmpty()
		|| theElem->getValue( xml::PLAIN_MODE ) != "\n" )
		{
			tag = getTag( theElem );
			AnsiString tmp = (const char *)tag;
			TTreeNode *newNode = XmlTreeView->Items->AddChild( parent, tmp );
			newNode->Data = theElem;

			buildTree( newNode, theElem );
		}
		else
		{
			xmlElem->removeObject( i-- );
		}
	}
}

void TxmlEditorFrame::selectXmlElement( xml::Element *theElement )
{
	doEnterFunction("TxmlEditorFrame::selectXmlElement");
	for( int i=0; i<XmlTreeView->Items->Count; i++ )
	{
		TTreeNode *node = XmlTreeView->Items->Item[i];
		if( node->Data == theElement )
		{
			XmlTreeView->Tag = 1;
			XmlTreeView->Selected = node;
			XmlTreeView->Tag = 0;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TxmlEditorFrame::XmlTreeViewChange(TObject *,
      TTreeNode *Node)
{
	xml::Element	*theElement = static_cast<xml::Element *>(Node->Data);
	if( theElement )
	{
		STRING	attr, value;

		bool wantValue = refreshValue();

		// clear old values
		for( int i=0; i<AttributeGrid->RowCount; i++ )
		{
			AttributeGrid->Cells[0][i] = "";
			AttributeGrid->Cells[1][i] = "";
		}
		if( wantValue )
		{
			AttributeGrid->Visible = false;
			AttributeGrid->Options >> goEditing;
			AttributeGrid->FixedRows = 0;
			AttributeGrid->RowCount = 1;
		}
		else
		{
			AttributeGrid->Visible = true;
			AttributeGrid->Options << goEditing;
			size_t numAttributes = theElement->getNumAttributes();

			AttributeGrid->RowCount = numAttributes+2;
			AttributeGrid->FixedRows = 1;
			AttributeGrid->Row = 1;

			AttributeGrid->Cells[0][0] = "Attribute";
			AttributeGrid->Cells[1][0] = "Value";

			for( size_t i=0; i<numAttributes; i++ )
			{
				attr = theElement->getAttributeName( i );
				value = theElement->getAttribute( i );

				AttributeGrid->Cells[0][i+1] = (const char *)attr;
				AttributeGrid->Cells[1][i+1] = (const char *)value;
			}
			AttributeGrid->Cells[0][numAttributes+1] = "";
			AttributeGrid->Cells[1][numAttributes+1] = "";
		}
		STRING thePath = theElement->getPath();
		StatusBar->SimpleText = (const char *)thePath;

		if( !XmlTreeView->Tag )
			viewerInstance->setPosition( theElement );
	}
	else
		StatusBar->SimpleText = "";
}
//---------------------------------------------------------------------------
void __fastcall TxmlEditorFrame::XmlTreeViewEdited(TObject *,
      TTreeNode *Node, AnsiString &S)
{
	xml::Element *theElement = static_cast<xml::Element*>(Node->Data);
	if( theElement )
	{
		STRING	 	newTag = S.c_str();
		xml::Mark	*xmlMark = dynamic_cast<xml::Mark*>(theElement);

		if( newTag[0U] == '?' )
			newTag += 1U;
		if( !newTag.isEmpty() )
		{
			theElement->setTag( newTag );
			refreshValue();
		}
		newTag = ( xmlMark ) ? "?" : "";
		newTag += theElement->getTag();

		S = (const char *)newTag;

		viewerInstance->setChanged( theElement ); 
	}
}
//---------------------------------------------------------------------------


void __fastcall TxmlEditorFrame::XmlTreeViewDragOver(TObject *,
	  TObject *, int X, int Y, TDragState , bool &Accept)
{
	Accept = false;

	TTreeNode *node = XmlTreeView->GetNodeAt( X, Y );
	if( node && node != XmlTreeView->Selected && XmlTreeView->Selected )
	{
		xml::Element *theTarget = static_cast<xml::Element*>(node->Data);
		if( theTarget )
		{
			xml::Element *theSource = static_cast<xml::Element*>(
				XmlTreeView->Selected->Data
			);
			if( theSource )
			{
				if( theTarget->isValidSubobject( theSource )
				&& !theTarget->getTag().isEmpty()
				&& !dynamic_cast<xml::Mark*>(theTarget)
				&& !dynamic_cast<xml::DocType*>(theSource) )
				{
					Accept = true;
					while( (theTarget = theTarget->getParent()) != NULL )
						if( theTarget == theSource )
						{
							Accept = false;
							break;
						}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::XmlTreeViewDragDrop(TObject *,
	  TObject *, int X, int Y)
{
	TTreeNode *node = XmlTreeView->GetNodeAt( X, Y );
	TTreeNode *Selected = XmlTreeView->Selected;
	if( node && Selected )
	{
		xml::Element *theElement = static_cast<xml::Element*>(Selected->Data);
		xml::Element *newParent = static_cast<xml::Element*>(node->Data);
		if( theElement && newParent )
		{
			Selected->MoveTo( node, naAddChild );
			theElement->remove();
			newParent->addObject( theElement );
			viewerInstance->setChanged( theElement ); 
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::ValueMemoChange(TObject *)
{
	if( !ValueMemo->Tag && !ValueMemo->ReadOnly )
	{
		TTreeNode *Selected = XmlTreeView->Selected;
		if( Selected )
		{
			xml::Element *theElement = static_cast<xml::Element*>(
				Selected->Data
			);
			if( theElement )
			{
				STRING newValue = ValueMemo->Text.c_str();
				newValue.replaceLineEnds( RL_END_LF );
				theElement->setValue( newValue );
				viewerInstance->setChanged( theElement ); 
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::AttributeGridSetEditText(TObject *,
      int ACol, int ARow, const AnsiString Value)
{
	if( !AttributeGrid->Tag )		// do not react on our own changes
	{
		TTreeNode *Selected = XmlTreeView->Selected;
		if( Selected )
		{
			xml::Element *theElement = static_cast<xml::Element*>(
				Selected->Data
			);
			if( theElement )
			{
				STRING tag=theElement->getTag();
				if( !tag.isEmpty() )
				{
					bool	refreshTree = false;
					bool	refreshStyleSheet = false;
					size_t	numAttr = theElement->getNumAttributes();
					size_t	attrIdx = ARow-1U;
					STRING sValue = Value.c_str();
					if( ACol == 0 )		// change of name
					{
						if( !sValue.isEmpty() )
						{
							if( attrIdx >= numAttr )
							{
								addAttributeRow();
								theElement->setStringAttribute( sValue, "" );
							}
							else
							{
								// replaint tree
								// if old name or new name is "name" or "id"
								CI_STRING attrName = theElement->getAttributeName(
									attrIdx
								);
								if( attrName == "href" )
									refreshStyleSheet = true;
								else if( attrName == "name" || attrName == "id" )
									refreshTree = true;
								else
								{
									attrName = sValue;
									if( attrName == "href" )
										refreshStyleSheet = true;
									else if( attrName == "name" || attrName == "id" )
										refreshTree = true;
								}
								theElement->setAttributeName( attrIdx, sValue );
							}
							viewerInstance->setChanged( theElement );
						}
					}
					else
					{
						if( attrIdx >= numAttr && !sValue.isEmpty() )
						{
							AttributeGrid->Cells[0][ARow] = "new";
							theElement->setStringAttribute( "new", sValue );
							addAttributeRow();
							viewerInstance->setChanged( theElement );
						}
						if( attrIdx < numAttr || !sValue.isEmpty() )
						{
							viewerInstance->setChanged( theElement );
							theElement->setStringAttribute( attrIdx, sValue );

							// replaint tree
							// if old name or new name is "name" or "id"
							CI_STRING attrName = theElement->getAttributeName(
								attrIdx
							);
							if( attrName == "href" )
								refreshStyleSheet = true;
							else if( attrName == "name" || attrName == "id" )
								refreshTree = true;
						}
					}

					refreshValue();
					if( refreshTree )
						XmlTreeView->Repaint();

					if( refreshStyleSheet
					&& dynamic_cast<xml::StyleSheet*>( theElement ) )
					{
						setStylesheetFile(
							theElement->getAttribute( "href" ),
							theElement->getAttribute( "type" )
						);
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::AttributeGridKeyUp(TObject *,
      WORD &Key, TShiftState Shift)
{
	if( AttributeGrid->Options.Contains( goEditing ) )
	{
		AttributeGrid->Tag = 1;
		TTreeNode *Selected = XmlTreeView->Selected;
		if( Selected )
		{
			xml::Element *theElement = static_cast<xml::Element *>(
				Selected->Data
			);
			if( theElement )
			{
				STRING tag=theElement->getTag();
				if( !tag.isEmpty() )
				{
					if( Key == VK_INSERT && Shift.Contains( ssCtrl ) )
					{
						size_t numAttributes = theElement->getNumAttributes();
						theElement->setAttributeName( numAttributes, "new" );
						theElement->setStringAttribute( numAttributes, "" );
						numAttributes++;

						AttributeGrid->Cells[0][numAttributes] = (const char *)"new";
						AttributeGrid->Cells[1][numAttributes] = (const char *)"";

						addAttributeRow();
						refreshValue();
						viewerInstance->setChanged( theElement );
					}
					else if( Key == VK_DELETE && Shift.Contains( ssCtrl ) )
					{
						size_t numAttributes = theElement->getNumAttributes();
						size_t attrIdx = AttributeGrid->Row-1;

						if( attrIdx < numAttributes )
						{
							theElement->deleteAttribute( attrIdx );
							for(
								size_t i=AttributeGrid->Row;
								i<(size_t)AttributeGrid->RowCount-1U;
								i++
							)
							{
								AttributeGrid->Cells[0][i] = AttributeGrid->Cells[0][i+1];
								AttributeGrid->Cells[1][i] = AttributeGrid->Cells[1][i+1];
							}
							if( numAttributes )
							{
								if( AttributeGrid->Row >= AttributeGrid->RowCount-1 )
									AttributeGrid->Row--;
								AttributeGrid->RowCount--;
							}
							else
							{
								AttributeGrid->RowCount = 2;
								AttributeGrid->Cells[0][1] = "";
								AttributeGrid->Cells[1][1] = "";
								AttributeGrid->Row = 1;
							}
						}
						refreshValue();
						viewerInstance->setChanged( theElement );
					}
				}
			}
		}
		AttributeGrid->Tag = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::AttributeGridGetEditText(TObject *,
      int ACol, int ARow, AnsiString &Value)
{
	if( !AttributeGrid->Tag )		// do not react on our own changes
	{
		TTreeNode *Selected = XmlTreeView->Selected;
		if( Selected )
		{
			xml::Element *theElement = static_cast<xml::Element*>(
				Selected->Data
			);
			if( theElement )
			{
				STRING tag=theElement->getTag();
				if( !tag.isEmpty() )
				{
					size_t	numAttr = theElement->getNumAttributes();
					size_t	attrIdx = ARow-1U;
					if( attrIdx < numAttr )
					{
						if( ACol == 0 )
							Value = (const char *)theElement->getAttributeName(
								attrIdx
							);
						else
							Value = (const char *)theElement->getAttribute(
								attrIdx
							);
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::AttributeGridDrawCell(TObject *,
	  int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	if( State.Contains(gdSelected) && !AttributeGrid->Focused() )
	{
		AttributeGrid->Canvas->Brush->Color = AttributeGrid->Color;
		AttributeGrid->Canvas->Font->Color = AttributeGrid->Font->Color;
		AttributeGrid->Canvas->FillRect(Rect);
		AttributeGrid->Canvas->TextRect(
			Rect, Rect.Left+2, Rect.Top+2, AttributeGrid->Cells[ACol][ARow]
		);
	}
}

//---------------------------------------------------------------------------
void TxmlEditorFrame::setDocument(xml::Document *newDocument, bool htmlMode )
{
	doEnterFunction("TxmlEditorFrame::setDocument");

	STRING			newStylesheetType, newStylesheetFile;

	schemaFile = "";
	filename = newDocument->getFilename();

	*viewerInstance = false;
	if( theDocument )
	{
		XmlTreeView->Items->Clear();
		delete theDocument;
		theDocument = NULL;
	}

	schemaManager.clearValidators();

	if( newDocument )
	{
		theDocument = newDocument;

		XmlTreeView->Items->BeginUpdate();
		TTreeNode *node = XmlTreeView->Items->Add(
			NULL, static_cast<const char *>(filename)
		);
		node->Data = theDocument;

		buildTree( node, theDocument );

		XmlTreeView->Items->Item[0]->Expand(false);
		XmlTreeView->Items->EndUpdate();

		XmlTreeView->Tag = 1;
		XmlTreeView->Selected = node;
		XmlTreeView->Tag = 0;

		newStylesheetFile = theDocument->getStyleSheet( &newStylesheetType );
		schemaFile = schemaManager.scanDocument( theDocument );
	}

	this->htmlMode = htmlMode;

	setStylesheetFile( newStylesheetFile, newStylesheetType );
	if( schemaChangedCB )
		schemaChangedCB( schemaFile );
}

//---------------------------------------------------------------------------
void TxmlEditorFrame::OpenStream(std::istream *theInput, const STRING &fileName )
{
	doEnterFunction("TxmlEditorFrame::OpenStream");

	xml::Document	*newDocument;
	bool			htmlMode;

	if( fileName.endsWithI( ".html" ) || fileName.endsWithI( ".htm" ) )
	{
		html::Parser htmlParser( theInput, fileName );

		newDocument = htmlParser.readFile( true );
		htmlMode = true;
	}
	else
	{
		xml::Parser xmlParser( theInput, fileName );

		// do not include blanks otherwise empty pcdata will be added
		newDocument = xmlParser.readFile( false );

		htmlMode = false;
	}

	setDocument( newDocument, htmlMode );
}
//---------------------------------------------------------------------------
void TxmlEditorFrame::SaveFile(const STRING &fileName)
{
	this->filename = fileName;
	XmlTreeView->Items->Item[0]->Text = (const char *)fileName;
	*viewerInstance = false;
	std::ofstream fp( fileName );
	if( fp.is_open() )
	{
		STRING xmlCode = theDocument->generate( xml::XML_MODE );
		fp << xmlCode;
	}
}
//---------------------------------------------------------------------------
void TxmlEditorFrame::clear( void )
{
	if( theDocument )
	{
		XmlTreeView->Items->Clear();
		delete theDocument;
		theDocument = NULL;
	}
	*viewerInstance = false;
}
//---------------------------------------------------------------------------
void TxmlEditorFrame::CreateDoc( void )
{
	clear();

	theDocument = new xml::Document("");
	theDocument->addObject( new xml::Declaration() );

	theDocument->addObject( new xml::DocType("root SYSTEM \"root.dtd\"") );
	theDocument->addObject( new xml::Any( "root" ) );

	TTreeNode *node = XmlTreeView->Items->Add( NULL, "untitled" );
	node->Data = theDocument;

	buildTree( node, theDocument );

	XmlTreeView->FullExpand();
	XmlTreeView->Selected = node;
}

//---------------------------------------------------------------------------
void __fastcall TxmlEditorFrame::TreeViewPopupClick(TObject *Sender)
{
	if( !XmlTreeView->IsEditing() )
	{
		TTreeNode *Selected = XmlTreeView->Selected;
		if( Selected )
		{
			xml::Element *theElement = static_cast<xml::Element*>(
				Selected->Data
			);
			if( theElement )
			{
				if( Sender == DeleteChildrenItem )
				{
					xml::XmlContainer *parent = theElement->getParent();
					if( parent
//					&& !dynamic_cast<xml::DocType *>(theElement)
					&& !dynamic_cast<xml::Declaration *>(theElement)
					&& !(
						dynamic_cast<xml::Any *>(theElement)
						&& !dynamic_cast<xml::Mark *>(theElement)
						&& parent == static_cast<xml::XmlContainer*>(
							theDocument
						))
					)
					{
						if( dynamic_cast<xml::StyleSheet*>( theElement ) )
							setStylesheetFile( "", "" );

						XmlTreeView->Items->Delete( Selected );

						parent->removeObject( theElement );
						delete theElement;

						viewerInstance->setDelete( theElement );
					}
				}
				else if( Sender == DeleteWithoutChildren )
				{
					xml::XmlContainer *parent = theElement->getParent();
					if( parent
//					&& !dynamic_cast<xml::DocType *>(theElement)
					&& !dynamic_cast<xml::Declaration*>(theElement)
					&& !(
						dynamic_cast<xml::Any*>(theElement)
						&& !dynamic_cast<xml::Mark*>(theElement)
						&& parent == static_cast<xml::XmlContainer*>(
							theDocument
						))
					)
					{
						if( dynamic_cast<xml::StyleSheet*>( theElement ) )
							setStylesheetFile( "", "" );

						size_t	index = theElement->getIndex();

						for(
							TTreeNode *childNode = Selected->getFirstChild();
							childNode;
							childNode = Selected->getFirstChild()
						)
						{
							childNode->MoveTo( Selected, naInsert );
							xml::Element *childXml = static_cast<xml::Element*>(
								childNode->Data)
							;
							if( childXml )
							{
								theElement->removeObject( childXml );
								parent->addObject( childXml, true );
								childXml->moveTo( index++ );
							}
						}

						XmlTreeView->Items->Delete( Selected );
						parent->removeObject( theElement );
						delete theElement;
						viewerInstance->setDelete( theElement );
					}
				}
				else if( Sender == CloneElement )
				{
					TTreeNode *parentNode = Selected->Parent;
					if( parentNode )
					{
						xml::Element *newElement = theElement->copy();

						TTreeNode *newNode = addNode( parentNode, newElement );
						buildTree( newNode, newElement );
						newNode->Expand( false );
						viewerInstance->setNew( newElement );
					}
				}
				else if( Sender == InsElement )
				{
					STRING tag = theElement->getTag();
					xml::Mark *theMark = dynamic_cast<xml::Mark *>(theElement);
					if( !tag.isEmpty() && !theMark )
					{
						STRING	newTag = theElement->getPrefix();
						if( !newTag.isEmpty() )
							newTag += ':';
						newTag += "new";

						xml::Any *newElement = new xml::Any( newTag );
						TTreeNode *newNode = addNode( Selected, newElement );
						Selected->Expand( false );
						newNode->EditText();
						viewerInstance->setNew( newElement );
					}
				}
				else if( Sender == InsTextPCDATA )
				{
					STRING tag = theElement->getTag();
					xml::Mark *theMark = dynamic_cast<xml::Mark*>(theElement);
					if( !tag.isEmpty() && !theMark  )
					{
						xml::PCData *newElement = new xml::PCData( "" );
						TTreeNode *newNode = addNode( Selected, newElement );
						Selected->Expand( false );
						XmlTreeView->Selected = newNode;
						ValueMemo->SetFocus();
						viewerInstance->setNew( newElement );
					}
				}
				else if( Sender == InsTextCDATA )
				{
					STRING tag = theElement->getTag();
					xml::Mark *theMark = dynamic_cast<xml::Mark*>(theElement);
					if( !tag.isEmpty() && !theMark )
					{
						xml::CData *newElement = new xml::CData( "" );
						TTreeNode *newNode = addNode( Selected, newElement );
						Selected->Expand( false );
						XmlTreeView->Selected = newNode;
						ValueMemo->SetFocus();
						viewerInstance->setNew( newElement );
					}
				}
				else if( Sender == InsComment  )
				{
					STRING tag = theElement->getTag();
					xml::Document *theDoc = dynamic_cast<xml::Document *>(
						theElement
					);
					if( !tag.isEmpty() || theDoc  )
					{
						xml::Comment *newElement = new xml::Comment( "" );
						TTreeNode *newNode = addNode( Selected, newElement );
						Selected->Expand( false );
						XmlTreeView->Selected = newNode;
						ValueMemo->SetFocus();
						viewerInstance->setNew( newElement );
					}
				}
				else if( Sender == ProcInstruction  )
				{
					STRING tag = theElement->getTag();
					xml::Document *theDoc = dynamic_cast<xml::Document *>(
						theElement
					);
					xml::Mark *theMark = dynamic_cast<xml::Mark *>(theElement);
					if( (!tag.isEmpty() && !theMark) || theDoc  )
					{
						xml::ProcInstruction *newElement = new xml::ProcInstruction(
							"new", ""
						);
						TTreeNode *newNode = addNode( Selected, newElement );
						Selected->Expand( false );
						XmlTreeView->Selected = newNode;
						ValueMemo->SetFocus();
						viewerInstance->setNew( newElement );
					}
				}
				else if( Sender == insXmlStyleSheet  )
				{
					if( theDocument  )
					{
						bool found = false;
						for(
							size_t i=0;
							!found && i<theDocument->getNumObjects();
							i++
						)
						{
							if(
								dynamic_cast<xml::StyleSheet*>(
									theDocument->getElement(i)
								)
							)
								found = true;
						}

						if( !found )
						{
							TTreeNode *newNode = XmlTreeView->Items->Insert(
								XmlTreeView->Items->Item[2], "?xml-stylesheet"
							);
							xml::Element *newElem = theDocument->addObject(
								new xml::StyleSheet()
							);
							newNode->Data = newElem;


							newElem->moveTo( 1 );

							Selected->Expand( false );
							XmlTreeView->Selected = newNode;
							ValueMemo->SetFocus();
							viewerInstance->setNew( newElem );
						}
					}
				}
				else if( Sender == DOCTYPE )
				{
					xml::Document *theDoc = dynamic_cast<xml::Document *>(
						theElement
					);
					if( theDoc  )
					{
						TTreeNode *newNode = XmlTreeView->Items->AddChild(
							Selected, "!DOCTYPE"
						);
						xml::Element *newElem = theElement->addObject(
							new xml::DocType( "" )
						);
						newNode->Data = newElem;
						Selected->Expand( false );
						XmlTreeView->Selected = newNode;
						ValueMemo->SetFocus();
						viewerInstance->setNew( newElem );
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::XmlTreeViewContextPopup(TObject *,
      TPoint &MousePos, bool &Handled)
{
	TTreeNode *node = XmlTreeView->GetNodeAt( MousePos.x, MousePos.y );
	if( node )
	{
		if( node != XmlTreeView->Selected )
			XmlTreeView->Selected = node;

		ChangePopup->Clear();
		xml::Element *theElement = static_cast<xml::Element*>(
			node->Data
		);
		if( dynamic_cast<xml::Any*>( theElement )
		|| dynamic_cast<html::HtmlBase*>( theElement ) )
		{
			xml::Validator	*theValidator = NULL;
			ArrayOfStrings	prefixe, tags;

			theElement->getPrefixe( &prefixe );
			for(
				ArrayOfStrings::const_iterator it = prefixe.cbegin(), endIT = prefixe.cend();
				it != endIT;
				++it
			)
			{
				const STRING &prefix = *it;
				TMenuItem *item = new TMenuItem( this );
				if( prefix.isEmpty() )
				{
					item->Hint = ":";
				}
				else
				{
					item->Hint = static_cast<const char *>(prefix);
				}

				item->Caption = static_cast<const char *>(prefix + ':');
				item->OnClick = ChangePopupClick;
				item->Tag = 0;
				ChangePopup->Add( item );
			}

			xml::Element *theParent = theElement->getPrefixParent();
			if( theParent )
			{
				theValidator = theParent->getValidSubobjects( &tags );
			}

			if( !tags.size() )
			{
				if( theValidator )
				{
					theValidator->getValidSubobjects( theParent, &tags );
				}
				else
				{
					theValidator = schemaManager.getValidator4Namespace(
						theElement->getNamespace()
					);
					if( theValidator )
					{
						if( theElement->getParent() == theDocument )
							theValidator->getValidSubobjects( &tags );
						else
							theValidator->getAllValidSubobjects( &tags );
					}
				}
			}

			SortedArray<STRING>	sortedTags;
			sortedTags.addElements( tags );
			STRING	last;
			for(
				SortedArray<STRING>::const_iterator it = sortedTags.cbegin(), endIT = sortedTags.cend();
				it != endIT;
				++it
			)
			{
				const STRING &next = *it;
				if( last != next )
				{
					TMenuItem *item = new TMenuItem( this );
					item->Hint = static_cast<const char *>(next);
					item->Caption = static_cast<const char *>(next);
					item->OnClick = ChangePopupClick;
					item->Tag = 1;
					ChangePopup->Add( item );
					last = next;
				}
			}
		}
		else if( dynamic_cast<xml::PCData*>(theElement) )
		{
			TMenuItem *item = new TMenuItem( this );
			item->Hint = "<cdata>";
			item->Caption = "<cdata>";
			item->OnClick = ChangePopupClick;
			item->Tag = 0;
			ChangePopup->Add( item );

			item = new TMenuItem( this );
			item->Hint = "New";
			item->Caption = "New Tag";
			item->OnClick = ChangePopupClick;
			item->Tag = 1;
			ChangePopup->Add( item );
		}
		else if( dynamic_cast<xml::CData*>(theElement) )
		{
			TMenuItem *item = new TMenuItem( this );
			item->Hint = "<pcdata>";
			item->Caption = "<pcdata>";
			item->OnClick = ChangePopupClick;
			item->Tag = 0;
			ChangePopup->Add( item );

			item = new TMenuItem( this );
			item->Hint = "New";
			item->Caption = "New Tag";
			item->OnClick = ChangePopupClick;
			item->Tag = 1;
			ChangePopup->Add( item );
		}
	}
	Handled = false;
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::ChangePopupClick(TObject *Sender)
{
	TMenuItem *item = dynamic_cast<TMenuItem*>(Sender);
	if( item && item->Hint.Length() )
	{
		TTreeNode *node = XmlTreeView->Selected;
		if( node )
		{
			xml::Element *theElement = static_cast<xml::Element*>(
				node->Data
			);
			if( dynamic_cast<xml::Any*>( theElement )
			||  dynamic_cast<html::HtmlBase*>( theElement ) )
			{
				if( item->Tag == 1 )				// this is a real xml tag
				{
					STRING	newTag = theElement->getPrefix();
					if( !newTag.isEmpty() )
						newTag += ':';
					newTag += item->Hint.c_str();
					if( htmlMode )
					{
						html::Factory	theFactory;
						xml::Element	*newElement = theFactory.createObject(
							newTag
						);
						xml::Element	*theParent = theElement->getParent();
						theParent->replaceObject( theElement, newElement, true );
						delete theElement;
						node->Data = newElement;
						theElement = newElement;
					}
					else
					{
						theElement->setTag( newTag );
					}
					node->Text = (const char *)newTag;
				}
				else if( item->Tag == 0 )			// this is a prefix
				{
					if( dynamic_cast<html::HtmlBase*>( theElement ) )
					{
						STRING newTag = item->Hint.c_str();
						newTag += ':';
						newTag += theElement->getTag();

						xml::Element	*theParent = theElement->getParent();
						xml::Element	*newElement = new xml::Any( newTag );
						theParent->replaceObject( theElement, newElement, true );
						delete theElement;
						node->Data = newElement;
						theElement = newElement;
						node->Text = (const char *)newTag;
					}
					else
					{
						STRING prefix = item->Hint.c_str();
						if( prefix == ":" )
							prefix = "";
						theElement->changeElementPrefix( prefix );
						node->Text = (const char *)theElement->getTag();
					}
				}

				refreshValue();
				*viewerInstance = true;

				xml::Element	*parent = theElement->getPrefixParent();
				if( parent )
				{
					xml::Validator *theValidator = parent->getValidator();
					if( theValidator )
						theValidator->isValidSubobject( parent, theElement );
				}
				else
				{
					STRING nameSpace = theElement->getNamespace();
					schemaManager.scanElement( theElement, filename );
				}
			}
			else if( dynamic_cast<xml::PCData*>(theElement) )
			{
				xml::Element *parent = theElement->getParent();
				if( parent )
				{
					if( item->Tag == 0 )
					{
						xml::CData *newElement = new xml::CData(
							theElement->getValue( xml::PLAIN_MODE )
						);
						parent->replaceObject( theElement, newElement );
						node->Data = (void *)newElement;
						node->Text = "<cdata>";

						delete theElement;
					}
					else if( item->Tag == 1 )
					{
						STRING	newTag = parent->getPrefix();
						if( !newTag.isEmpty() )
							newTag += ':';
						newTag += "new";

						xml::Any *newElement = new xml::Any( newTag );

						parent->replaceObject( theElement, newElement );
						node->Data = (void *)newElement;
						node->Text = (const char *)newTag;

						newElement->addObject( theElement );

						TTreeNode *newNode = XmlTreeView->Items->AddChild(
							node, "<pcdata>"
						);
						newNode->Data = theElement;
						node->Expand( false );
						node->EditText();
						*viewerInstance = true;
					}
				}

			}
			else if( dynamic_cast<xml::CData*>(theElement) )
			{
				xml::Element *parent = theElement->getParent();
				if( parent )
				{
					if( item->Tag == 0 )
					{
						xml::PCData *newElement = new xml::PCData(
							theElement->getValue( xml::PLAIN_MODE )
						);
						parent->replaceObject( theElement, newElement );
						node->Data = (void *)newElement;
						node->Text = "<pcdata>";

						delete theElement;
					}
					else if( item->Tag == 1 )
					{
						STRING	newTag = parent->getPrefix();
						if( !newTag.isEmpty() )
							newTag += ':';
						newTag += "new";

						xml::Any	*newElement = new xml::Any( newTag );

						parent->replaceObject( theElement, newElement );
						node->Data = (void *)newElement;
						node->Text = (const char *)newTag;

						newElement->addObject( theElement );

						TTreeNode *newNode = XmlTreeView->Items->AddChild(
							node, "<cdata>"
						);
						newNode->Data = theElement;
						node->Expand( false );
						node->EditText();
						*viewerInstance = true;
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TxmlEditorFrame::AttributePopupClick(TObject *Sender)
{
	TMenuItem *item = dynamic_cast<TMenuItem*>(Sender);
	if( item && item->Hint.Length() )
	{
		TTreeNode *node = XmlTreeView->Selected;
		if( node )
		{
			xml::Element *theElement = static_cast<xml::Element*>(
				node->Data
			);
			if( theElement )
			{
				size_t attrIdx = AttributeGrid->Row-1;
				size_t numAttrs = theElement->getNumAttributes();
				if( attrIdx >= numAttrs )
				{
					addAttributeRow();
					if( AttributeGrid->Col == 0 )
						theElement->setStringAttribute( item->Hint.c_str(), "" );
					else
					{
						theElement->setStringAttribute( "new", item->Hint.c_str() );
						AttributeGrid->Cells[0][AttributeGrid->Row] = "new";
					}
				}
				else
				{
					if( AttributeGrid->Col == 0 )
						theElement->setAttributeName(
							attrIdx, item->Hint.c_str()
						);
					else if( AttributeGrid->Col == 1 )
						theElement->setStringAttribute(
							attrIdx, item->Hint.c_str()
						);

				}
				AttributeGrid->Cells[AttributeGrid->Col][AttributeGrid->Row] = item->Hint;
				refreshValue();
				viewerInstance->setChanged( theElement );
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::AttributeGridContextPopup(TObject *,
	  TPoint &MousePos, bool &)
{
	int	row, col;
	AttributeGrid->MouseToCell( MousePos.x, MousePos.y, col, row );
	AttributePopupMenu->Items->Clear();
	TTreeNode *node = XmlTreeView->Selected;
	if( node && row )
	{
		xml::Element *theElement = static_cast<xml::Element*>(
			node->Data
		);
		if( theElement )
		{
			ArrayOfStrings	attributes;

			AttributeGrid->Col = col;
			AttributeGrid->Row = row;
			if( col == 0 )
			{
				theElement->getValidAttributes( &attributes );

				STRING	prefix = theElement->getPrefix4Namespace(
					xml::XMLNS_SCHEMA_INSTANCE
				);
				if( !prefix.isEmpty() )
				{
					prefix += ':';
					attributes.addElement( prefix + xml::XMLNS_SCHEMALOCATION );
					attributes.addElement( prefix + xml::XMLNS_NS_SCHEMALOCATION );
				}

				attributes.addElement( xml::XMLNS_ATTRIBUTE );
			}
			else if( col == 1 )
			{
				STRING attrName = theElement->getAttributeName( row-1 );
				if( attrName == xml::XMLNS_ATTRIBUTE
				|| attrName.beginsWith( STRING(xml::XMLNS_ATTRIBUTE) + ':' ) )
				{
					attributes.addElement( xml::XMLNS_SCHEMA );
					attributes.addElement( xml::XMLNS_SCHEMA_INSTANCE );
					attributes.addElement( xml::XMLNS_XSLT );
				}
				else
				{
					theElement->getValidAttributeValues( row-1, &attributes );
				}

			}

			SortedArray<STRING>	sortedAttributes;
			sortedAttributes.addElements( attributes );

			STRING	last;
			for(
				SortedArray<STRING>::const_iterator it = sortedAttributes.cbegin(),
					endIT = sortedAttributes.cend();
				it != endIT;
				++it
			)
			{
				const STRING	&next = *it;
				if( next != last )
				{
					TMenuItem *item = new TMenuItem( this );
					item->Hint = static_cast<const char *>(next);
					item->Caption = static_cast<const char *>(next);
					item->OnClick = AttributePopupClick;
					item->Tag = col;
					AttributePopupMenu->Items->Add( item );
					last = next;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
STRING TxmlEditorFrame::testDocument( void )
{
	STRING	errorText;

	if( theDocument )
	{
		bool result = htmlMode
			? schemaManager.testHTML( theDocument )
			: schemaManager.testXML( theDocument )
		;
		if( !result )
		{
			xml::Element	*errorElement = schemaManager.getErrorElement();
			errorText = schemaManager.getErrorText();
			for( int j=0; j<XmlTreeView->Items->Count; j++ )
			{
				if(XmlTreeView->Items->Item[j]->Data == errorElement )
				{
					XmlTreeView->Selected = XmlTreeView->Items->Item[j];
/*v*/				break;
				}
			}
		}
	}

	return errorText;
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::XmlTreeViewEditing(TObject *,
	  TTreeNode *Node, bool &AllowEdit)
{
	AllowEdit = false;
	xml::Element *theElement = static_cast<xml::Element*>(
		Node->Data
	);
	if( theElement )
	{
		xml::Special	*xmlSpecial = dynamic_cast<xml::Special*>(theElement);
		STRING tag = theElement->getTag();
		if( !tag.isEmpty() && !xmlSpecial )
		{
			AllowEdit = true;
			Node->Text = (const char *)tag;
		}
	}
}
//---------------------------------------------------------------------------
void TxmlEditorFrame::Cut( TWinControl *active )
{
	if( active == ValueMemo )
	{
		ValueMemo->CutToClipboard();
		*viewerInstance = true;
	}
	else if( active == AttributeGrid )
	{
		MyStringGrid	*tmp = static_cast<MyStringGrid *>(AttributeGrid);
		TInplaceEdit	*editor = tmp->GetEditor();
		if( editor )
		{
			editor->CutToClipboard();
			*viewerInstance = true;
		}
	}
	else if( active == XmlTreeView )
	{
		Copy( active );
		TreeViewPopupClick( DeleteChildrenItem );
		*viewerInstance = true;
	}
}
//---------------------------------------------------------------------------

void TxmlEditorFrame::Copy( TWinControl *active )
{
	if( active == ValueMemo )
	{
		ValueMemo->CopyToClipboard();
	}
	else if( active == AttributeGrid )
	{
		MyStringGrid	*tmp = static_cast<MyStringGrid *>(AttributeGrid);
		TInplaceEdit	*editor = tmp->GetEditor();
		if( editor )
			editor->CopyToClipboard();
	}
	else if( active == XmlTreeView )
	{
		TTreeNode *node = XmlTreeView->Selected;
		if( node )
		{
			xml::Element *element = static_cast<xml::Element*>(
				node->Data
			);
			if( element )
			{
				STRING data = element->generate( xml::XML_MODE );
				TClipboard  *clip = Clipboard();
				clip->AsText = (const char *)data;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TxmlEditorFrame::Paste( TWinControl *active )
{
	if( active == ValueMemo )
	{
		ValueMemo->PasteFromClipboard();
		*viewerInstance = true;
	}
	else if( active == AttributeGrid )
	{
		MyStringGrid	*tmp = static_cast<MyStringGrid *>(AttributeGrid);
		TInplaceEdit	*editor = tmp->GetEditor();
		if( editor )
		{
			editor->PasteFromClipboard();
			*viewerInstance = true;
		}
	}
	else if( active == XmlTreeView )
	{
		TTreeNode *node = XmlTreeView->Selected;
		if( node )
		{
			xml::Element *element = static_cast<xml::Element *>(
				node->Data
			);
			if( element )
			{
				TClipboard  *clip = Clipboard();
				if( clip->HasFormat( CF_TEXT ) )
				{
					STRING 			data = clip->AsText.c_str();
					iSTRINGstream	theInputStream( data );
					xml::Parser		theXmlParser( &theInputStream, "" );

					xml::Document	*theDoc = theXmlParser.readFile( true );

					if( theDoc )
					{
						xml::Element *theData = theDoc->getRoot();
						if( theData )
						{
							buildTree( node, theDoc );
							theData->remove();
							element->addObject( theData );
							node->Expand(false);
							*viewerInstance = true;
						}
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TxmlEditorFrame::XmlTreeViewAdvancedCustomDrawItem(
	  TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState ,
	  TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
{
	PaintImages = DefaultDraw = true;
	if( Stage == cdPostPaint && Node && Sender )
	{
		xml::Element	*theElement = static_cast<xml::Element *>(
			Node->Data
		);
		if( theElement )
		{
			STRING id = theElement->getAttribute( "ID" );
			if( id.isEmpty() )
				id = theElement->getAttribute( "NAME" );
			if( id.isEmpty() )
				id = theElement->getAttribute( "MATCH" );
			if( id.isEmpty() )
				id = theElement->getAttribute( "SELECT" );

			if( !id.isEmpty() )
			{
				STRING newTag = " (";
				newTag += id;
				newTag += ')';
				TRect NodeRect = Node->DisplayRect(true);
				Sender->Canvas->Font->Style = TFontStyles() << fsItalic;
				Sender->Canvas->TextOut(NodeRect.Right, NodeRect.Top, (const char *)newTag);
			}
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TxmlEditorFrame::XmlTreeViewKeyDown(TObject *,
      WORD &Key, TShiftState Shift)
{
	if( !XmlTreeView->IsEditing() )
	{
		TTreeNode *Selected = XmlTreeView->Selected;
		if( Selected )
		{
			xml::Element *theElement = static_cast<xml::Element*>(
				Selected->Data
			);
			if( theElement && Shift.Contains(ssCtrl) )
			{
				if( Key == VK_UP )
				{
					TTreeNode *sibling = Selected->getPrevSibling();
					if( sibling )
					{
						Selected->MoveTo( sibling, naInsert );
						theElement->moveTo( theElement->getIndex()-1 );
						*viewerInstance = true;
						Key = 0;
					}
				}
				else if( Key == VK_DOWN )
				{
					TTreeNode *sibling = Selected->getNextSibling();
					if( sibling )
					{
						TTreeNode *sibling2 = sibling->getNextSibling();
						if( sibling2 )
							Selected->MoveTo( sibling2, naInsert );
						else
							Selected->MoveTo( sibling, naAdd );
						theElement->moveTo( theElement->getIndex()+1 );
						*viewerInstance = true;
						Key = 0;
					}
				}
				else if( Key == 'C' )
				{
					Copy( XmlTreeView );
					Key = 0;
				}
				else if( Key == 'X' )
				{
					Cut( XmlTreeView );
					Key = 0;
				}
				else if( Key == 'V' )
				{
					Paste( XmlTreeView );
					Key = 0;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TxmlEditorFrame::CreateElementClick(TObject *)
{
	TTreeNode *Selected = XmlTreeView->Selected;
	if( Selected )
	{
		xml::Element	*theElement = static_cast<xml::Element *>(Selected->Data);
		bool			createPCdata = dynamic_cast<xml::PCData*>(theElement);
		bool			createCdata = dynamic_cast<xml::CData*>(theElement);
		if( theElement && (createPCdata || createCdata) )
		{
			TTreeNode	*newSelected = Selected->Parent;
			if( newSelected )
			{
				TTreeNode	*newItemNode;
				xml::Element*newItem, *textItem;
				STRING		text = ValueMemo->Text.c_str();
				STRING		left = text.leftString( ValueMemo->SelStart );
				STRING		middle = text.subString(
					ValueMemo->SelStart, ValueMemo->SelLength
				);
				STRING		right = text.subString(
					ValueMemo->SelStart + ValueMemo->SelLength
				);

				if( !left.isEmpty() )
				{
					if( createCdata )
						newItem = new xml::CData( left );
					else
						newItem = new xml::PCData( left );

					addNode( newSelected, newItem );
				}
				newItem = new xml::Any( "new" );
				newItemNode = addNode( newSelected, newItem );

				if( !middle.isEmpty() )
				{
					if( createCdata )
						textItem = new xml::CData( middle );
					else
						textItem = new xml::PCData( middle );

					addNode( newItemNode, textItem );
				}
				if( !right.isEmpty() )
				{
					if( createCdata )
						newItem = new xml::CData( right );
					else
						newItem = new xml::PCData( right );

					addNode( newSelected, newItem );
				}

				XmlTreeView->Selected = newSelected;
				XmlTreeView->Items->Delete( Selected );
				theElement->remove();
				delete theElement;
				*viewerInstance = true;
				newSelected->Expand( true );
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::CollapseClick(TObject *)
{
	TTreeNode *Selected = XmlTreeView->Selected;
	if( Selected )
	{
		Selected->Collapse( true );
	}
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::ExpandClick(TObject *)
{
	TTreeNode *Selected = XmlTreeView->Selected;
	if( Selected )
	{
		Selected->Expand( true );
	}
}
//---------------------------------------------------------------------------
void XML_TREE_VIEWER::handlePositionChange( void *, void *position )
{
	theViewer->selectXmlElement( static_cast<xml::Element*>(position) );
}
//---------------------------------------------------------------------------
void XML_TREE_VIEWER::handleChange( void *, void *item )
{
	if( item )
		theViewer->refreshValue( static_cast<xml::Element*>(item) );
}
//---------------------------------------------------------------------------
void XML_TREE_VIEWER::handleNew( void *, void *item )
{
	if( item )
		theViewer->refreshValue( static_cast<xml::Element*>(item) );
}
//---------------------------------------------------------------------------
void XML_TREE_VIEWER::handleDelete( void *, void *item )
{
	if( item )
		theViewer->refreshValue( static_cast<xml::Element*>(item) );
}
//---------------------------------------------------------------------------

void __fastcall TxmlEditorFrame::ShowStylesClick(TObject *)
{
	TTreeNode *Selected = XmlTreeView->Selected;
	if( Selected )
	{
		xml::Element *theElement = static_cast<xml::Element*>(Selected->Data);
		if( theElement )
		{
			cssStyleForm->setCssStyle( theElement->getCssStyle() );
			cssStyleForm->ShowModal();
		}
	}
}
//---------------------------------------------------------------------------

