# Known Packets.

## Server
```
OnConsoleMessage
 -> string (message)
 
OnRequestWorldSelectMenu
 -> string (WorldOffsers)
 -> WorldOffsers (
        default|{WorldName}\n
                   (ButtonText)    (MultiChoice) (Size) (Unknown?)
        add_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\n
        (foreach world)                                   (Unknown?)
            add_floater|{World_Name}|{Joined_Users}|{Size}|3529161471\n
    )
    

```
## Client
```

```