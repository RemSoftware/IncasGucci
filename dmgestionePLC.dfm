object TDMgestionePLC: TTDMgestionePLC
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 281
  Width = 360
  object TimerEventi: TTimer
    Enabled = False
    OnTimer = TimerEventiTimer
    Left = 32
    Top = 24
  end
  object TimerPlc: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerPlcTimer
    Left = 104
    Top = 24
  end
  object TimerPhoenix: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerPhoenixTimer
    Left = 106
    Top = 104
  end
  object TimerAllarmi: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = TimerAllarmiTimer
    Left = 170
    Top = 34
  end
  object TimerSemaforo: TTimer
    Enabled = False
    OnTimer = TimerSemaforoTimer
    Left = 195
    Top = 135
  end
end
