object FormPlcCarico: TFormPlcCarico
  Left = 411
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Plc Signal'
  ClientHeight = 281
  ClientWidth = 519
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 519
    Height = 32
    Align = alTop
    Caption = 'Status Posizione'
    Color = clInactiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
  end
  object Panel2: TPanel
    Left = 0
    Top = 78
    Width = 519
    Height = 148
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object GroupBox3: TGroupBox
      Left = 7
      Top = 6
      Width = 129
      Height = 118
      Caption = 'Stato Posizione'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Label2: TLabel
        Left = 26
        Top = 90
        Width = 36
        Height = 16
        Caption = 'IDUDC'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        Visible = False
      end
      object ckDisabilitaPos: TCheckBox
        Left = 18
        Top = 26
        Width = 95
        Height = 20
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
        Left = 18
        Top = 49
        Width = 83
        Height = 19
        Caption = 'Prenotata'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = cPrenotataPosClick
      end
    end
    object GroupBox8: TGroupBox
      Left = 138
      Top = 6
      Width = 183
      Height = 60
      Caption = 'Piano 1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object Label26: TLabel
        Left = 32
        Top = 25
        Width = 38
        Height = 16
        Caption = 'H.Prel.'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object eH1: TEdit
        Left = 78
        Top = 23
        Width = 95
        Height = 21
        Alignment = taRightJustify
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnChange = cbTipoUDCChange
      end
    end
    object GroupBox2: TGroupBox
      Left = 324
      Top = 8
      Width = 185
      Height = 58
      Caption = 'Piano 2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object Label1: TLabel
        Left = 29
        Top = 28
        Width = 38
        Height = 16
        Caption = 'H.Prel.'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object eH2: TEdit
        Left = 75
        Top = 26
        Width = 95
        Height = 21
        Alignment = taRightJustify
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnChange = cbTipoUDCChange
      end
    end
    object GroupBox1: TGroupBox
      Left = 138
      Top = 66
      Width = 183
      Height = 60
      Caption = 'UDC 1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      object CancellaUDC1btn: TSpeedButton
        Left = 147
        Top = 25
        Width = 28
        Height = 25
        BiDiMode = bdLeftToRight
        Glyph.Data = {
          F6060000424DF606000000000000360000002800000018000000180000000100
          180000000000C006000000000000000000000000000000000000FFFFFF6763CF
          1712B61813BA1812BA1812BA1812BA1712B91712B91712B91711B91711B91711
          B91711B91711B81711B81711B81711B81711B81711B81711B71610B36662CDFF
          FFFF6966D4201DD52724ED2624ED2623ED2622ED2622ED2621ED2621ED2520ED
          2520ED251FED251EED241EED241DED241DED241DED241CED241CED241CED241C
          ED241CED1E17D36763D11A16BD2929EE2829EE2828EE2828EE2220DA2321DF27
          26ED2725ED2725ED2724ED2724ED2624ED2623ED2622ED2622ED2621ED1A15C2
          211CDF2520ED251FED251EED241EED1812B91D19C42A2EEE2A2DEE2A2DEE2526
          E1140FAA140FAA2525E1292AEE2929EE2929EE2829EE2828EE2828EE2827ED28
          26ED1A16C1130DA6140FAA2320E02724ED2624ED2623ED1A15C01E1BC72C33EE
          2C32EE272BE21611AF6C69CA403CBA1611AF2629E22B2FEE2A2EEE2A2DEE2A2D
          EE2A2CEE2A2CEE1C19C5140FABA7A5DF6C69CA1610AF2524E12828EE2828EE1C
          17C31F1DC92D37EE2930E31813B46D6ACDFFFFFFF0F0FA423DBF1813B4282CE3
          2C33EE2C32EE2C31EE2C31EE2324D71611B0A8A5E1FFFFFFFFFFFF6D6ACD1812
          B42526DF2A2CEE1D19C6201FCC2F3DEF2225CE1812B6A9A6E4FFFFFFFFFFFFF1
          F0FA6E6BD11915B92930E42D37EE2D37EE2528D91812B6A8A6E3FFFFFFFFFFFF
          F1F0FB433FC41914B9282CE32C31EE1F1BC92120CE3141EF3141EF2225CE1A14
          BBA9A7E6FFFFFFFFFFFFFFFFFF706CD41B16BF2B34E5272CDB1A14BBA9A7E5FF
          FFFFFFFFFFFFFFFF4540C81B16BE2931E42D37EE2D36EE1F1DCB2222D13346EF
          3245EF3245EF2427D11B15C0AAA7E8FFFFFFFFFFFFFFFFFF716DD71D17C31B15
          C0AAA7E7FFFFFFFFFFFFFFFFFF716DD81D17C32B35E62F3CEE2F3BEE2E3BEE21
          1ECD2423D3344AEF344AEF3449EF3449EF2932DC1D16C4AAA8E9FFFFFFFFFFFF
          FFFFFF716DDA716DDAFFFFFFFFFFFFFFFFFF726DDB1E18C72D3AE73141EF3141
          EF3040EF3040EF2220D02525D5374EF0374DEF364DEF364CEF364CEF2D38E01E
          16C9ABA8EBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF726EDD1F19CB2F3EE8
          3346EF3346EF3245EF3245EF3244EF2322D35656E04C61F23951F03950F03850
          F0384FF0384FF02E3AE21F17CD817DE3FFFFFFFFFFFFFFFFFFFFFFFF736EE01F
          17CD3141E8354BEF354BEF344AEF344AEF3449EF475AF15454DE5D5DE36D7EF4
          6679F45066F23E55F03A52F03A52F03547EA211CD3736FE2FFFFFFFFFFFFFFFF
          FFFFFFFF817DE52018D12F3AE4384EF0374EF03A50F04D60F16274F36879F35B
          5BE15E5EE56E81F56E81F56E80F46E80F45E73F34E5EEE302BDA746FE5FFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFABA8EF2F27D84752EA5C6FF36B7CF46B7C
          F46B7CF46A7CF45D5CE35F5FE77184F57083F57083F57083F56B79F25A56E497
          94EDFFFFFFFFFFFFFFFFFF9E9BEF9490EDFFFFFFFFFFFFFFFFFFC1BFF45953E3
          656EEE6D7EF46C7EF46C7EF46C7EF45D5DE55F60E97286F57286F57186F56D7C
          F25B56E79794EFFFFFFFFFFFFFFFFFFFC1BFF65953E65953E69794F0FFFFFFFF
          FFFFFFFFFFC1BFF65953E66164EC6E81F56E80F46E80F45E5EE76060EA7489F5
          7488F56E7EF35B56EA7873EDF5F4FEFFFFFFFFFFFFC1BFF75953E96973F06D7C
          F35B56EA9794F1FFFFFFFFFFFFFFFFFFC1BFF75953E96265EE7083F57083F55F
          5FE86061EC758BF57080F35C57EC7974EFF5F4FEFFFFFFFFFFFFC1BFF85A54EC
          6A75F17488F57488F56F7EF35C57EC9894F3FFFFFFFFFFFFFFFFFFC1BFF85A54
          EC646AF07185F55F60EA6162EE778DF56E7BF35C58EF9894F5FFFFFFFFFFFFC1
          BFF95A54EE6A73F2758BF5758AF5758AF5758AF57080F45C57EF7974F2F5F4FE
          FFFFFF9894F55C57EF6F7FF47388F56060EB6263EF798FF5798FF57384F45D58
          F09895F6C2BFFA5B55F0666AF2788EF5778DF5778DF5778DF5778CF5778CF571
          82F45D58F07A75F39895F65C58F07081F4758AF5758AF56061ED6263F07B92F6
          7B91F67B91F67586F55D59F25B55F2676BF37A90F57A90F5798FF5798FF5798F
          F5798FF5798FF5788EF57384F45D59F25D59F27283F4778DF5778DF5778CF561
          62EE6161F17D93F67D93F67D93F67D93F67787F56A70F47C92F67C92F67C92F6
          7C92F67B91F67B91F67A91F57A90F57A90F57A90F57485F4707EF4798FF5798F
          F5798FF5798FF5605FEF9797F6717CF47E94F67E94F67E94F67E94F67E94F67E
          94F67E94F67E94F67D93F67D93F67D93F67D93F67D93F67D92F67C92F67C92F6
          7C92F67C92F67B91F67B91F66F7AF39796F5FFFFFF9897F86261F36465F36465
          F36465F36465F36465F36465F36465F36465F36465F36465F36465F36465F364
          65F36465F36465F36465F26464F26464F26160F29797F7FFFFFF}
        ParentBiDiMode = False
        Visible = False
        OnClick = CancellaUDC1btnClick
      end
      object eUDC1: TEdit
        Left = 7
        Top = 28
        Width = 136
        Height = 21
        Alignment = taRightJustify
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Visible = False
        OnChange = cbTipoUDCChange
      end
    end
    object GroupBox4: TGroupBox
      Left = 324
      Top = 66
      Width = 185
      Height = 60
      Caption = 'UDC 2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      object CancellaUDC2btn: TSpeedButton
        Left = 149
        Top = 23
        Width = 28
        Height = 25
        BiDiMode = bdLeftToRight
        Glyph.Data = {
          F6060000424DF606000000000000360000002800000018000000180000000100
          180000000000C006000000000000000000000000000000000000FFFFFF6763CF
          1712B61813BA1812BA1812BA1812BA1712B91712B91712B91711B91711B91711
          B91711B91711B81711B81711B81711B81711B81711B81711B71610B36662CDFF
          FFFF6966D4201DD52724ED2624ED2623ED2622ED2622ED2621ED2621ED2520ED
          2520ED251FED251EED241EED241DED241DED241DED241CED241CED241CED241C
          ED241CED1E17D36763D11A16BD2929EE2829EE2828EE2828EE2220DA2321DF27
          26ED2725ED2725ED2724ED2724ED2624ED2623ED2622ED2622ED2621ED1A15C2
          211CDF2520ED251FED251EED241EED1812B91D19C42A2EEE2A2DEE2A2DEE2526
          E1140FAA140FAA2525E1292AEE2929EE2929EE2829EE2828EE2828EE2827ED28
          26ED1A16C1130DA6140FAA2320E02724ED2624ED2623ED1A15C01E1BC72C33EE
          2C32EE272BE21611AF6C69CA403CBA1611AF2629E22B2FEE2A2EEE2A2DEE2A2D
          EE2A2CEE2A2CEE1C19C5140FABA7A5DF6C69CA1610AF2524E12828EE2828EE1C
          17C31F1DC92D37EE2930E31813B46D6ACDFFFFFFF0F0FA423DBF1813B4282CE3
          2C33EE2C32EE2C31EE2C31EE2324D71611B0A8A5E1FFFFFFFFFFFF6D6ACD1812
          B42526DF2A2CEE1D19C6201FCC2F3DEF2225CE1812B6A9A6E4FFFFFFFFFFFFF1
          F0FA6E6BD11915B92930E42D37EE2D37EE2528D91812B6A8A6E3FFFFFFFFFFFF
          F1F0FB433FC41914B9282CE32C31EE1F1BC92120CE3141EF3141EF2225CE1A14
          BBA9A7E6FFFFFFFFFFFFFFFFFF706CD41B16BF2B34E5272CDB1A14BBA9A7E5FF
          FFFFFFFFFFFFFFFF4540C81B16BE2931E42D37EE2D36EE1F1DCB2222D13346EF
          3245EF3245EF2427D11B15C0AAA7E8FFFFFFFFFFFFFFFFFF716DD71D17C31B15
          C0AAA7E7FFFFFFFFFFFFFFFFFF716DD81D17C32B35E62F3CEE2F3BEE2E3BEE21
          1ECD2423D3344AEF344AEF3449EF3449EF2932DC1D16C4AAA8E9FFFFFFFFFFFF
          FFFFFF716DDA716DDAFFFFFFFFFFFFFFFFFF726DDB1E18C72D3AE73141EF3141
          EF3040EF3040EF2220D02525D5374EF0374DEF364DEF364CEF364CEF2D38E01E
          16C9ABA8EBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF726EDD1F19CB2F3EE8
          3346EF3346EF3245EF3245EF3244EF2322D35656E04C61F23951F03950F03850
          F0384FF0384FF02E3AE21F17CD817DE3FFFFFFFFFFFFFFFFFFFFFFFF736EE01F
          17CD3141E8354BEF354BEF344AEF344AEF3449EF475AF15454DE5D5DE36D7EF4
          6679F45066F23E55F03A52F03A52F03547EA211CD3736FE2FFFFFFFFFFFFFFFF
          FFFFFFFF817DE52018D12F3AE4384EF0374EF03A50F04D60F16274F36879F35B
          5BE15E5EE56E81F56E81F56E80F46E80F45E73F34E5EEE302BDA746FE5FFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFABA8EF2F27D84752EA5C6FF36B7CF46B7C
          F46B7CF46A7CF45D5CE35F5FE77184F57083F57083F57083F56B79F25A56E497
          94EDFFFFFFFFFFFFFFFFFF9E9BEF9490EDFFFFFFFFFFFFFFFFFFC1BFF45953E3
          656EEE6D7EF46C7EF46C7EF46C7EF45D5DE55F60E97286F57286F57186F56D7C
          F25B56E79794EFFFFFFFFFFFFFFFFFFFC1BFF65953E65953E69794F0FFFFFFFF
          FFFFFFFFFFC1BFF65953E66164EC6E81F56E80F46E80F45E5EE76060EA7489F5
          7488F56E7EF35B56EA7873EDF5F4FEFFFFFFFFFFFFC1BFF75953E96973F06D7C
          F35B56EA9794F1FFFFFFFFFFFFFFFFFFC1BFF75953E96265EE7083F57083F55F
          5FE86061EC758BF57080F35C57EC7974EFF5F4FEFFFFFFFFFFFFC1BFF85A54EC
          6A75F17488F57488F56F7EF35C57EC9894F3FFFFFFFFFFFFFFFFFFC1BFF85A54
          EC646AF07185F55F60EA6162EE778DF56E7BF35C58EF9894F5FFFFFFFFFFFFC1
          BFF95A54EE6A73F2758BF5758AF5758AF5758AF57080F45C57EF7974F2F5F4FE
          FFFFFF9894F55C57EF6F7FF47388F56060EB6263EF798FF5798FF57384F45D58
          F09895F6C2BFFA5B55F0666AF2788EF5778DF5778DF5778DF5778CF5778CF571
          82F45D58F07A75F39895F65C58F07081F4758AF5758AF56061ED6263F07B92F6
          7B91F67B91F67586F55D59F25B55F2676BF37A90F57A90F5798FF5798FF5798F
          F5798FF5798FF5788EF57384F45D59F25D59F27283F4778DF5778DF5778CF561
          62EE6161F17D93F67D93F67D93F67D93F67787F56A70F47C92F67C92F67C92F6
          7C92F67B91F67B91F67A91F57A90F57A90F57A90F57485F4707EF4798FF5798F
          F5798FF5798FF5605FEF9797F6717CF47E94F67E94F67E94F67E94F67E94F67E
          94F67E94F67E94F67D93F67D93F67D93F67D93F67D93F67D92F67C92F67C92F6
          7C92F67C92F67B91F67B91F66F7AF39796F5FFFFFF9897F86261F36465F36465
          F36465F36465F36465F36465F36465F36465F36465F36465F36465F36465F364
          65F36465F36465F36465F26464F26464F26160F29797F7FFFFFF}
        ParentBiDiMode = False
        Visible = False
        OnClick = CancellaUDC2btnClick
      end
      object eUDC2: TEdit
        Left = 9
        Top = 26
        Width = 136
        Height = 21
        Alignment = taRightJustify
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Visible = False
        OnChange = cbTipoUDCChange
      end
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 226
    Width = 519
    Height = 55
    Align = alBottom
    BevelOuter = bvLowered
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    object SpeedButton1: TSpeedButton
      Left = 204
      Top = 8
      Width = 115
      Height = 38
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = 'Refresh Mode'
      Visible = False
    end
    object BitBtn4: TBitBtn
      Left = 326
      Top = 8
      Width = 129
      Height = 38
      Caption = 'Chiudi'
      TabOrder = 0
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      Left = 55
      Top = 7
      Width = 129
      Height = 38
      Caption = 'Applica'
      TabOrder = 1
      OnClick = BitBtn5Click
    end
  end
  object Panel5: TPanel
    Left = 0
    Top = 32
    Width = 519
    Height = 46
    Align = alTop
    BevelOuter = bvLowered
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    object Label20: TLabel
      Left = 10
      Top = 16
      Width = 89
      Height = 19
      Caption = 'Posizione :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label22: TLabel
      Left = 112
      Top = 16
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
      Left = 110
      Top = 35
      Width = 98
      Height = 19
      Caption = 'Nomi piani :'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      Visible = False
    end
    object Label24: TLabel
      Left = 283
      Top = 16
      Width = 29
      Height = 19
      Alignment = taRightJustify
      Caption = 'Pos'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 468
      Top = 16
      Width = 29
      Height = 19
      Alignment = taRightJustify
      Caption = 'Pos'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 400
    Top = 65535
  end
end
