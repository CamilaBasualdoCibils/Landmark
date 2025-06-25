#include "DeviceList.h"

device_list::device_list(const device_container & _d)
{
    devices = _d;
}

void device_list::Filter(const filter_mask &mask)
{
    for( int i = 0; i < devices.size();i++) {
        
    }
}
