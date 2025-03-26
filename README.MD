## Problem Overview

There are five companies developing eVTOL aircraft. The vehicle produced by each manufacturer has different characteristics. <br/>
Six distinct properties are laid out in the below table:

| Company Name                    | Alpha Company | Bravo Company | Charlie Company | Delta Company | Echo Company |
| ------------------------------- | ------------- | ------------- | --------------- | ------------- | ------------ |
| Cruise Speed (mph)              |      120      |      100      |       160       |      90       |      30      |
| Battery Capacity (kWh)          |      320      |      100      |       220       |      120      |      150     |
| Time to Charge (hours)          |      0.6      |      0.2      |       0.8       |      0.62     |      0.3     |
| Energy use at Cruise (kWh/mile) |      1.6      |      1.5      |       2.2       |      0.8      |      5.8     |
| Passenger Count                 |      4        |      5        |       3         |      2        |      2       |
| Probability of fault per hour   |      0.25     |      0.10     |       0.05      |      0.22     |      0.61    |

You will simulate using these vehicle for 3 hours. Of course your simulation should take much less time than that.  <br/>
20 total vehicles should be deployed, and a random number of each type of vehicle should be used (with the total between all five types being 20). <br/>

There are only three chargers available for all 20 vehicles! A single charger can only be used by one vehicle at a time.  <br/>
Assume the chargers can charge the batteries in the Time to Charge time listed for each vehicle.  <br/>

Keep track of the following statistics per vehicle type:  <br/>
- average flight time per flight  <br/>
- average distance traveled per flight  <br/>
- average time charging per charge session  <br/>
- total number of faults  <br/>
- total number of passenger miles.  <br/>
	For example, if there are 2 vehicles carrying 4 passengers on a vehicle that cruises for 1 hour at 100 mph, total number of passenger miles is 2 * 4 * 1 * 100 = 800.

Assume that:
- Each vehicle starts the simulation with a fully-charged battery
- Each vehicle instantaneously reaches Cruise Speed
- Each vehicle is airborne for the full use of the battery, and is immediately in line for the charger after running out of battery power.

Please include the statistics recorded during at least one run of the simulation in your submission.

Please do not hesitate to reach out to ask any questions about the problem!  <br/>
However, if you find yourself making an assumption that we haven’t explained explicitly, you may simply document your assumption.  <br/>
It’s interesting to us to hear what assumptions you made and why.


## Design Rationale (OOP, modularity)

## Build Instructions

1. Install CMake
```
sudo apt update
sudo apt install cmake
```

2. Verify
```
cmake --version
```

3. Navigate to build directory:
```
cd ~/dev/sandbox/c++/evtol_sim/build
```

4. Clean and reconfigure:
```
rm -rf *
cmake ..
make
```

5. Run
```
./evtol_sim
```


## Sample output explanation
```
matthew@flopt:~/dev/sandbox/c++/evtol_sim/build$ ./evtol_sim
Basic unit tests passed

Alpha (3 vehicles):
Avg Flight Time: 0.292222 hours
Avg Distance: 35.0667 miles
Avg Charge Time: 0.506667 hours
Total Faults: 3
Total Passenger Miles: 12624

Bravo (7 vehicles):
Avg Flight Time: 0.098254 hours
Avg Distance: 9.8254 miles
Avg Charge Time: 0.466667 hours
Total Faults: 0
Total Passenger Miles: 10316.7

Charlie (6 vehicles):
Avg Flight Time: 0.06875 hours
Avg Distance: 11 miles
Avg Charge Time: 0.483333 hours
Total Faults: 0
Total Passenger Miles: 5940

Delta (3 vehicles):
Avg Flight Time: 0.298889 hours
Avg Distance: 26.9 miles
Avg Charge Time: 0.56 hours
Total Faults: 1
Total Passenger Miles: 4842

Echo (1 vehicles):
Avg Flight Time: 0.114943 hours
Avg Distance: 3.44828 miles
Avg Charge Time: 0.733333 hours
Total Faults: 0
Total Passenger Miles: 206.897
```