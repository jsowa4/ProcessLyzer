# ProcessLyzer v1.2
## Purpose
This command lines application was created to track the different processes running on your linux machine, as a form of process logging. It initiates a scan of all processes running on your machine, every 10 seconds, and updates you on any new processes found via command line. New processes are tracked in data/proc_history.txt.
## How To Run ProcessLyzer (Linux Only)
At the moment, the application can only be run via CMake and the subsequently generated Makefile:

1. Run `cmake CMakeLists.txt` to generate the make file.
2. Make the exectuable using `make`.
3. Run the application: `./ProcessLyzer`.

## Future Development
The following features are in development:
- More execution options, such as time between scans and ignored processes
- Analysis of abnormal CPU usage
- GUI Application
- And more!
