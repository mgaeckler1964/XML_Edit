object cssStyleFrame: TcssStyleFrame
  Left = 0
  Top = 0
  Width = 320
  Height = 240
  TabOrder = 0
  OnResize = FrameResize
  object StringGridcssStyles: TStringGrid
    Left = 0
    Top = 0
    Width = 320
    Height = 240
    Align = alClient
    ColCount = 2
    RowCount = 1
    FixedRows = 0
    TabOrder = 0
    OnDrawCell = StringGridcssStylesDrawCell
    RowHeights = (
      24)
  end
end
