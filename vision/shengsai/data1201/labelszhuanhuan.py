import os
import json
import random
import base64
import shutil
import argparse
from pathlib import Path
from glob import glob
from dic_lab import dic_labels

def generate_labels(dic_labs):
    ratio = dic_labs['ratio']
    labels_path = 'labels'  # 修改路径，直接使用labels文件夹
    for index, label_file_path in enumerate(glob(f'{labels_path}/*.txt')):

        # 读取文件名
        image_id = os.path.basename(label_file_path).rstrip('.txt')
        
        # 计算是train 还是 valid
        train_or_valid = 'train' if random.random() < ratio else 'valid'

        # yolo 格式的 lables
        yolo_annotation_path = os.path.join(train_or_valid, 'labels', image_id + '.txt')
        
        # yolo 格式的图像保存
        yolo_image_path = os.path.join(train_or_valid, 'images', image_id + '.jpg')
        
        # 获取位置信息
        with open(label_file_path, 'r') as label_file, open(yolo_annotation_path, 'w') as yolo_annotation_file:
            for line in label_file:
                parts = line.strip().split()
                #object_class = dic_labels[int(parts[0])]  # 假设类别标签是整数
                object_class = parts[0]  # 直接使用parts[0]作为类别索引
                x, y, width, height = map(float, parts[1:])
                
                yolo_annotation_file.write(f'{object_class} {x} {y} {width} {height}\n')

        # 拷贝图像文件
        src_image_path = os.path.join('images', f'{image_id}.jpg')
        dst_image_path = os.path.join(train_or_valid, 'images', f'{image_id}.jpg')
        shutil.copy(src_image_path, dst_image_path)
        
        print("create label %d : %s" % (index, image_id))


if __name__ == "__main__":
    os.makedirs(os.path.join("train", 'images'), exist_ok=True)
    os.makedirs(os.path.join("train", 'labels'), exist_ok=True)
    os.makedirs(os.path.join("valid", 'images'), exist_ok=True)
    os.makedirs(os.path.join("valid", 'labels'), exist_ok=True)
    generate_labels(dic_labels)
