object XmlEditForm: TXmlEditForm
  Left = 730
  Top = 463
  Width = 782
  Height = 541
  Caption = 'XML Editor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inline xmlEditorFrame: TxmlEditorFrame
    Width = 774
    Height = 495
    Align = alClient
    inherited Splitter2: TSplitter
      Height = 476
    end
    inherited XmlTreeView: TTreeView
      Height = 476
    end
    inherited Panel1: TPanel
      Width = 552
      Height = 476
      inherited Splitter1: TSplitter
        Width = 550
      end
      inherited AttributeGrid: TStringGrid
        Width = 550
      end
      inherited ValueMemo: TMemo
        Width = 550
        Height = 285
      end
    end
    inherited StatusBar: TStatusBar
      Top = 476
      Width = 774
    end
  end
  object OpenDialog: TOpenDialog
    Filter = 
      'All XML|*.xml;*.xsd;*.xsl;*.xhtml|XML Files (*.xml)|*.xml|XML Sc' +
      'hema (*.xsd)|*.xsd|XSL Files (*.xsl)|*.xsl|XHTML Files (*.xhtml)' +
      '|*.xhtml|HTML-Files (*.htm;*.html)|*.htm;*.html|VSTGUI Descripti' +
      'on (*.uidesc)|*.uidesc|All Files|*.*'
    Title = 'Open XML File'
    Left = 328
    Top = 184
  end
  object MainMenu: TMainMenu
    Left = 136
    Top = 184
    object File1: TMenuItem
      Caption = '&File'
      object New1: TMenuItem
        Caption = '&New'
        OnClick = New1Click
      end
      object Open1: TMenuItem
        Caption = '&Open'
        OnClick = Open1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object MenuFileSave: TMenuItem
        Caption = '&Save'
        Enabled = False
        OnClick = MenuFileSaveClick
      end
      object MenuFileSaveAs: TMenuItem
        Caption = 'Save &As...'
        Enabled = False
        OnClick = MenuFileSaveAsClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = '&Exit'
        OnClick = Exit1Click
      end
    end
    object Edit1: TMenuItem
      Caption = '&Edit'
      object Cut1: TMenuItem
        Caption = 'C&ut'
        ShortCut = 16472
        OnClick = Cut1Click
      end
      object Copy1: TMenuItem
        Caption = '&Copy'
        ShortCut = 16451
        OnClick = Copy1Click
      end
      object Paste1: TMenuItem
        Caption = '&Paste'
        ShortCut = 16470
        OnClick = Paste1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Renderer1: TMenuItem
        Caption = '&Renderer'
        OnClick = Renderer1Click
      end
    end
    object MenuSchema: TMenuItem
      Caption = '&Schema'
      object MenuSchemaTestDocument: TMenuItem
        Caption = '&Test Document'
        OnClick = MenuSchemaTestDocumentClick
      end
      object MenuSchemaOpen: TMenuItem
        Caption = '&Open Schema'
        Enabled = False
        OnClick = MenuSchemaOpenClick
      end
      object MenuSchemaRefreshSchema: TMenuItem
        Caption = '&Refresh Schema'
        OnClick = MenuSchemaRefreshSchemaClick
      end
    end
    object MenuTransform: TMenuItem
      Caption = '&Transform'
      Enabled = False
      object Transform2: TMenuItem
        Caption = '&Transform'
        OnClick = Transform2Click
      end
      object OpenStylesheet1: TMenuItem
        Caption = '&Open Stylesheet'
        OnClick = OpenStylesheet1Click
      end
    end
  end
  object SaveDialog: TSaveDialog
    Filter = 
      'XML Files (*.xml)|*.xml|XML Schema (*.xsd)|*.xsd|XSL File (*.xsl' +
      ')|*.xsl|XHTML File (*.xhtml)|*.xhtml|All Files|*.*'
    Title = 'Save XML File'
    Left = 254
    Top = 184
  end
end
