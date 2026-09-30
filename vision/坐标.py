import cv2

# 初始化摄像头
cap = cv2.VideoCapture(0)

# 输入像素点坐标
input_x = 100  # 示例 x 坐标
input_y = 100  # 示例 y 坐标

while True:
    # 捕获视频帧
    ret, frame = cap.read()
    if not ret:
        break

    # 在指定坐标绘制红点
    cv2.circle(frame, (input_x, input_y), 3, (0, 0, 255), -1)  # 红色圆点，半径为5

    # 显示图像
    cv2.imshow('Camera Feed', frame)

    # 按 'q' 键退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放摄像头和窗口
cap.release()
cv2.destroyAllWindows()
