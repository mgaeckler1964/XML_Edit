object XmlViewerForm: TXmlViewerForm
  Left = 350
  Top = 187
  Width = 577
  Height = 333
  Caption = 'XmlViewerForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object PanelToolBar: TPanel
    Left = 0
    Top = 0
    Width = 569
    Height = 41
    Align = alTop
    TabOrder = 0
    object CheckBoxXmlFrames: TCheckBox
      Left = 8
      Top = 8
      Width = 65
      Height = 17
      Caption = 'Frames'
      TabOrder = 0
      OnClick = CheckBoxXmlFramesClick
    end
  end
end
