# ESP CSP LoRa

This is an example project for getting libcsp running on the ESP32 and
utilizing LoRa technology. The code is designed to run on the LoRa32 V2.1_1.6,
and it uses the driver for the LoRa chip found at
[nopnop2002/esp-idf-sx127x](https://github.com/nopnop2002/esp-idf-sx127x).

Inside this repository, you can find the CSP source code at
[libcsp/libcsp](https://github.com/libcsp/libcsp), along with the necessary
code to enable CSP to utilize the LoRa chip on the board.

![Setup](img/fig.png)

The current capabilities of this project are illustrated in the figure above.
The setup consists of two Lilligo LoRa boards. One of the boards can serve as a
UDP bridge, allowing communication with the other LoRa board through UDP ->
LoRa.


In addition this project also implements vmem storage in the esp32 flash. This
is done in the `vmem_flash.c` file. It utilizes the additional libparam flash
partition sepcified in the partitioning table `partitions.csv`. This allows
storage of configuration paramters between reboots and in the future could
allow implementation of the multiple flash slots used on satellites. This would
requiere implementing hooks on the existing esp32 bootloader.
