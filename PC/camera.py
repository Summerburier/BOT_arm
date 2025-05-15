import serial
import struct
import cv2
import numpy as np
import json
from find_device import device_find
import time
from VOL import detect_objects
import pdb


class Camera:

    def __init__(self):
        self.device: serial.Serial = device_find("COM7")
        self.camera_matrix = np.array(
            [
                [201.49118833614946, 0.0, 163.74569446915814],
                [0.0, 195.2758652143091, 118.47406147279513],
                [0.0, 0.0, 1.0],
            ]
        )
        self.dis_coeffs = np.array(
            [
                -0.18642619985136755,
                0.007485318084083075,
                -0.002027215946539897,
                0.006413127487293533,
                0.13618506952983916,
            ]
        )

    def ori_snap(self) -> np.ndarray:
        """拍摄原图"""
        try:
            self.device.write(b"snap")
            self.device.flush()
            size = struct.unpack("<L", self.device.read(4))[0]
            if size > 55000:
                raise ValueError(f"Image size too large:{size}")
            data = self.device.read(size)
            if len(data) != size:
                print(f"Warning: 期望{size}字节，实际收到{len(data)}字节")
            return cv2.imdecode(
                np.frombuffer(data, dtype=np.uint8), cv2.IMREAD_COLOR
            )
        except Exception as e:
            while True:
                print(e)
                try:
                    self.close()
                    self.device = device_find("COM7")
                    self.device.write(b"snap")
                    self.device.flush()
                    size = struct.unpack("<L", self.device.read(4))[0]
                    if size == 0:
                        continue
                    if size > 55000:
                        raise ValueError(f"Image size too large:{size}")
                    data = self.device.read(size)
                    if len(data) != size:
                        print(f"Warning: 期望{size}字节，实际收到{len(data)}字节")
                    return cv2.imdecode(
                        np.frombuffer(data, dtype=np.uint8),
                        cv2.IMREAD_COLOR,
                    )
                except:
                    print("Reconnecting...")
                    time.sleep(2)
                    continue

    def snap(self) -> np.ndarray:
        """拍摄矫正后的图像"""
        img = self.ori_snap()
        return cv2.undistort(
            img, self.camera_matrix, self.dis_coeffs, None, self.camera_matrix
        )

    def close(self):
        if self.device.is_open:
            self.device.close()


def main():
    openmv = Camera()
    while True:
        img = openmv.snap()  # 矫正后的原图
        objects, img1 = detect_objects(img)  # img1为带识别框的图像

        cv2.imshow("Original", img)   # 显示原图
        cv2.imshow("Detected", img1)  # 显示识别结果

        print(objects)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q") or key == 27:
            break
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
