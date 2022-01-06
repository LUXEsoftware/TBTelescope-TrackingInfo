# TrackingInfo
**Maintainer**: Shan Huang (shan.huang@desy.de)
**Module Type**: *GLOBAL*  
**Status**: Immature

### Description
This is a home made module for telescope raw data analysis software.

Thanks to the helps from Drs Adrian Herkert, Oleksandr Borysov, and Roma Bugiel.

### Parameters
* `double z_ref`: the longitudinal coordinate of a *reference plane*
* `double z_dut`: the longitudinal coordinate of the *detector-under-test (DUT)'s plane*

### Tree produced
* `vector x/y_ref`: the transverse coordinates of the track(s) hitting on the *reference plane*
* `vector x/y_dut`: the transverse coordinates of the track(s) hitting on the *detector's plane*
* `vector triggerid`: the universal/extended trigger-logic-unit (TLU) ID for each event
* `vector timestamp`: the time of each event

### Usage
```toml
[TrackingInfo]
z_ref = 1000    # by default the reference plane is set at 1000 mm
z_dut = 1500    # change the number to the DUT's z coordinate in mm

```
