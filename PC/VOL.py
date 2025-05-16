from ultralytics import YOLO
import numpy as np
import cv2

# 加载模型（只加载一次，避免每帧都加载）
model = YOLO(r'./PC/best.pt')
# 0 梯形
# 1 圆形
# 2 三角形
# 3 六边形
# 4 矩形
def detect_objects(image: np.ndarray):
    """
    输入: OpenCV读取的ndarray图像
    输出: 检测到的对象列表，每个对象为 (类别, (中心点x, 中心点y))
    """
    results = model.predict(image, device=0, verbose=False)  # 指定用GPU
    objects = []
    img_draw = image.copy()
    for r in results:
        # r.boxes.xyxy: [N, 4] 检测框坐标 (x1, y1, x2, y2)
        # r.boxes.cls: [N] 类别索引
        for box, cls in zip(r.boxes.xyxy.cpu().numpy(), r.boxes.cls.cpu().numpy()):
            x1, y1, x2, y2 = map(int, box)
            cx, cy = (x1 + x2) // 2, (y1 + y2) // 2
            objects.append((int(cls), (cx, cy)))
            # 画框和类别
            cv2.rectangle(img_draw, (x1, y1), (x2, y2), (0,255,0), 2)
            cv2.putText(img_draw, str(int(cls)), (x1, y1-10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,0,255), 2)
            cv2.circle(img_draw, (cx, cy), 4, (255,0,0), -1)
    return objects, img_draw

# # 示例用法
# if __name__ == '__main__':
    # import cv2
    # img = cv2.imread(r"F:\vscode\yolov11\ultralytics\mydata\img\1747214861664.jpg")
    # objs = detect_objects(img)
    # print(objs)