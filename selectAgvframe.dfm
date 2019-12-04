object frSelectAgv: TfrSelectAgv
  Left = 0
  Top = 0
  Width = 1225
  Height = 44
  Color = clBtnFace
  ParentColor = False
  TabOrder = 0
  object mainPanel: TPanel
    Left = 3
    Top = 2
    Width = 1219
    Height = 39
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object lbTitolo: TLabel
      Left = 16
      Top = 8
      Width = 136
      Height = 19
      Caption = 'xxxxxxxxxxxxxxxxx'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 967
      Top = 8
      Width = 86
      Height = 16
      Caption = 'Mission Priority'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object GroupBox1: TGroupBox
      Left = 512
      Top = 0
      Width = 417
      Height = 33
      Color = clGradientInactiveCaption
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBackground = False
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      object cbAgv2: TCheckBox
        Left = 69
        Top = 6
        Width = 49
        Height = 17
        Caption = 'Agv 2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object cbAgv7: TCheckBox
        Left = 344
        Top = 6
        Width = 49
        Height = 17
        Caption = 'Agv 7'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
      end
      object cbAgv6: TCheckBox
        Left = 289
        Top = 6
        Width = 49
        Height = 17
        Caption = 'Agv 6'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
      end
      object cbAgv5: TCheckBox
        Left = 234
        Top = 6
        Width = 49
        Height = 17
        Caption = 'Agv 5'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
      end
      object cbAgv4: TCheckBox
        Left = 179
        Top = 6
        Width = 49
        Height = 17
        Caption = 'Agv 4'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
      end
      object cbAgv3: TCheckBox
        Left = 124
        Top = 6
        Width = 49
        Height = 17
        Caption = 'Agv 3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 5
      end
      object cbAgv1: TCheckBox
        Left = 14
        Top = 6
        Width = 49
        Height = 17
        Caption = 'Agv 1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 6
      end
    end
    object cbPriority1: TComboBox
      Left = 1059
      Top = 5
      Width = 153
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Items.Strings = (
        '0 (Very High)'
        '1 (High)'
        '2 (Medium)'
        '3 (Low)'
        '4 (Very Low)'
        '5 (Very Very Low)')
    end
  end
end
