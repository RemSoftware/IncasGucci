object PosVuotiForm: TPosVuotiForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  ClientHeight = 289
  ClientWidth = 561
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = OnActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 561
    Height = 32
    Align = alTop
    Caption = 'Segnali PLC'
    Ctl3D = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 0
    ExplicitWidth = 572
  end
  object Panel4: TPanel
    Left = 0
    Top = 240
    Width = 561
    Height = 49
    Align = alBottom
    BevelOuter = bvLowered
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    ExplicitTop = 119
    ExplicitWidth = 572
    object SpeedButton1: TSpeedButton
      Left = 147
      Top = 7
      Width = 129
      Height = 38
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = 'Refresh Mode'
    end
    object BitBtn2: TBitBtn
      Left = 286
      Top = 6
      Width = 129
      Height = 38
      Caption = 'Chiudi'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = BitBtn4Click
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 96
    Width = 254
    Height = 139
    Caption = 'Segnali d'#39'Ingresso'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object cbRichiestaIncrocio: TCheckBox
      Left = 9
      Top = 26
      Width = 163
      Height = 17
      Caption = 'Richiesta Impegno Incrocio'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object BitBtn6: TBitBtn
      Tag = 1
      Left = 165
      Top = 107
      Width = 80
      Height = 25
      Caption = 'Set'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = BitBtn6Click
    end
  end
  object GroupBox4: TGroupBox
    Left = 267
    Top = 96
    Width = 292
    Height = 140
    Caption = 'Segnali Uscita'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object cbConsensoIncrocio: TCheckBox
      Left = 15
      Top = 22
      Width = 158
      Height = 25
      Caption = 'Consenso Impegno Incrocio'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object BitBtn1: TBitBtn
      Tag = 1
      Left = 200
      Top = 106
      Width = 83
      Height = 25
      Caption = 'Set'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = BitBtn1Click
    end
    object cbSegnalazioneRosso: TCheckBox
      Left = 15
      Top = 47
      Width = 138
      Height = 25
      Caption = 'Segnalazione Rosso'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object cbSegnalazioneGiallo: TCheckBox
      Left = 15
      Top = 73
      Width = 133
      Height = 25
      Caption = 'Segnalazione Giallo'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object cbSegnalazioneVerde: TCheckBox
      Left = 15
      Top = 99
      Width = 133
      Height = 25
      Caption = 'Segnalazione Verde'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
    end
  end
  object Panel7: TPanel
    Left = 10
    Top = 39
    Width = 547
    Height = 53
    BevelOuter = bvLowered
    TabOrder = 4
    object Label3: TLabel
      Left = 310
      Top = 18
      Width = 61
      Height = 13
      Caption = 'Cycle Time : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lCycle: TLabel
      Left = 377
      Top = 19
      Width = 12
      Height = 13
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 451
      Top = 18
      Width = 36
      Height = 13
      Caption = 'Errors :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lErrors: TLabel
      Left = 497
      Top = 18
      Width = 6
      Height = 13
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Panel15: TPanel
      Left = 13
      Top = 13
      Width = 125
      Height = 25
      Alignment = taRightJustify
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = 'Connesso     '
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object Shape4: TShape
        Left = 4
        Top = 4
        Width = 21
        Height = 17
        Brush.Color = clLime
        Shape = stCircle
      end
    end
    object GroupBox1: TGroupBox
      Left = 152
      Top = 6
      Width = 139
      Height = 33
      Caption = 'Watch Dog'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object Label9: TLabel
        Left = 7
        Top = 16
        Width = 13
        Height = 13
        Caption = 'Plc'
      end
      object lWatchDogPlc: TLabel
        Left = 26
        Top = 16
        Width = 6
        Height = 13
        Caption = '0'
      end
      object Label12: TLabel
        Left = 70
        Top = 16
        Width = 11
        Height = 13
        Caption = 'Pc'
      end
      object lWatchDogPc: TLabel
        Left = 91
        Top = 16
        Width = 6
        Height = 13
        Caption = '0'
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 400
    OnTimer = Timer1Timer
    Left = 461
    Top = 228
  end
end
