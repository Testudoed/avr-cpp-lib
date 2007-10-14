object Main: TMain
  Left = 446
  Top = 329
  Width = 771
  Height = 661
  Caption = 'AVRCppLib device-specific code generator'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Grid: TStringGrid
    Left = 0
    Top = 201
    Width = 763
    Height = 390
    Align = alClient
    DefaultColWidth = 100
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    TabOrder = 0
    OnMouseDown = GridMouseDown
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 763
    Height = 201
    Align = alTop
    Caption = 'Configuration'
    TabOrder = 1
    object Label1: TLabel
      Left = 16
      Top = 28
      Width = 70
      Height = 13
      Caption = 'XML files path:'
    end
    object Label2: TLabel
      Left = 16
      Top = 52
      Width = 68
      Height = 13
      Caption = 'XSL files path:'
    end
    object Label3: TLabel
      Left = 16
      Top = 76
      Width = 83
      Height = 13
      Caption = 'Destination path:'
    end
    object Label4: TLabel
      Left = 16
      Top = 140
      Width = 63
      Height = 13
      Caption = 'Transformer:'
    end
    object Label5: TLabel
      Left = 16
      Top = 164
      Width = 101
      Height = 13
      Caption = 'Transform command:'
    end
    object Label6: TLabel
      Left = 16
      Top = 116
      Width = 83
      Height = 13
      Caption = 'Exclude XSL files:'
    end
    object eDirXML: TEdit
      Left = 136
      Top = 24
      Width = 425
      Height = 21
      TabOrder = 0
    end
    object eDirXSL: TEdit
      Left = 136
      Top = 48
      Width = 425
      Height = 21
      TabOrder = 1
    end
    object eDirDestination: TEdit
      Left = 136
      Top = 72
      Width = 425
      Height = 21
      TabOrder = 3
    end
    object eTransformerPath: TEdit
      Left = 136
      Top = 136
      Width = 425
      Height = 21
      TabOrder = 2
    end
    object eTransformCommand: TEdit
      Left = 136
      Top = 160
      Width = 425
      Height = 21
      TabOrder = 4
    end
    object eXSLFileExclusion: TEdit
      Left = 136
      Top = 112
      Width = 425
      Height = 21
      TabOrder = 5
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 591
    Width = 763
    Height = 43
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    DesignSize = (
      763
      43)
    object bTransform: TBitBtn
      Left = 557
      Top = 10
      Width = 201
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '&Execute XSLT transformation'
      TabOrder = 1
      OnClick = bTransformClick
      Kind = bkOK
    end
    object bRefresh: TBitBtn
      Left = 453
      Top = 10
      Width = 94
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '&Refresh'
      ModalResult = 4
      TabOrder = 0
      OnClick = bRefreshClick
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333444444
        33333333333F8888883F33330000324334222222443333388F3833333388F333
        000032244222222222433338F8833FFFFF338F3300003222222AAAAA22243338
        F333F88888F338F30000322222A33333A2224338F33F8333338F338F00003222
        223333333A224338F33833333338F38F00003222222333333A444338FFFF8F33
        3338888300003AAAAAAA33333333333888888833333333330000333333333333
        333333333333333333FFFFFF000033333333333344444433FFFF333333888888
        00003A444333333A22222438888F333338F3333800003A2243333333A2222438
        F38F333333833338000033A224333334422224338338FFFFF8833338000033A2
        22444442222224338F3388888333FF380000333A2222222222AA243338FF3333
        33FF88F800003333AA222222AA33A3333388FFFFFF8833830000333333AAAAAA
        3333333333338888883333330000333333333333333333333333333333333333
        0000}
      NumGlyphs = 2
    end
  end
end
