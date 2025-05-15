"""
Date: 2025-03-04 21:17:21
LastEditTime: 2025-03-22 15:25:32
Description:

"""

import serial
from serial.tools import list_ports


def device_find(devide_keyword: str) -> serial.Serial:
    """查找并自动连接串口设备，波特率115200

    Args:
        devide_keyword (str):设备关键字

    Returns:
        str: 串口设备
    """
    ports = list_ports.comports()
    device: serial.Serial = "OpenMV"

    for port in ports:
        # print(port.description)
        if devide_keyword in port.description:
            device = serial.Serial(port.device, 115200)
    if device is None:
        raise ValueError("Device not found")
    device.reset_input_buffer()
    return device
