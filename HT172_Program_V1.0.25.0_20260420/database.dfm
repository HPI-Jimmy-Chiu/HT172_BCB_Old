object DataModule1: TDataModule1
  OldCreateOrder = True
  Left = 281
  Top = 214
  Height = 482
  Width = 618
  object UserActionList: TActionList
    Left = 32
    Top = 16
    object InitialMotorName: TAction
      Caption = 'InitialMotorName'
      OnExecute = InitialMotorNameExecute
    end
    object SpecificSetupForMotorParameter: TAction
      Caption = 'SpecificSetupForMotorParameter'
      OnExecute = SpecificSetupForMotorParameterExecute
    end
    object InitialCylinderName: TAction
      Caption = 'InitialCylinderName'
      OnExecute = InitialCylinderNameExecute
    end
    object SpecificSetupForCylinderParameter: TAction
      Caption = 'SpecificSetupForCylinderParameter'
      OnExecute = SpecificSetupForCylinderParameterExecute
    end
    object InitialSensorName: TAction
      Caption = 'InitialSensorName'
    end
    object SpecificSetupForSensorParameter: TAction
      Caption = 'SpecificSetupForSensorParameter'
      OnExecute = SpecificSetupForSensorParameterExecute
    end
    object InitialSwitchName: TAction
      Caption = 'InitialSwitchName'
      OnExecute = InitialSwitchNameExecute
    end
    object SpecificSetupForSwitchParameter: TAction
      Caption = 'SpecificSetupForSwitchParameter'
      OnExecute = SpecificSetupForSwitchParameterExecute
    end
    object InitialSuckerName: TAction
      Caption = 'InitialSuckerName'
      OnExecute = InitialSuckerNameExecute
    end
    object SpecificSetupForSuckerParameter: TAction
      Caption = 'SpecificSetupForSuckerParameter'
      OnExecute = SpecificSetupForSuckerParameterExecute
    end
    object Initial_IO_Setup: TAction
      Caption = 'Initial_IO_Setup'
      OnExecute = Initial_IO_SetupExecute
    end
  end
  object UserMotion: TActionList
    Left = 116
    Top = 16
    object ActionTrack1: TAction
      Caption = 'ActionTrack1'
      OnExecute = ActionTrack1Execute
    end
    object ActionTrack2: TAction
      Caption = 'ActionTrack2'
      OnExecute = ActionTrack2Execute
    end
    object ActionTrack3: TAction
      Caption = 'ActionTrack3'
      OnExecute = ActionTrack3Execute
    end
    object ActionTrack4: TAction
      Caption = 'ActionTrack4'
      OnExecute = ActionTrack4Execute
    end
    object Loader: TAction
      Caption = 'Loader'
      OnExecute = LoaderExecute
    end
    object ActionEmpty1: TAction
      Caption = 'ActionEmpty1'
      OnExecute = ActionEmpty1Execute
    end
    object ActionEmpty1_Track: TAction
      Caption = 'ActionEmpty1_Track'
      OnExecute = ActionEmpty1_TrackExecute
    end
    object ActionEmpty2: TAction
      Caption = 'ActionEmpty2'
      OnExecute = ActionEmpty2Execute
    end
    object ActionStoreArm: TAction
      Caption = 'ActionStoreArm'
      OnExecute = ActionStoreArmExecute
    end
    object SortArm1: TAction
      Caption = 'SortArm1'
      OnExecute = SortArm1Execute
    end
    object SortArm2: TAction
      Caption = 'SortArm2'
      OnExecute = SortArm2Execute
    end
    object ActionMag1: TAction
      Caption = 'ActionMag1'
      OnExecute = ActionMag1Execute
    end
    object ActionMag2: TAction
      Caption = 'ActionMag2'
      OnExecute = ActionMag2Execute
    end
    object ActionMag3: TAction
      Caption = 'ActionMag3'
      OnExecute = ActionMag3Execute
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer1Timer
    Left = 36
    Top = 164
  end
end
