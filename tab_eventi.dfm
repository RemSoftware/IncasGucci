inherited fTabEventi: TfTabEventi
  Caption = 'Missioni Gestionale WMS'
  ClientHeight = 575
  ClientWidth = 1484
  OnActivate = FormActivate
  ExplicitWidth = 1500
  ExplicitHeight = 614
  PixelsPerInch = 96
  TextHeight = 19
  inherited PanelButtons: TPanel
    Left = 1359
    Top = 105
    Width = 125
    Height = 470
    ExplicitLeft = 1359
    ExplicitTop = 105
    ExplicitWidth = 125
    ExplicitHeight = 470
    object Label2: TLabel [0]
      Left = 0
      Top = 19
      Width = 122
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = 'Numero Record'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Comic Sans MS'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Tot: TLabel [1]
      Left = 1
      Top = 44
      Width = 122
      Height = 19
      Alignment = taCenter
      AutoSize = False
      Caption = '0'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Comic Sans MS'
      Font.Style = [fsBold]
      ParentFont = False
    end
    inherited PanelClose: TPanel
      Top = 384
      Width = 123
      ExplicitTop = 384
      ExplicitWidth = 123
      inherited BitBtnClose: TBitBtn
        Width = 117
        Glyph.Data = {00000000}
        ExplicitWidth = 117
      end
    end
  end
  inherited PanelTitle: TPanel
    Width = 1484
    Caption = 'Tabella Stato Baie'
    ExplicitWidth = 1484
  end
  object Panel2: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 43
    Width = 1478
    Height = 59
    Align = alTop
    Alignment = taLeftJustify
    BevelWidth = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object Label3: TLabel
      Left = 8
      Top = 23
      Width = 32
      Height = 16
      Caption = 'Filtri:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object NMisCheck: TCheckBox
      Left = 44
      Top = 6
      Width = 89
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Id'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = NMisCheckClick
    end
    object NMis: TCSpinEdit
      Left = 46
      Top = 29
      Width = 89
      Height = 22
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Value = 1
      OnChange = NMisCheckClick
    end
    object CheckBox2: TCheckBox
      Left = 158
      Top = 6
      Width = 131
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Nome Baia'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = NMisCheckClick
    end
    object Destinazione1: TEdit
      Left = 158
      Top = 29
      Width = 131
      Height = 21
      TabOrder = 3
      OnChange = NMisCheckClick
    end
    object CheckBox6: TCheckBox
      Left = 310
      Top = 6
      Width = 131
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Stato Fotocellula'
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = NMisCheckClick
    end
    object Edit2: TEdit
      Left = 310
      Top = 29
      Width = 131
      Height = 21
      TabOrder = 5
      OnChange = NMisCheckClick
    end
  end
  object DBGrid1: TDBGrid
    AlignWithMargins = True
    Left = 3
    Top = 108
    Width = 1353
    Height = 464
    Align = alClient
    DataSource = DataSource1
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgCancelOnExit]
    ParentFont = False
    TabOrder = 3
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'Id'
        Title.Alignment = taCenter
        Title.Font.Charset = DEFAULT_CHARSET
        Title.Font.Color = clWindowText
        Title.Font.Height = -13
        Title.Font.Name = 'MS Sans Serif'
        Title.Font.Style = []
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'NomeBaia'
        Title.Alignment = taCenter
        Title.Font.Charset = DEFAULT_CHARSET
        Title.Font.Color = clWindowText
        Title.Font.Height = -13
        Title.Font.Name = 'MS Sans Serif'
        Title.Font.Style = []
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'StatoFotocellula'
        Title.Alignment = taCenter
        Title.Font.Charset = DEFAULT_CHARSET
        Title.Font.Color = clWindowText
        Title.Font.Height = -13
        Title.Font.Name = 'MS Sans Serif'
        Title.Font.Style = []
        Visible = True
      end>
  end
  object DataSource1: TDataSource
    DataSet = Query1
    Left = 24
    Top = 120
  end
  object Query1: TADOQuery
    Connection = dmDB.ADOConnection1
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      'Select * from statobaie')
    Left = 168
    Top = 176
  end
end
