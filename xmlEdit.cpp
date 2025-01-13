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

#include <winlib/winapp.h>
#include <gak/logfile.h>

#include <vcl.h>
#pragma hdrstop
USERES("xmlEdit.res");
USEFORM("xmlEditFrm.cpp", XmlEditForm);
USEFORM("xmlEditFram.cpp", xmlEditorFrame); /* TFrame: File Type */
USEFORM("XmlViewerFrm.cpp", XmlViewerForm);
USELIB("..\..\Object\winlib\winlibBCB.lib");
USELIB("..\..\Object\gaklib\gaklib_bcb.lib");
USEFORM("cssStyleFram.cpp", cssStyleFrame); /* TFrame: File Type */
USEFORM("cssStyleFrm.cpp", cssStyleForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE , LPSTR cmdLine, int)
{
	doEnableLogEx(gakLogging::llInfo);

	winlib::Application theVclApp;

	theVclApp.setInstance( hInstance );

	try
	{
		Application->Initialize();
		Application->Tag = (int)cmdLine;
		Application->Title = "XML Editor";
		Application->CreateForm(__classid(TXmlEditForm), &XmlEditForm);
		Application->CreateForm(__classid(TXmlViewerForm), &XmlViewerForm);
		Application->CreateForm(__classid(TcssStyleForm), &cssStyleForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
