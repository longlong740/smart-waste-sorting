import cv2
import os
import numpy as np
import random

# 图像增强函数
def augment_image(image):
    # 随机水平翻转
    if random.random() > 0.5:
        image = cv2.flip(image, 1)
    
    # 随机旋转（限制旋转角度在 ±15° 以内）
    angle = random.uniform(-30, 30)
    h, w = image.shape[:2]
    M = cv2.getRotationMatrix2D((w/2, h/2), angle, 1)
    image = cv2.warpAffine(image, M, (w, h))
    
    # 随机缩放（限制缩放范围在 0.9 到 1.1）
    scale = random.uniform(0.9, 1.1)
    image = cv2.resize(image, None, fx=scale, fy=scale)
    
    # 随机平移（限制平移距离在图像宽度/高度的10%以内）
    tx = random.randint(-int(w * 0.1), int(w * 0.1))
    ty = random.randint(-int(h * 0.1), int(h * 0.1))
    M = np.float32([[1, 0, tx], [0, 1, ty]])
    image = cv2.warpAffine(image, M, (w, h))
    
    # 随机调整亮度和对比度（限制在 ±20% 以内）
    value = random.randint(-30, 30)
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    hsv[:, :, 2] = cv2.add(hsv[:, :, 2], value)
    image = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)
    return image
# 数据扩充函数
def augment_dataset(input_dir, output_dir, augmentation_factor=5):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # 遍历输入文件夹中的所有图片
    for filename in os.listdir(input_dir):
        if filename.endswith('.jpg') or filename.endswith('.png'):
            img_path = os.path.join(input_dir, filename)
            image = cv2.imread(img_path)
            
            # 原图保存一份
            cv2.imwrite(os.path.join(output_dir, filename), image)
            
            # 生成增强图像
            for i in range(augmentation_factor):
                augmented_image = augment_image(image)
                augmented_filename = f"{os.path.splitext(filename)[0]}_aug_{i}.jpg"
                cv2.imwrite(os.path.join(output_dir, augmented_filename), augmented_image)

# 输入文件夹路径（原始图像数据）
input_dir = 'C:\\Users\\lenovo\\Desktop\\turnip1'
# 输出文件夹路径（存储扩充后的图像）
output_dir = 'C:\\Users\\lenovo\\Desktop\\turnip2'

# 扩充数据集（扩充5倍）
augment_dataset(input_dir, output_dir, augmentation_factor=5)
