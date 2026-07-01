# note-zephyr (Cygnet)

Zephyr RTOS Example for Notecard on the Blues Cygnet.

This is the Swan Zephyr example retargeted for Cygnet; the application code is identical. Build with the upstream Blues Cygnet board target:

```
west build -b cygnet
west flash
```

(The Swan version of this example uses `west build -b swan_r5`.)
