object dmDB: TdmDB
  OldCreateOrder = False
  OnDestroy = DataModuleDestroy
  Height = 270
  Width = 291
  object ADOConnection1: TADOConnection
    CommandTimeout = 10
    Connected = True
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=pwd;Persist Security Info=True;User' +
      ' ID=sa;Initial Catalog=DivaLgv;Data Source=127.0.0.1'
    ConnectionTimeout = 5
    ConnectOptions = coAsyncConnect
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    AfterConnect = ADOConnection1AfterConnect
    OnConnectComplete = ADOConnection1ConnectComplete
    Left = 40
    Top = 16
  end
  object TimerPuliziaStorici: TTimer
    Interval = 60000
    OnTimer = TimerPuliziaStoriciTimer
    Left = 38
    Top = 147
  end
  object TimerConnect: TTimer
    Enabled = False
    Interval = 6000
    OnTimer = TimerConnectTimer
    Left = 38
    Top = 78
  end
  object QueryMissioniSocket: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 144
    Top = 16
  end
  object AlarmTable: TADOTable
    Connection = ADOConnection1
    Left = 112
    Top = 84
  end
  object MissTable: TADOTable
    Connection = ADOConnection1
    Left = 157
    Top = 144
  end
  object QueryNewAlarm: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 224
    Top = 16
  end
  object QueryPos: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 167
    Top = 86
  end
  object QueryMappa: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 224
    Top = 87
  end
  object QueryCorsie: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 224
    Top = 167
  end
  object QueryTipologie: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 224
    Top = 215
  end
  object FMConnection: TADOConnection
    CommandTimeout = 10
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=pwd;Persist Security Info=True;User' +
      ' ID=sa;Initial Catalog=DivaLgv;Data Source=127.0.0.1'
    ConnectionTimeout = 5
    ConnectOptions = coAsyncConnect
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    AfterConnect = ADOConnection1AfterConnect
    OnConnectComplete = ADOConnection1ConnectComplete
    Left = 46
    Top = 209
  end
end
