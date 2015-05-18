#if defined QUAN_OSD_TELEM_RECEIVER
#include "rx_telemetry.cpp"
#else
#if defined QUAN_OSD_TELEM_TRANSMITTER
#include "tx_telemetry.cpp"
#endif
#endif
