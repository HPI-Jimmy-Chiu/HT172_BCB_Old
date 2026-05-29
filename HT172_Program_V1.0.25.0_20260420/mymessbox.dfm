object MyMessageBox: TMyMessageBox
  Left = 328
  Top = 430
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Message'
  ClientHeight = 396
  ClientWidth = 662
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poDefault
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 24
    Top = 496
    Width = 113
    Height = 20
    Caption = 'ErrorMessage'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 24
    Top = 571
    Width = 67
    Height = 20
    Caption = 'Solution'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 457
    Height = 153
    BevelInner = bvLowered
    Color = 14670284
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 42
      Width = 441
      Height = 33
      Alignment = taCenter
      AutoSize = False
      Caption = '123'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'MS Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 8
      Top = 88
      Width = 442
      Height = 33
      Alignment = taCenter
      AutoSize = False
      Caption = 'Label2'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'MS Serif'
      Font.Style = []
      ParentFont = False
    end
    object Button1: TButton
      Left = 320
      Top = 72
      Width = 129
      Height = 33
      Caption = 'Change Language'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Visible = False
      OnClick = Button1Click
    end
  end
  object palPause: TPanel
    Left = 172
    Top = 172
    Width = 129
    Height = 33
    BevelWidth = 2
    Caption = 'Pause'
    Color = 16757009
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = palPauseClick
  end
  object Memo1: TMemo
    Left = 24
    Top = 596
    Width = 641
    Height = 73
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Lines.Strings = (
      'Memo1')
    ParentFont = False
    TabOrder = 6
  end
  object Edit1: TEdit
    Left = 24
    Top = 520
    Width = 641
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Text = 'Edit1'
  end
  object Button2: TButton
    Left = 328
    Top = 120
    Width = 129
    Height = 33
    Caption = 'Off Buzzer'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = Button2Click
  end
  object palYes: TPanel
    Tag = 1
    Left = 71
    Top = 172
    Width = 130
    Height = 33
    BevelWidth = 2
    Caption = 'Yes'
    Color = 16757009
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Visible = False
    OnClick = palYesClick
  end
  object palNo: TPanel
    Tag = 2
    Left = 271
    Top = 172
    Width = 130
    Height = 33
    BevelWidth = 2
    Caption = 'No'
    Color = 16757009
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Visible = False
    OnClick = palYesClick
  end
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
    Left = 288
    Top = 216
  end
end
