import os
import pygame
import cv2
import sys

# 设置窗口位置（左上角坐标）
window_position_x, window_position_y = 100, 100  # 根据你的需求调整位置
os.environ['SDL_VIDEO_WINDOW_POS'] = f"{window_position_x},{window_position_y}"

# 初始化Pygame
pygame.init()

# 视频文件路径
video_path = '/home/zhz/Desktop/video.mp4'

# 打开视频文件
cap = cv2.VideoCapture(video_path)

# 检查视频是否成功打开
if not cap.isOpened():
    print("Error: Failed to open video file.")
    sys.exit()

# 设置窗口大小
window_width, window_height = 300, 250
screen = pygame.display.set_mode((window_width, window_height), pygame.NOFRAME)  # NOFRAME 隐藏边框

# 设置帧率
clock = pygame.time.Clock()

# 循环播放视频帧
running = True
while running:
    ret, frame = cap.read()

    # 视频播放完毕后从头开始
    if not ret:
        cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
        continue

    # 调整帧的大小与窗口匹配
    frame = cv2.resize(frame, (window_width, window_height))

    # 1. 垂直翻转帧 (0 表示垂直翻转)
    flipped_frame = cv2.flip(frame, 0)

    # 2. 逆时针旋转 90 度
    rotated_frame = cv2.transpose(flipped_frame)

    # 3. 水平翻转（左右对称）
    mirrored_frame = cv2.flip(rotated_frame, 1)

    # 转换 BGR 到 RGB (cv2 用 BGR，而 Pygame 使用 RGB)
    frame_rgb = cv2.cvtColor(mirrored_frame, cv2.COLOR_BGR2RGB)

    # 将OpenCV帧转换为Pygame图像
    frame_surface = pygame.surfarray.make_surface(frame_rgb)

    # 显示帧
    screen.blit(frame_surface, (0, 0))
    pygame.display.update()

    # 事件处理
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # 按下 'q' 键退出
    keys = pygame.key.get_pressed()
    if keys[pygame.K_q]:
        running = False

    # 控制帧率
    clock.tick(25)  # 控制帧率为 25 帧每秒

# 释放视频资源
cap.release()
pygame.quit()
sys.exit()
