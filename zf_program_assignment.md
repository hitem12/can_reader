Assume linux environment running systemd.
- [x] Create a CMake project called "can_reader" which builds a C++17 application.
- [ ] In the CMake install section, install the application to /usr/bin (standard Linux bin directory) and install a corresponding systemd service.
-[ ] The systemd service should be enabled by default during installation.

The main application shall do the following:
-[x] provide command line argument -h and --help which triggers printing of usage information. If this parameter is present, exit with the success code regardles of other arguments.
-[x] provide command line argument -i and --interface which specifies a socket CAN interface name, e.g. "-i can1", this argument is required

-[ ] after parsing command line arguments, connect to the CAN interface specified on the command line via -i
-[ ] install a CAN filter to only process extended CAN frames matching the following CAN ID and mask 0x00112200:0x00FFFF00
-[ ] print all accepted CAN frames on stdout in form <can id in uppercase hex>,<data in uppercase hex>, e.g. 1F112211,AABBCCDDEEFF
-[ ] if the CAN ID matches exactly 0x1F1122EE, decode the CAN frame data part uing the following description
    - byte 0 and 1 -> interpret as a little-endian uint16 value representing engine RPM
    - byte 1 and 2 -> interpret as a big-endian uint16 value representing vehicle speed in km/h, the value must be scaled using a factor 0.01, e.g. 0x00FF -> 2.55km/h
    - byte 3       -> interpret as a uint8 value representing engine temperature in deg C, the value must be shifted using an offset of 60, e.g. 0x20 -> -28 C
-[ ] print the additional parsed information (if the CAN ID matches) in form <key>:<value> delimited by a space on a separate line, e.g. rpm:1000 speed:25.4, temp:58
-[ ] print any errors on stderr

[ ] Pack the entire project into a ZIP archive and send it at <david.fabian@zf.com>. The deadline date is specified in the email.
