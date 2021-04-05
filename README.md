# nrf53_blinky

To build:
```
west build -b nrf5340dk_nrf5340_cpuapp
```

To flash:
```
west flash
```

If you get an error like:
```
    FATAL ERROR: command exited with status 16: nrfjprog --program zephyr/zephyr.hex -f NRF53 --snr 960149330 --coprocessor CP_APPLICATION --sectorerase
```

you can "recoved" with the command:
```
nrfjprog --recover -f NRF53 
```
