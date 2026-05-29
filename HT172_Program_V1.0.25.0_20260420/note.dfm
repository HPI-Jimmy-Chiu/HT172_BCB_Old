object fNote: TfNote
  Left = 214
  Top = 79
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Note'
  ClientHeight = 969
  ClientWidth = 1697
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
  object NullLed: TALed
    Left = 24
    Top = 102
    Width = 9
    Height = 9
  end
  object Label1: TLabel
    Left = 56
    Top = 565
    Width = 777
    Height = 20
    AutoSize = False
    Caption = 
      '                                                                ' +
      '                         process procedure'
    Color = 12761254
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Memo1: TMemo
    Left = 0
    Top = 821
    Width = 1697
    Height = 148
    Align = alClient
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1697
    Height = 769
    Align = alTop
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 0
    object TPanel
      Left = 0
      Top = 0
      Width = 157
      Height = 769
      Align = alLeft
      BevelOuter = bvNone
      Color = 12761254
      TabOrder = 0
      object BtnSkip: TBtnPanel
        Tag = 1
        Left = 10
        Top = 56
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'SKIP'
        Color = 8421440
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = BtnSkipClick
        TrueColor = clRed
        FalseColor = 8421440
        TrueFontColor = clWhite
        FalseFontColor = clWhite
        Style = tsButtons
      end
      object BtnRetry: TBtnPanel
        Tag = 2
        Left = 10
        Top = 106
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'RETRY'
        Color = 8404992
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = BtnSkipClick
        TrueColor = clRed
        FalseColor = 8404992
        TrueFontColor = clWhite
        FalseFontColor = clWhite
        Style = tsButtons
      end
      object BtnTrayFeed: TBtnPanel
        Tag = 3
        Left = 10
        Top = 155
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'TRAY FEED'
        Color = 8404992
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = BtnSkipClick
        TrueColor = clRed
        FalseColor = 8404992
        TrueFontColor = clWhite
        FalseFontColor = clWhite
        Style = tsButtons
      end
      object BtnTrayEnd: TBtnPanel
        Tag = 4
        Left = 10
        Top = 205
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'TRAY END'
        Color = 8404992
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = BtnSkipClick
        TrueColor = clRed
        FalseColor = 8404992
        TrueFontColor = clWhite
        FalseFontColor = clWhite
        Style = tsButtons
      end
      object BtnCleanOut: TBtnPanel
        Tag = 5
        Left = 10
        Top = 254
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'CLEAN OUT'
        Color = 8404992
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnClick = BtnSkipClick
        TrueColor = clRed
        FalseColor = 8404992
        TrueFontColor = clWhite
        FalseFontColor = clWhite
        Style = tsButtons
      end
      object BtnStart: TBtnPanel
        Tag = 4
        Left = 10
        Top = 303
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'START'
        Color = clBlue
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnClick = BtnStartClick
        TrueColor = 12615680
        FalseColor = clBlue
        FalseFontColor = clWhite
        Style = tsButtons
      end
      object BtnPause: TBtnPanel
        Tag = 5
        Left = 10
        Top = 353
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'PAUSE'
        Color = 8404992
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        OnClick = BtnPauseClick
        TrueColor = 12615680
        FalseColor = 8404992
        FalseFontColor = clWhite
        Style = tsButtons
      end
      object BtnOffBuzzer: TBtnPanel
        Tag = 5
        Left = 10
        Top = 402
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'Off Buzzer'
        Color = 8404992
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        OnClick = BtnOffBuzzerClick
        TrueColor = 12615680
        FalseColor = 8404992
        FalseFontColor = clWhite
        Style = tsButtons
      end
      object NullTray: TTMyTray
        Left = 144
        Top = 485
        Width = 9
        Height = 9
      end
      object BtnHome: TBtnPanel
        Tag = 6
        Left = 10
        Top = 7
        Width = 135
        Height = 33
        BevelInner = bvRaised
        Caption = 'HOME && RETRY'
        Color = 8404992
        Font.Charset = CHINESEBIG5_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = #24494#36575#27491#40657#39636
        Font.Style = []
        ParentFont = False
        TabOrder = 9
        OnClick = BtnSkipClick
        TrueColor = clRed
        FalseColor = 8404992
        TrueFontColor = clWhite
        FalseFontColor = clWhite
        Style = tsButtons
      end
    end
    object PanelMain6: TPanel
      Left = 157
      Top = 0
      Width = 1540
      Height = 769
      Align = alClient
      BevelInner = bvLowered
      Color = 12761254
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object labCCDStep: TLabel
        Left = 1222
        Top = 72
        Width = 3
        Height = 13
      end
      object Panel11: TPanel
        Left = 499
        Top = 618
        Width = 770
        Height = 9
        Color = clOlive
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 77
      end
      object Panel3: TPanel
        Left = 502
        Top = 472
        Width = 767
        Height = 9
        Color = clOlive
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 58
      end
      object Panel14: TPanel
        Left = 502
        Top = 326
        Width = 767
        Height = 9
        Color = clOlive
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 47
      end
      object Panel20: TPanel
        Left = 499
        Top = 177
        Width = 746
        Height = 9
        Color = clOlive
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 31
      end
      object pnlAuto1PathL: TPanel
        Left = 494
        Top = 32
        Width = 9
        Height = 710
        Color = clOlive
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 20
      end
      object palTrack3TrayArm: TPanel
        Left = 1164
        Top = 185
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 32
        object mtTrack3TrayArm: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palTrack2TrayArm: TPanel
        Left = 1164
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 56
        object mtTrack2TrayArm: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palTrack1TrayArm: TPanel
        Left = 1164
        Top = 479
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 64
        object mtTrack1TrayArm: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object Panel71: TPanel
        Left = 330
        Top = 665
        Width = 34
        Height = 40
        Color = 9534289
        TabOrder = 79
        object Panel72: TPanel
          Left = 4
          Top = 7
          Width = 26
          Height = 26
          Color = 12428409
          TabOrder = 0
          object MyLed5: TMyLed
            Left = 2
            Top = 2
            Width = 22
            Height = 22
            Hint = 'WorkArm2OutSuck10'
            LEDStyle = LEDSqLarge
            ShowHint = True
          end
        end
      end
      object palMagEmptyTray: TPanel
        Left = 296
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 49
        object mtMagEmptyTray: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object Panel30: TPanel
        Left = 285
        Top = 170
        Width = 423
        Height = 9
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 29
      end
      object palWorkEmpty3: TPanel
        Left = 506
        Top = 187
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 34
        object mtWorkEmpty3: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palWorkEmpty2: TPanel
        Left = 506
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 50
        object mtWorkEmpty2: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palMag2: TPanel
        Left = 186
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 48
        object mtMag2: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palMag1: TPanel
        Left = 186
        Top = 484
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 28
        object mtMag1: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palMag3: TPanel
        Left = 186
        Top = 164
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 66
        object mtMag3: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object Panel67: TPanel
        Left = 1266
        Top = 536
        Width = 17
        Height = 169
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 70
      end
      object Panel39: TPanel
        Left = 1288
        Top = 523
        Width = 134
        Height = 153
        Color = 9534289
        TabOrder = 69
        object labAuto05: TLabel
          Left = 8
          Top = 14
          Width = 34
          Height = 13
          Caption = 'Auto05'
        end
        object labAuto04: TLabel
          Left = 8
          Top = 42
          Width = 34
          Height = 13
          Caption = 'Auto04'
        end
        object labAuto03: TLabel
          Left = 8
          Top = 70
          Width = 34
          Height = 13
          Caption = 'Auto03'
        end
        object labAuto02: TLabel
          Left = 8
          Top = 98
          Width = 34
          Height = 13
          Caption = 'Auto02'
        end
        object labAuto01: TLabel
          Left = 8
          Top = 126
          Width = 34
          Height = 13
          Caption = 'Auto01'
        end
        object palAutoStore05: TPanel
          Left = 53
          Top = 8
          Width = 65
          Height = 25
          TabOrder = 0
        end
        object palAutoStore04: TPanel
          Left = 53
          Top = 36
          Width = 65
          Height = 25
          TabOrder = 1
        end
        object palAutoStore03: TPanel
          Left = 53
          Top = 64
          Width = 65
          Height = 25
          TabOrder = 2
        end
        object palAutoStore02: TPanel
          Left = 53
          Top = 92
          Width = 65
          Height = 25
          TabOrder = 3
        end
        object palAutoStore01: TPanel
          Left = 53
          Top = 120
          Width = 65
          Height = 25
          TabOrder = 4
        end
      end
      object Panel37: TPanel
        Left = 1288
        Top = 361
        Width = 134
        Height = 153
        Color = 9534289
        TabOrder = 57
        object labAuto10: TLabel
          Left = 8
          Top = 14
          Width = 34
          Height = 13
          Caption = 'Auto10'
        end
        object labAuto09: TLabel
          Left = 8
          Top = 42
          Width = 34
          Height = 13
          Caption = 'Auto09'
        end
        object labAuto08: TLabel
          Left = 8
          Top = 70
          Width = 34
          Height = 13
          Caption = 'Auto08'
        end
        object labAuto07: TLabel
          Left = 8
          Top = 98
          Width = 34
          Height = 13
          Caption = 'Auto07'
        end
        object labAuto06: TLabel
          Left = 8
          Top = 126
          Width = 34
          Height = 13
          Caption = 'Auto06'
        end
        object palAutoStore10: TPanel
          Left = 53
          Top = 8
          Width = 65
          Height = 25
          TabOrder = 0
        end
        object palAutoStore09: TPanel
          Left = 53
          Top = 36
          Width = 65
          Height = 25
          TabOrder = 1
        end
        object palAutoStore08: TPanel
          Left = 53
          Top = 64
          Width = 65
          Height = 25
          TabOrder = 2
        end
        object palAutoStore07: TPanel
          Left = 53
          Top = 92
          Width = 65
          Height = 25
          TabOrder = 3
        end
        object palAutoStore06: TPanel
          Left = 53
          Top = 120
          Width = 65
          Height = 25
          TabOrder = 4
        end
      end
      object Panel36: TPanel
        Left = 1290
        Top = 195
        Width = 132
        Height = 153
        Color = 9534289
        TabOrder = 41
        object labAuto15: TLabel
          Left = 8
          Top = 14
          Width = 34
          Height = 13
          Caption = 'Auto15'
        end
        object labAuto14: TLabel
          Left = 8
          Top = 42
          Width = 34
          Height = 13
          Caption = 'Auto14'
        end
        object labAuto13: TLabel
          Left = 8
          Top = 70
          Width = 34
          Height = 13
          Caption = 'Auto13'
        end
        object labAuto12: TLabel
          Left = 8
          Top = 98
          Width = 34
          Height = 13
          Caption = 'Auto12'
        end
        object labAuto11: TLabel
          Left = 8
          Top = 126
          Width = 34
          Height = 13
          Caption = 'Auto11'
        end
        object palAutoStore15: TPanel
          Left = 51
          Top = 8
          Width = 65
          Height = 25
          TabOrder = 0
        end
        object palAutoStore14: TPanel
          Left = 51
          Top = 36
          Width = 65
          Height = 25
          TabOrder = 1
        end
        object palAutoStore13: TPanel
          Left = 51
          Top = 64
          Width = 65
          Height = 25
          TabOrder = 2
        end
        object palAutoStore12: TPanel
          Left = 51
          Top = 92
          Width = 65
          Height = 25
          TabOrder = 3
        end
        object palAutoStore11: TPanel
          Left = 51
          Top = 120
          Width = 65
          Height = 25
          TabOrder = 4
        end
      end
      object Panel35: TPanel
        Left = 1291
        Top = 27
        Width = 131
        Height = 153
        Color = 9534289
        TabOrder = 6
        object labAuto20: TLabel
          Left = 8
          Top = 14
          Width = 34
          Height = 13
          Caption = 'Auto20'
        end
        object labAuto19: TLabel
          Left = 8
          Top = 42
          Width = 34
          Height = 13
          Caption = 'Auto19'
        end
        object labAuto18: TLabel
          Left = 8
          Top = 70
          Width = 34
          Height = 13
          Caption = 'Auto18'
        end
        object labAuto17: TLabel
          Left = 8
          Top = 98
          Width = 34
          Height = 13
          Caption = 'Auto17'
        end
        object labAuto16: TLabel
          Left = 8
          Top = 126
          Width = 34
          Height = 13
          Caption = 'Auto16'
        end
        object palAutoStore20: TPanel
          Left = 51
          Top = 8
          Width = 65
          Height = 25
          TabOrder = 0
        end
        object palAutoStore19: TPanel
          Left = 51
          Top = 36
          Width = 65
          Height = 25
          TabOrder = 1
        end
        object palAutoStore18: TPanel
          Left = 51
          Top = 64
          Width = 65
          Height = 25
          TabOrder = 2
        end
        object palAutoStore17: TPanel
          Left = 51
          Top = 92
          Width = 65
          Height = 25
          TabOrder = 3
        end
        object palAutoStore16: TPanel
          Left = 51
          Top = 120
          Width = 65
          Height = 25
          TabOrder = 4
        end
      end
      object palMagLD: TPanel
        Left = 296
        Top = 187
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 33
        object mtMagLD: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object Panel34: TPanel
        Left = 285
        Top = 32
        Width = 9
        Height = 710
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
      end
      object palEmpty2Car: TPanel
        Left = 296
        Top = 602
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 74
        object mtMagEmptyCar: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object Panel31: TPanel
        Left = 379
        Top = 32
        Width = 9
        Height = 710
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 17
      end
      object palAuto1: TPanel
        Left = 715
        Top = 479
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 59
        object mtAuto1: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto6: TPanel
        Left = 715
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 51
        object mtAuto6: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto5: TPanel
        Left = 1060
        Top = 479
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 63
        object mtAuto5: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto4: TPanel
        Left = 974
        Top = 479
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 62
        object mtAuto4: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto3: TPanel
        Left = 887
        Top = 479
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 61
        object mtAuto3: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto2: TPanel
        Left = 801
        Top = 479
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 60
        object mtAuto2: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto10: TPanel
        Left = 1060
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 55
        object mtAuto10: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto9: TPanel
        Left = 974
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 54
        object mtAuto9: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto8: TPanel
        Left = 887
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 53
        object mtAuto8: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto7: TPanel
        Left = 801
        Top = 333
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 52
        object mtAuto7: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto15: TPanel
        Left = 1060
        Top = 187
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 40
        object mtAuto15: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto14: TPanel
        Left = 974
        Top = 187
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 39
        object mtAuto14: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto13: TPanel
        Left = 888
        Top = 187
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 38
        object mtAuto13: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto12: TPanel
        Left = 802
        Top = 187
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 37
        object mtAuto12: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto11: TPanel
        Left = 716
        Top = 187
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 36
        object mtAuto11: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object Panel5: TPanel
        Left = 598
        Top = 32
        Width = 9
        Height = 710
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 23
      end
      object palWorkLoader: TPanel
        Left = 609
        Top = 187
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 35
        object mtWorkLoader: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object Panel2: TPanel
        Left = 692
        Top = 32
        Width = 9
        Height = 710
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 24
      end
      object palWorkEmpty1: TPanel
        Left = 506
        Top = 481
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 65
        object mtWorkEmpty1: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object pnlAuto1PathR: TPanel
        Left = 588
        Top = 32
        Width = 9
        Height = 710
        Color = clOlive
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 22
      end
      object palSaveDoor2: TPanel
        Left = 652
        Top = 748
        Width = 285
        Height = 20
        Caption = '2'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 82
      end
      object palSaveDoor3: TPanel
        Left = 4
        Top = 496
        Width = 20
        Height = 180
        Caption = '3'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 67
      end
      object palSaveDoor4: TPanel
        Left = 4
        Top = 261
        Width = 20
        Height = 180
        Caption = '4'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 43
      end
      object palSaveDoor5: TPanel
        Left = 4
        Top = 27
        Width = 20
        Height = 180
        Caption = '5'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object palSaveDoor6: TPanel
        Left = 380
        Top = 4
        Width = 336
        Height = 20
        Caption = '6'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object palSaveDoor7: TPanel
        Left = 720
        Top = 4
        Width = 557
        Height = 20
        Caption = '7'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object palEmg4: TPanel
        Left = 0
        Top = 712
        Width = 57
        Height = 20
        Caption = 'EMG 4'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 80
      end
      object palEmg1: TPanel
        Left = 28
        Top = 4
        Width = 57
        Height = 20
        Caption = 'EMG 1'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object palEmg3: TPanel
        Left = 1365
        Top = 712
        Width = 57
        Height = 20
        Caption = 'EMG 3'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 81
      end
      object palVisionErr: TPanel
        Left = 28
        Top = 27
        Width = 146
        Height = 61
        Caption = 'Vision Error'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object palMagTrayArm: TPanel
        Left = 167
        Top = 298
        Width = 109
        Height = 29
        Caption = 'Tray Arm'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 46
      end
      object palTrack4TrayArm: TPanel
        Left = 1164
        Top = 31
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 14
        object mtTrack4TrayArm: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto20: TPanel
        Left = 1060
        Top = 31
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 13
        object mtAuto20: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto19: TPanel
        Left = 974
        Top = 31
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 12
        object mtAuto19: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto18: TPanel
        Left = 888
        Top = 31
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 11
        object mtAuto18: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto17: TPanel
        Left = 802
        Top = 31
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 10
        object mtAuto17: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palAuto16: TPanel
        Left = 716
        Top = 31
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 9
        object mtAuto16: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object Panel29: TPanel
        Left = 285
        Top = 32
        Width = 420
        Height = 9
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
      end
      object pnlSortArm1_Y: TPanel
        Left = 701
        Top = 31
        Width = 17
        Height = 610
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object pnlSortArm1_X: TPanel
        Left = 597
        Top = 170
        Width = 685
        Height = 17
        Caption = 'Sort Arm 1'
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 30
      end
      object palSortArm1: TPanel
        Left = 680
        Top = 129
        Width = 58
        Height = 99
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 27
        object ledSortArm1ZA: TMyLed
          Left = 5
          Top = 7
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm1ZB: TMyLed
          Left = 31
          Top = 7
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm1ZC: TMyLed
          Left = 5
          Top = 30
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm1ZD: TMyLed
          Left = 31
          Top = 30
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm1ZE: TMyLed
          Left = 5
          Top = 52
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm1ZF: TMyLed
          Left = 31
          Top = 52
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm1ZG: TMyLed
          Left = 5
          Top = 75
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm1ZH: TMyLed
          Left = 31
          Top = 75
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
      end
      object pnlSortArm2_Y: TPanel
        Left = 385
        Top = 32
        Width = 17
        Height = 535
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 18
      end
      object pnlSortArm2_X: TPanel
        Left = 168
        Top = 282
        Width = 233
        Height = 17
        Caption = 'Sort Arm  2'
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 45
      end
      object palSortArm2: TPanel
        Left = 365
        Top = 241
        Width = 58
        Height = 99
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 42
        object ledSortArm2ZA: TMyLed
          Left = 5
          Top = 7
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm2ZC: TMyLed
          Left = 5
          Top = 30
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm2ZE: TMyLed
          Left = 5
          Top = 52
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm2ZG: TMyLed
          Left = 5
          Top = 75
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm2ZB: TMyLed
          Left = 31
          Top = 7
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm2ZD: TMyLed
          Left = 31
          Top = 30
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm2ZF: TMyLed
          Left = 31
          Top = 52
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
        object ledSortArm2ZH: TMyLed
          Left = 31
          Top = 75
          Width = 22
          Height = 14
          Hint = 'WorkArm1InSuck02'
          LEDStyle = LEDHorizontal
          ShowHint = True
        end
      end
      object palSysErr: TPanel
        Left = 28
        Top = 104
        Width = 146
        Height = 61
        Caption = 'System Error'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 26
      end
      object Panel6: TPanel
        Left = 1266
        Top = 32
        Width = 17
        Height = 673
        Color = clActiveCaption
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 25
      end
      object palTrayArm: TPanel
        Left = 1187
        Top = 662
        Width = 96
        Height = 34
        Caption = 'Tray Arm'
        Color = clTeal
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 78
      end
      object palEmpty1Car: TPanel
        Left = 506
        Top = 602
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 75
        object mtEmptyCar: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palWorkEmpty4: TPanel
        Left = 506
        Top = 32
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 21
        object mtWorkEmpty4: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palShiftStage: TPanel
        Left = 409
        Top = 32
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 19
        object mtShiftStage: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palSaveDoor9: TPanel
        Left = 1431
        Top = 261
        Width = 20
        Height = 180
        Caption = '9'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 44
      end
      object palSaveDoor8: TPanel
        Left = 1435
        Top = 27
        Width = 20
        Height = 180
        Caption = '8'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object palSaveDoor10: TPanel
        Left = 1431
        Top = 496
        Width = 20
        Height = 180
        Caption = '10'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 68
      end
      object palSaveDoor1: TPanel
        Left = 940
        Top = 748
        Width = 337
        Height = 20
        Caption = '1'
        Color = clGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 83
      end
      object palLoadCar: TPanel
        Left = 610
        Top = 602
        Width = 80
        Height = 140
        Color = 9534289
        TabOrder = 76
        object mtLoader: TTMyTray
          Left = 10
          Top = 10
          Width = 60
          Height = 120
          XItem = 4
          YItem = 5
          DirectStyle = csRightBottom
        end
      end
      object palEmg2: TPanel
        Left = 1365
        Top = 4
        Width = 57
        Height = 20
        Caption = 'EMG 2'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object palMagEmptyID: TPanel
        Left = 284
        Top = 574
        Width = 105
        Height = 25
        Caption = 'CCD'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 71
      end
      object palWorkEmpty1ID: TPanel
        Left = 495
        Top = 574
        Width = 102
        Height = 25
        Caption = 'CCD'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 72
      end
      object palLoadCarID: TPanel
        Left = 600
        Top = 574
        Width = 101
        Height = 25
        Caption = 'CCD Arm'
        Color = 12428409
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 73
      end
      object palNoticeEmptyTray: TPanel
        Left = 396
        Top = 308
        Width = 743
        Height = 169
        BevelInner = bvRaised
        BevelOuter = bvNone
        BevelWidth = 5
        BorderWidth = 1
        Color = clRed
        TabOrder = 84
        Visible = False
        object Label12: TLabel
          Left = 56
          Top = 32
          Width = 665
          Height = 29
          AutoSize = False
          Caption = 'Please change a empty tray to the top of Magazine!'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clYellow
          Font.Height = -24
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label13: TLabel
          Left = 56
          Top = 69
          Width = 625
          Height = 29
          AutoSize = False
          Caption = #35531#23559'Magazine'#19978#30340'tray'#30436#26356#25563#28858#31354'tray!'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
      end
    end
  end
  object Panel7: TPanel
    Left = 0
    Top = 769
    Width = 1697
    Height = 52
    Align = alTop
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 1
    object Label2: TLabel
      Left = 8
      Top = 4
      Width = 778
      Height = 20
      AutoSize = False
      Caption = '  Err Code                          Message'
      Color = 12761254
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object edtAlarmMsg: TEdit
      Left = 176
      Top = 21
      Width = 1565
      Height = 28
      Color = clWhite
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
    object edtAlarmCode: TEdit
      Left = 7
      Top = 21
      Width = 154
      Height = 28
      Color = 14145495
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
  end
  object Timer1: TTimer
    Interval = 1
    OnTimer = Timer1Timer
    Left = 112
    Top = 472
  end
end
