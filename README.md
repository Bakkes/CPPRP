# CPPRP

Rocket League replay parser written in C++. The project has been tested on Ubuntu (only tested 64 bit) and Windows (32 and 64 bit).

CPPRPTest currently contains only internal test things/benchmarking, so probably not very relevant. 

# CPPRP
The core library is CPPRP, and should be built as a dynamic/static library so it can be included in other projects. From testing, the library should be able to parse all replays up to and including Rocket League v1.66. I've ran tests on a set of 10k replays from between mid 2017 till august 2019. Out of this set, 6 could not be parsed (for unknown reasons). I've also ran some random sampling tests on the (now defunct) RocketLeagueReplays replay dump, and on those there seems to be 99.6% coverage.

# CPPRPJSON
Uses CPPRP to parse the replay and then serializes the replaydata to JSON. Currently, it is able to serialize any and all fields, except for some relating to the client's loadout (WIP). Also the code's a mess but it's still a WIP.

# Functionality

CPPRPBM is a BakkesMod plugin which will be able to play certain parts of the replay back ingame (unfinished).
CPPRPTest contains some tests and is probably not set up to work on other computers at the moment.
LoadoutExtractor is a binary which prints out camera settings for all the players in the replay as well as share codes for the loadouts which can be imported in BakkesMod.

# Scripts
Since C++ doesn't have reflection, but we still need to generate a lot of code for the parsing/serialization logic, I wrote some preprocessor scripts which outputs code to automatically parse certain classes/properties.

# Usage
The CPPRPJSON accepts the following arguments
```
-i  --input     Path to the input file
-o  --output    Path to write output json to
-ho --header    When passed, will only parse and output the header
    --stdout    Will print the JSON output to the terminal
    --print     Same as --stdout
```

Some notes:
if -i or --input isn't specified, it will take the first loose argument, so you can do `CPPRPJSON test.replay`
if -o or --output is not given, it will automatically print json output to the terminal.

# TODO
- [ ] Better organize this readme
