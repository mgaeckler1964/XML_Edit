object xmlEditorFrame: TxmlEditorFrame
  Left = 0
  Top = 0
  Width = 675
  Height = 493
  TabOrder = 0
  object Splitter2: TSplitter
    Left = 217
    Top = 0
    Width = 5
    Height = 474
    Cursor = crHSplit
  end
  object XmlTreeView: TTreeView
    Left = 0
    Top = 0
    Width = 217
    Height = 474
    Align = alLeft
    DragMode = dmAutomatic
    HideSelection = False
    Indent = 19
    PopupMenu = TreeViewPopupMenu
    TabOrder = 0
    OnAdvancedCustomDrawItem = XmlTreeViewAdvancedCustomDrawItem
    OnChange = XmlTreeViewChange
    OnContextPopup = XmlTreeViewContextPopup
    OnDragDrop = XmlTreeViewDragDrop
    OnDragOver = XmlTreeViewDragOver
    OnEdited = XmlTreeViewEdited
    OnEditing = XmlTreeViewEditing
    OnKeyDown = XmlTreeViewKeyDown
  end
  object Panel1: TPanel
    Left = 222
    Top = 0
    Width = 453
    Height = 474
    Align = alClient
    TabOrder = 1
    object Splitter1: TSplitter
      Left = 1
      Top = 185
      Width = 451
      Height = 5
      Cursor = crVSplit
      Align = alTop
    end
    object AttributeGrid: TStringGrid
      Left = 1
      Top = 1
      Width = 451
      Height = 184
      Align = alTop
      ColCount = 2
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goEditing]
      PopupMenu = AttributePopupMenu
      TabOrder = 0
      OnContextPopup = AttributeGridContextPopup
      OnDrawCell = AttributeGridDrawCell
      OnGetEditText = AttributeGridGetEditText
      OnKeyUp = AttributeGridKeyUp
      OnSetEditText = AttributeGridSetEditText
      RowHeights = (
        24)
    end
    object ValueMemo: TMemo
      Left = 1
      Top = 190
      Width = 451
      Height = 283
      Align = alClient
      PopupMenu = ValuePopupMenu
      ScrollBars = ssVertical
      TabOrder = 1
      OnChange = ValueMemoChange
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 474
    Width = 675
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  object TreeViewPopupMenu: TPopupMenu
    Left = 112
    Top = 48
    object Expand: TMenuItem
      Caption = 'Expand'
      ShortCut = 16571
      OnClick = ExpandClick
    end
    object Collapse: TMenuItem
      Caption = 'Collapse'
      OnClick = CollapseClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object ShowStyles: TMenuItem
      Caption = 'Show Styles'
      OnClick = ShowStylesClick
    end
    object N5: TMenuItem
      Caption = '-'
    end
    object ChangePopup: TMenuItem
      Caption = 'Change'
      OnClick = ChangePopupClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object DeleteChildrenItem: TMenuItem
      Caption = 'Delete with Children'
      ShortCut = 46
      OnClick = TreeViewPopupClick
    end
    object DeleteWithoutChildren: TMenuItem
      Caption = 'Delete without Children'
      OnClick = TreeViewPopupClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object CloneElement: TMenuItem
      Caption = 'Clone'
      OnClick = TreeViewPopupClick
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object InsElement: TMenuItem
      Caption = 'Element'
      ShortCut = 45
      OnClick = TreeViewPopupClick
    end
    object InsTextPCDATA: TMenuItem
      Caption = 'Text (PCDATA)'
      ShortCut = 16429
      OnClick = TreeViewPopupClick
    end
    object InsTextCDATA: TMenuItem
      Caption = 'Text (CDATA)'
      ShortCut = 8237
      OnClick = TreeViewPopupClick
    end
    object InsComment: TMenuItem
      Caption = 'Comment'
      ShortCut = 32813
      OnClick = TreeViewPopupClick
    end
    object ProcInstruction: TMenuItem
      Caption = 'Processing Instruction'
      ShortCut = 49197
      OnClick = TreeViewPopupClick
    end
    object insXmlStyleSheet: TMenuItem
      Caption = 'xml-stylesheet'
      OnClick = TreeViewPopupClick
    end
    object DOCTYPE: TMenuItem
      Caption = '!DOCTYPE'
      ShortCut = 24621
      OnClick = TreeViewPopupClick
    end
  end
  object AttributePopupMenu: TPopupMenu
    Left = 422
    Top = 16
  end
  object ValuePopupMenu: TPopupMenu
    Left = 374
    Top = 248
    object CreateElement: TMenuItem
      Caption = 'Create Element'
      OnClick = CreateElementClick
    end
  end
end
