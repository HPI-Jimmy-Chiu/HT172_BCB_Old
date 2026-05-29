object TrayEditForm: TTrayEditForm
  Left = 812
  Top = 81
  BorderStyle = bsSingle
  Caption = 'TrayEditForm'
  ClientHeight = 658
  ClientWidth = 503
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnUpdate: TSpeedButton
    Left = 224
    Top = 576
    Width = 137
    Height = 33
    Caption = 'Update'
    OnClick = btnUpdateClick
  end
  object btnAbort: TSpeedButton
    Left = 368
    Top = 576
    Width = 129
    Height = 33
    Caption = 'Abort'
    OnClick = btnAbortClick
  end
  object lblBin: TLabel
    Left = 20
    Top = 612
    Width = 15
    Height = 13
    Caption = 'Bin'
  end
  object mtLoaderBuffer: TTMyTray
    Left = 6
    Top = 2
    Width = 491
    Height = 567
    Color = clWhite
    XItem = 5
    YItem = 8
    OnMouseDown = mtLoaderBufferMouseDown
    OnMouseUp = mtLoaderBufferMouseUp
    OnMouseMove = mtLoaderBufferMouseMove
  end
  object btnManualInput: TButton
    Left = 336
    Top = 632
    Width = 113
    Height = 25
    Caption = 'Manual Input'
    TabOrder = 3
    OnClick = btnManualInputClick
  end
  object cbBinCount: TComboBox
    Left = 56
    Top = 608
    Width = 145
    Height = 21
    ItemHeight = 13
    TabOrder = 2
    Text = '1'
  end
  object edtID: TLabeledEdit
    Left = 56
    Top = 580
    Width = 121
    Height = 21
    EditLabel.Width = 35
    EditLabel.Height = 13
    EditLabel.Caption = 'Tray ID'
    LabelPosition = lpLeft
    LabelSpacing = 3
    TabOrder = 1
  end
  object edtXPos: TLabeledEdit
    Left = 56
    Top = 636
    Width = 89
    Height = 21
    EditLabel.Width = 7
    EditLabel.Height = 13
    EditLabel.Caption = 'X'
    LabelPosition = lpLeft
    LabelSpacing = 3
    TabOrder = 4
    OnClick = edtXPosClick
  end
  object edtYPos: TLabeledEdit
    Left = 196
    Top = 637
    Width = 89
    Height = 21
    EditLabel.Width = 7
    EditLabel.Height = 13
    EditLabel.Caption = 'Y'
    LabelPosition = lpLeft
    LabelSpacing = 3
    TabOrder = 5
    OnClick = edtYPosClick
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 96
    Top = 8
  end
end
