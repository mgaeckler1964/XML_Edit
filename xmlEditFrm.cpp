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
#include <strstrea.h>

#include <vcl.h>

#include <gak/htmlParser.h>
#include <gak/xmlValidator.h>
#include <gak/xslt.h>
#include <winlib/f_type.h>

#pragma hdrstop

#include "xmlEditFrm.h"
#include "XmlViewerFrm.h"
//---------------------------------------------------------------------------
using namespace gak;
using namespace xml;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "xmlEditFram"
#pragma link "xmlEditFram"
#pragma link "xmlEditFram"
#pragma resource "*.dfm"
TXmlEditForm *XmlEditForm;
//---------------------------------------------------------------------------
__fastcall TXmlEditForm::TXmlEditForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TXmlEditForm::styleChangedCB( const STRING &styleSheetFile, const STRING &styleSheetType )
{
	MenuTransform->Enabled = !styleSheetFile.isEmpty() && styleSheetType == "text/xsl";
}
//---------------------------------------------------------------------------
void TXmlEditForm::schemaChangedCB( const STRING &schemaFile )
{
	MenuSchemaOpen->Enabled = !schemaFile.isEmpty();
}
//---------------------------------------------------------------------------
void TXmlEditForm::OpenFile( const STRING &fileName )
{
	this->filename = fileName;
	xmlEditorFrame->OpenFile( filename );
	STRING nCaption = "XML Editor ";
	nCaption += fileName;
	Caption = (const char *)nCaption;
	Application->Title = (const char *)nCaption;
	MenuFileSave->Enabled = true;
	MenuFileSaveAs->Enabled = true;
}

//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::FormShow(TObject *)
{
	winlib::FileTypeRegistry	fType;

	getFileType( "xml", "edit", &fType );
	if( fType.type.isEmpty() )
		fType.type = "xml_file";
	if( fType.type_description.isEmpty() )
		fType.type = "XML File";

	fType.cmd = "edit";
	fType.cmd_description = "Edit &XML";
	fType.commandLine = '"';
	fType.commandLine += Application->ExeName.c_str();
	fType.commandLine += "\" \"%1\"";
	fType.ddeCommand = "";
	fType.ddeIfExec = "";
	fType.application = "";
	fType.topic = "";
	addFileType( &fType );

	getFileType( "xsl", "edit", &fType );
	if( fType.type.isEmpty() )
		fType.type = "xsl_file";
	if( fType.type_description.isEmpty() )
		fType.type = "XSL Stylesheet";

	fType.cmd = "edit";
	fType.cmd_description = "Edit &XML";
	fType.commandLine = '"';
	fType.commandLine += Application->ExeName.c_str();
	fType.commandLine += "\" \"%1\"";
	fType.ddeCommand = "";
	fType.ddeIfExec = "";
	fType.application = "";
	fType.topic = "";
	addFileType( &fType );

	getFileType( "xsd", "edit", &fType );
	if( fType.type.isEmpty() )
		fType.type = "xsd_file";
	if( fType.type_description.isEmpty() )
		fType.type = "XML Schema";

	fType.cmd = "edit";
	fType.cmd_description = "Edit &XML";
	fType.commandLine = '"';
	fType.commandLine += Application->ExeName.c_str();
	fType.commandLine += "\" \"%1\"";
	fType.ddeCommand = "";
	fType.ddeIfExec = "";
	fType.application = "";
	fType.topic = "";
	addFileType( &fType );

	xmlEditorFrame->initShow();

	if( Application->Tag )
	{
		STRING fileName = (const char *)Application->Tag;
		if( fileName[0] == '"' )
		{
			fileName += 1U;
			fileName.cut( fileName.strlen() -1 );
		}
		if( !fileName.isEmpty() )
			OpenFile( fileName );
	}
}
//---------------------------------------------------------------------------
void __fastcall TXmlEditForm::Open1Click(TObject *Sender)
{
	if( hasChanged() )
	{
		switch(
			Application->MessageBox(
				"Data changed. Want to save?", "Warning",
				MB_YESNOCANCEL|MB_ICONQUESTION
			)
		)
		{
			case IDYES:
				MenuFileSaveClick( Sender );
				if( !hasChanged() )
/*v*/				break;
			case IDCANCEL:
/***/			return;
		}
	}
	if( OpenDialog->Execute() )
	{
		OpenFile( OpenDialog->FileName.c_str() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::Exit1Click(TObject *Sender)
{
	if( hasChanged() )
	{
		switch(
			Application->MessageBox(
				"Data changed. Want to save?", "Warning",
				MB_YESNOCANCEL|MB_ICONQUESTION
			)
		)
		{
			case IDYES:
				MenuFileSaveClick( Sender );
				if( !hasChanged() )
/*v*/				break;
			case IDCANCEL:
/***/			return;
		}
	}
	Close();
	Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::New1Click(TObject *Sender)
{
	if( hasChanged() )
	{
		switch(
			Application->MessageBox(
				"Data changed. Want to save?", "Warning",
				MB_YESNOCANCEL|MB_ICONQUESTION
			)
		)
		{
			case IDYES:
				MenuFileSaveClick( Sender );
				if( !hasChanged() )
/*v*/				break;
			case IDCANCEL:
/***/			return;
		}
	}
	filename = "";
	xmlEditorFrame->CreateDoc();
	MenuFileSave->Enabled = false;
	MenuFileSaveAs->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::MenuFileSaveAsClick(TObject *Sender)
{
	if( SaveDialog->Execute() )
	{
		filename = SaveDialog->FileName.c_str();

		MenuFileSaveClick( Sender );
	}
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::MenuFileSaveClick(TObject *Sender)
{
	if( filename.isEmpty() )
		MenuFileSaveAsClick( Sender );
	else
	{
		xmlEditorFrame->SaveFile( filename );
	}
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::FormCloseQuery(TObject *Sender,
	  bool &CanClose)
{
	if( hasChanged() )
	{
		switch(
			Application->MessageBox(
				"Data changed. Want to save?", "Warning",
				MB_YESNOCANCEL|MB_ICONQUESTION
			)
		)
		{
			case IDYES:
				MenuFileSaveClick( Sender );
				if( !hasChanged() )
/*v*/				break;
			case IDCANCEL:
				CanClose = false;
/***/			return;
		}
	}
	CanClose = true;
}
//---------------------------------------------------------------------------
void __fastcall TXmlEditForm::FormDestroy(TObject *)
{
/*
	doEnterFunction("TXmlEditForm::FormDestroy");
	if( theDocument )
	{
		delete theDocument;
		theDocument = NULL;
	}
*/
}
//---------------------------------------------------------------------------



void __fastcall TXmlEditForm::Cut1Click(TObject *)
{
	xmlEditorFrame->Cut( ActiveControl );
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::Copy1Click(TObject *)
{
	xmlEditorFrame->Copy( ActiveControl );
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::Paste1Click(TObject *)
{
	xmlEditorFrame->Paste( ActiveControl );
}
//---------------------------------------------------------------------------
void __fastcall TXmlEditForm::MenuSchemaTestDocumentClick(TObject *)
{
	STRING		errorText = xmlEditorFrame->testDocument();
	if( !errorText.isEmpty() )
	{
		Application->MessageBox( errorText, "Error", MB_OK|MB_ICONERROR );
	}
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::MenuSchemaOpenClick(TObject *)
{
	STRING	schemaFile = xmlEditorFrame->getSchemaFile();
	if( !schemaFile.isEmpty() )
	{
		STRING	cmd = Application->ExeName.c_str();
		cmd += " \"";
		cmd += schemaFile;
		cmd += '\"';
		WinExec( cmd, SW_SHOW );
	}
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::OpenStylesheet1Click(TObject *)
{
	STRING	type;
	STRING	stylesheetFile = xmlEditorFrame->getStylesheetFile( &type );
	if( !stylesheetFile.isEmpty() && type=="text/xsl" )
	{
		STRING	cmd = Application->ExeName.c_str();
		cmd += " \"";
		cmd += stylesheetFile;
		cmd += '\"';
		WinExec( cmd, SW_SHOW );
	}
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::Transform2Click(TObject *)
{
	STRING	type;
	STRING	stylesheetFile = xmlEditorFrame->getStylesheetFile( &type );
	if( !stylesheetFile.isEmpty() && type=="text/xsl" )
	{
		Transformator	theXslProcessor(
			xmlEditorFrame->getDocument(), stylesheetFile
		);
		Document	*theResultDoc = theXslProcessor.transform();
		if( theResultDoc )
		{
			STRING	result = theResultDoc->generateDoc();

			if( !result.isEmpty() )
			{
				html::Document	*htmlDoc = dynamic_cast<html::Document*>(
					theResultDoc
				);
				STRING	resultFile = getenv( "TEMP" );
				resultFile += htmlDoc ? "\\result.html" : "\\result.xml";

				result.writeToFile( resultFile );

				if( htmlDoc )
					ShellExecute(
						NULL, "open", resultFile, NULL, NULL, SW_SHOWNORMAL
					); 
				else
				{
					STRING	cmd = Application->ExeName.c_str();
					cmd += " \"";
					cmd += resultFile;
					cmd += '\"';
					WinExec( cmd, SW_SHOW );
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::FormCreate(TObject *)
{
	doEnterFunction("TXmlEditForm::FormCreate");
	STRING	cssFilename = makeFullPath( Application->ExeName.c_str(), "editor.css" );
	if( exists( cssFilename ) )
	{
		ifstream	istream( cssFilename );
		editorCssRules.readCssFile( &istream, false );
	}
	xmlEditorFrame->initFrame( &theManager, &editorCssRules );
	xmlEditorFrame->setStyleChangedCB( styleChangedCB );
	xmlEditorFrame->setSchemaChangedCB( schemaChangedCB );
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::MenuSchemaRefreshSchemaClick(TObject *)
{
	xmlEditorFrame->refreshSchema();	
}
//---------------------------------------------------------------------------

void __fastcall TXmlEditForm::Renderer1Click(TObject *)
{
	Document *theDocument = xmlEditorFrame->getDocument();

	if( theDocument )
	{
		XmlViewerForm->setDocument( theDocument, &theManager );
		XmlViewerForm->Show();
	}
}
//---------------------------------------------------------------------------


