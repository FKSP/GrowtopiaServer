# Known Packets.

## Server
```
OnConsoleMessage
 -> string (data)
 
OnRequestWorldSelectMenu
 -> string (data)
 -> data (
        default|{WorldName}\n
                   (ButtonText)    (MultiChoice) (Size)(ButtonColor)
        add_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\n
        (foreach world)                                   (ButtonColor)
            add_floater|{World_Name}|{Joined_Users}|{Size}|3529161471\n
    )

# not a usual packet! but really fun to play!
action|play_sfx\n
file|audio/ogg/suspended.ogg\n
delayMS|0\n
```
## Client
```

```


## List of all known packet names
```
OnReconnect
OnSpawn
OnRemove
OnRequestWorldSelectMenu
OnConsoleMessage
OnTalkBubble
OnTextOverlay
OnDialogRequest
OnStoreRequest
OnStorePurchaseResult
OnZoomCamera
OnPinchMod
OnFailedToEnterWorld
SetHasGrowID
OnInitialLogonAccepted
OnSetBux
OnParticleEffect
OnItemEffect
OnAddNotification
OnForceTradeEnd
OnTradeStatus
OnDisableRemoveIAPScreen
OnSendToServer
OnToggleBetaMode
OnSetBetaMode
OnSetBetaSilent
OnSendIAPConfirmation
OnSetBaseWeather
OnSetCurrentWeather
```