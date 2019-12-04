object DMGestioneEventi: TDMGestioneEventi
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 357
  Width = 430
  object TimerEventi: TTimer
    Enabled = False
    OnTimer = TimerEventiTimer
    Left = 344
    Top = 31
  end
  object TimerGestioneLuci: TTimer
    Enabled = False
    OnTimer = TimerGestioneLuciTimer
    Left = 331
    Top = 265
  end
  object TimerMissioni: TTimer
    OnTimer = TimerMissioniTimer
    Left = 27
    Top = 21
  end
  object TimerPlc: TTimer
    Enabled = False
    OnTimer = TimerPlcTimer
    Left = 343
    Top = 141
  end
  object TimerPhoenix: TTimer
    Interval = 500
    OnTimer = TimerPhoenixTimer
    Left = 331
    Top = 206
  end
  object TimerMissioniDB: TTimer
    Enabled = False
    Left = 345
    Top = 83
  end
end
