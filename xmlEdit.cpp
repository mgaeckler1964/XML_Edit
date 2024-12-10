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
	winlib::Application theVclApp;

	doDisableLog();

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
