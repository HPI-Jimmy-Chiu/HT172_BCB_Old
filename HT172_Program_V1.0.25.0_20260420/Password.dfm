object fPassword: TfPassword
  Left = 367
  Top = 444
  Width = 496
  Height = 253
  Caption = 'Password'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 72
    Width = 125
    Height = 35
    Caption = 'Password'
    Font.Charset = CHINESEBIG5_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = '微軟正黑體'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 24
    Top = 16
    Width = 132
    Height = 35
    Caption = 'Username'
    Font.Charset = CHINESEBIG5_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = '微軟正黑體'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object SpeedButton1: TSpeedButton
    Left = 32
    Top = 152
    Width = 209
    Height = 49
    Caption = 'OK'
    Font.Charset = CHINESEBIG5_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = '新細明體'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton2: TSpeedButton
    Left = 248
    Top = 152
    Width = 201
    Height = 49
    Caption = 'Cancel'
    Font.Charset = CHINESEBIG5_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = '新細明體'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton2Click
  end
  object Edit1: TEdit
    Left = 174
    Top = 64
    Width = 270
    Height = 45
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 2
    OnKeyDown = Edit1KeyDown
  end
  object Edit2: TEdit
    Left = 174
    Top = 8
    Width = 270
    Height = 45
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnKeyDown = Edit2KeyDown
  end
  object ComboBox1: TComboBox
    Left = 176
    Top = 8
    Width = 300
    Height = 45
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -32
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemHeight = 37
    ParentFont = False
    TabOrder = 1
    OnChange = ComboBox1Change
  end
end
