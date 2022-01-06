# TBTelescope-TrackingInfo
Home-made module for Corryvreckan - Telescope raw data analysis software

Thanks to the helps from Drs Adrian Herkert, Oleksandr Borysov, and Roma Bugiel.

## USAGE

1. Clone the folder to `[corryvrackan]/src/modules/`
2. Go to `[corryvrackan]/build` or other customised build folder, and re-make the software by:
```
cmake
make
make install
```
3. Add the following lines in the .conf file:
```
[TrackingInfo]
z_ref = 1000    # by default the reference plane is set at 1000 mm
z_dut = 1500    # change the number to the DUT's z coordinate in mm
```

## OUTPUT
Like the other module's, the information extracted by this module should appear in the output root file of Corryvreckan under the `TDirectory TrackingInfo`, where a `TTree Tracks` can be found. The TTree contains the following entries:
- `vector x/y_ref`: the transverse coordinates of the track(s) hitting on the *reference plane*
- `vector x/y_dut`: the transverse coordinates of the track(s) hitting on the *detector's plane*
- `vector triggerid`: the universal/extended trigger-logic-unit (TLU) ID for each event
- `vector timestamp`: the time of each event
Notes:
1. The number of track(s) in each event, namely a period of time determined by the telescope, could be 0, 1 (the most probable case), or even more.
2. The raw TLU ID is a signed 16-bit integer ranges from 0 to 32767.
3. The unit of timestamp looks like nanosecond but to-be confirmed. It's unclear when the time was recorded (namely, at the beginning, end, or in the middle of an event).
