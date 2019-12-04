object DMGestione_Porte: TDMGestione_Porte
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Height = 357
  Width = 556
  object TimerEventi: TTimer
    Enabled = False
    OnTimer = TimerEventiTimer
    Left = 32
    Top = 24
  end
  object TimerPlcPorte: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerPlcPorteTimer
    Left = 104
    Top = 24
  end
end
