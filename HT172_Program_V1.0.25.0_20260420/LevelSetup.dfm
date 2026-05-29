object fLevelSetup: TfLevelSetup
  Left = 583
  Top = 239
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Level Setup'
  ClientHeight = 712
  ClientWidth = 579
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object RadioGroup1: TRadioGroup
    Left = 8
    Top = 8
    Width = 561
    Height = 65
    Caption = 'Enable at'
    Columns = 5
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Items.Strings = (
      'Operator'
      'Engineer'
      'Supervisor'
      'HonPrec'
      'Always Not')
    ParentFont = False
    TabOrder = 0
  end
  object RadioGroup2: TRadioGroup
    Left = 8
    Top = 88
    Width = 561
    Height = 65
    Caption = 'Visible at'
    Columns = 5
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Items.Strings = (
      'Operator'
      'Engineer'
      'Supervisor'
      'HonPrec'
      'Always Not')
    ParentFont = False
    TabOrder = 1
  end
  object RadioGroup3: TRadioGroup
    Left = 8
    Top = 168
    Width = 561
    Height = 65
    Caption = 'Enable at'
    Columns = 5
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Items.Strings = (
      'Pause'
      'Running'
      'Always')
    ParentFont = False
    TabOrder = 2
  end
  object RadioGroup4: TRadioGroup
    Left = 7
    Top = 248
    Width = 561
    Height = 65
    Caption = 'Visible at'
    Columns = 5
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Items.Strings = (
      'Pause'
      'Running'
      'Always')
    ParentFont = False
    TabOrder = 3
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 466
    Width = 561
    Height = 81
    Caption = 'Language'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 52
      Height = 20
      Caption = 'English'
    end
    object Label2: TLabel
      Left = 16
      Top = 48
      Width = 64
      Height = 20
      Caption = #20013#25991'        '
    end
    object Edit1: TEdit
      Left = 100
      Top = 21
      Width = 341
      Height = 28
      TabOrder = 0
      Text = 'Edit1'
    end
    object Edit2: TEdit
      Left = 100
      Top = 48
      Width = 341
      Height = 28
      TabOrder = 1
      Text = 'Edit2'
    end
  end
  object RadioGroup5: TRadioGroup
    Left = 8
    Top = 312
    Width = 561
    Height = 65
    Caption = 'Enable at'
    Columns = 5
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Items.Strings = (
      'Ambient'
      'Hot'
      'Always')
    ParentFont = False
    TabOrder = 4
  end
  object RadioGroup6: TRadioGroup
    Left = 7
    Top = 392
    Width = 561
    Height = 65
    Caption = 'Visible at'
    Columns = 5
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Items.Strings = (
      'Ambient'
      'Hot'
      'Always')
    ParentFont = False
    TabOrder = 5
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 554
    Width = 561
    Height = 81
    Caption = 'Font'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    object Label3: TLabel
      Left = 8
      Top = 32
      Width = 33
      Height = 20
      Caption = 'Font'
    end
    object Label4: TLabel
      Left = 368
      Top = 32
      Width = 49
      Height = 20
      Caption = 'Color   '
    end
    object Label5: TLabel
      Left = 192
      Top = 32
      Width = 31
      Height = 20
      Caption = 'Size'
    end
    object edFont: TEdit
      Left = 56
      Top = 32
      Width = 121
      Height = 28
      TabOrder = 0
      Text = 'edFont'
      OnClick = edFontClick
    end
    object edSize: TEdit
      Left = 232
      Top = 32
      Width = 121
      Height = 28
      TabOrder = 1
      Text = 'edSize'
      OnClick = edFontClick
    end
    object edColor: TEdit
      Left = 416
      Top = 32
      Width = 121
      Height = 28
      TabOrder = 2
      Text = 'edColor'
      OnClick = edFontClick
    end
  end
  object BitBtn1: TBitBtn
    Left = 8
    Top = 658
    Width = 174
    Height = 49
    Caption = 'Update'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 207
    Top = 658
    Width = 166
    Height = 49
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    OnClick = BitBtn2Click
  end
  object BitBtn3: TBitBtn
    Left = 396
    Top = 658
    Width = 174
    Height = 49
    Caption = 'Update&&Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 4259584
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    OnClick = BitBtn3Click
  end
  object SetLevelPopupMenu: TPopupMenu
    Left = 432
    Top = 152
    object SetLevel1: TMenuItem
      Caption = 'Set Level'
      OnClick = SetLevel1Click
    end
  end
  object FontDialog1: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MinFontSize = 0
    MaxFontSize = 0
    Left = 528
    Top = 152
  end
end
