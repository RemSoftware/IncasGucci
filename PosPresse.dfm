object PosPresseForm: TPosPresseForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  ClientHeight = 359
  ClientWidth = 817
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object PanelTitle: TPanel
    Left = 0
    Top = 0
    Width = 817
    Height = 32
    Align = alTop
    Caption = 'Status Posizione'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    ExplicitWidth = 553
  end
  object Panel5: TPanel
    Left = 0
    Top = 32
    Width = 817
    Height = 40
    Align = alTop
    BevelOuter = bvLowered
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentCtl3D = False
    ParentFont = False
    TabOrder = 1
    ExplicitWidth = 553
    object Label20: TLabel
      Left = 38
      Top = 11
      Width = 84
      Height = 19
      Caption = 'Posizione:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelPos: TLabel
      Left = 130
      Top = 11
      Width = 29
      Height = 19
      Caption = 'Pos'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label23: TLabel
      Left = 301
      Top = 10
      Width = 129
      Height = 19
      Caption = 'Nome posizione'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelPosCli: TLabel
      Left = 453
      Top = 10
      Width = 29
      Height = 19
      Caption = 'Pos'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object GroupBox5: TGroupBox
    Left = 0
    Top = 72
    Width = 226
    Height = 95
    TabOrder = 2
    object Label2: TLabel
      Left = 19
      Top = 24
      Width = 14
      Height = 17
      Alignment = taRightJustify
      Caption = 'ID'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -14
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object edID: TEdit
      Left = 38
      Top = 19
      Width = 169
      Height = 27
      Hint = 'Lotto di produzione'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
  end
  object GroupBox3: TGroupBox
    Left = 228
    Top = 72
    Width = 324
    Height = 97
    Caption = 'Stato'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    object Led: TShape
      Left = 227
      Top = 23
      Width = 21
      Height = 17
      Brush.Color = clGray
      Shape = stCircle
      OnMouseUp = LedMouseUp
    end
    object LedLabel: TLabel
      Left = 253
      Top = 23
      Width = 54
      Height = 16
      Caption = 'Chiamata'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object ckDisabilitaPos: TCheckBox
      Left = 10
      Top = 21
      Width = 83
      Height = 25
      Caption = 'Disabilitata'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = ckDisabilitaPosClick
    end
    object cPrenotataPos: TCheckBox
      Left = 126
      Top = 21
      Width = 74
      Height = 25
      Caption = 'Riservata'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = cPrenotataPosClick
    end
    object eHPrel: TLabeledEdit
      Left = 213
      Top = 55
      Width = 95
      Height = 24
      Alignment = taCenter
      EditLabel.Width = 96
      EditLabel.Height = 16
      EditLabel.Caption = 'Altezza Forche   '
      EditLabel.Font.Charset = DEFAULT_CHARSET
      EditLabel.Font.Color = clBlack
      EditLabel.Font.Height = -13
      EditLabel.Font.Name = 'Tahoma'
      EditLabel.Font.Style = []
      EditLabel.ParentFont = False
      LabelPosition = lpLeft
      TabOrder = 2
    end
  end
  object BitBtn4: TBitBtn
    Left = 346
    Top = 306
    Width = 129
    Height = 38
    Caption = 'Chiudi'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = BitBtn4Click
  end
  object BitBtn5: TBitBtn
    Left = 213
    Top = 306
    Width = 129
    Height = 38
    Caption = 'Applica'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = BitBtn5Click
  end
  object BitBtn1: TBitBtn
    Left = 79
    Top = 306
    Width = 129
    Height = 38
    Caption = 'PLC'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = BitBtn1Click
  end
  object GroupBox2: TGroupBox
    Left = -2
    Top = 169
    Width = 337
    Height = 108
    Caption = 'Input'
    TabOrder = 7
    object Label1: TLabel
      Left = 11
      Top = 69
      Width = 59
      Height = 16
      Caption = 'ID PalletIN'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object ckInput1: TCheckBox
      Left = 11
      Top = 23
      Width = 141
      Height = 17
      Caption = 'Richiesta prelievo'
      TabOrder = 0
    end
    object ckInput2: TCheckBox
      Left = 176
      Top = 24
      Width = 128
      Height = 17
      Caption = 'Consenso Ingresso'
      TabOrder = 1
    end
    object ckInput3: TCheckBox
      Left = 11
      Top = 46
      Width = 129
      Height = 17
      Caption = 'Free'
      TabOrder = 2
    end
    object eUDC: TEdit
      Left = 83
      Top = 69
      Width = 139
      Height = 21
      Alignment = taRightJustify
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object BitBtn6: TBitBtn
      Tag = 1
      Left = 230
      Top = 66
      Width = 97
      Height = 25
      Caption = 'Set'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      Visible = False
    end
    object test: TEdit
      Left = 166
      Top = 42
      Width = 58
      Height = 21
      Alignment = taRightJustify
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
    end
  end
  object GroupBox1: TGroupBox
    Left = 337
    Top = 170
    Width = 315
    Height = 108
    Caption = 'Output'
    TabOrder = 8
    object ckOutput2: TCheckBox
      Left = 174
      Top = 23
      Width = 128
      Height = 17
      Caption = 'AGV in ingombro'
      TabOrder = 0
    end
    object ckOutput1: TCheckBox
      Left = 20
      Top = 25
      Width = 131
      Height = 17
      Caption = 'Richiesta ingresso'
      TabOrder = 1
    end
    object BitBtn2: TBitBtn
      Tag = 1
      Left = 212
      Top = 65
      Width = 100
      Height = 25
      Caption = 'Set'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      Visible = False
      OnClick = BitBtn1Click
    end
    object ckOutput3: TCheckBox
      Left = 20
      Top = 55
      Width = 131
      Height = 17
      Caption = 'Pallet prelevato'
      TabOrder = 3
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 5
    Top = 4
  end
end
