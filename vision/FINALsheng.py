
# -*- coding: utf-8 -*-
import cv2
import numpy as np
import onnxruntime as ort
import serial
import time
import struct
import pygame
import sys
import random
import os

def handle_serial_input(recv):
    global garbage_info_list  # 在函数开头声明为全局变量
    if recv == b'\x04':
        info = "有害垃圾满载"
        garbage_info_list.append(info)
        adisplay_screen()
    if recv == b'\x05':
        info = "其他垃圾满载"
        garbage_info_list.append(info)
        adisplay_screen()
    if recv == b'\x06':
        info = "可回收垃圾满载"
        garbage_info_list.append(info)
        adisplay_screen()
    if recv == b'\x07':
        info = "厨余垃圾满载"
        garbage_info_list.append(info)
        adisplay_screen()
#import tkinter as tk
#import threading
#ser = serial.Serial('/dev/ttyAMA0',115200)
#ser1 = serial.Serial('/dev/ttyAMA1',115200)

# 存储垃圾信息的列表
garbage_info_list = []

# 定义最大显示行数
max_lines = 7  # 可以根据需要调整显示的最大行数
# 设置窗口位置（左上角坐标）
window_position_x, window_position_y = 0, 0  # 根据你的需求调整位置
os.environ['SDL_VIDEO_WINDOW_POS'] = f"{window_position_x},{window_position_y}"
# Pygame窗口设置
pygame.init()
screen_width = 1000
screen_height = 800
screen = pygame.display.set_mode((screen_width, screen_height),pygame.NOFRAME)
pygame.display.set_caption("垃圾桶")
font = pygame.font.SysFont("wenquanyizenhei", 25)  # 这里使用了宋体字体，字号由参数传入

def display_screen(xuhao, zhonglei, geshu):
    global garbage_info_list
    info = f"序号：{xuhao} 类别：{zhonglei} 个数：{geshu} 状态：OK!"
    
    # 将新信息添加到列表中
    garbage_info_list.append(info)
    
    # 如果超过最大行数，删除最旧的信息
    if len(garbage_info_list) > max_lines:
        garbage_info_list.pop(0)

    # 更新显示
    adisplay_screen()

def adisplay_screen():
    screen.fill((0, 0, 0))  # 清屏

    # 绘制计数文本
    text_height = font.size("Test")[1]  # 使用 "Test" 文本的高度作为基准
    text_x = 20  # 起始 x 坐标
    text_y = 30

    # 渲染每行文本并绘制到屏幕上
    for line in garbage_info_list:
        text_surface = font.render(line, True, (255, 255, 255))
        screen.blit(text_surface, (text_x, text_y))
        text_y += text_height  # 将下一行文本的起始位置下移一个字体高度

    # 刷新屏幕
    pygame.display.flip()
def outuart(cx,ca,cy,cz,cw):
	data = struct.pack("BBBBBBBBB",
				   0x2C,
				   0x12,
				   int(cx),
				   int(ca),
				   int(cy),
				   int(cz),
                   int(cw),
                   0x5B,
				   0x23)
	#ser.write(data);
    

def plot_one_box(x, img, color=None, label=None, line_thickness=None):
    """
    description: Plots one bounding box on image img,
                 this function comes from YoLov5 project.
    param: 
        x:      a box likes [x1,y1,x2,y2]
        img:    a opencv image object
        color:  color to draw rectangle, such as (0,255,0)
        label:  str
        line_thickness: int
    return:
        no return
    """
    tl = (
        line_thickness or round(0.002 * (img.shape[0] + img.shape[1]) / 2) + 1
    )  # line/font thickness
    color = color or [random.randint(0, 255) for _ in range(3)]
    c1, c2 = (int(box[0]), int(box[1])), (int(box[2]), int(box[3]))
    
    cv2.rectangle(img, c1, c2, color, thickness=tl, lineType=cv2.LINE_AA)
    if label:
        tf = max(tl - 1, 1)  # font thickness
        t_size = cv2.getTextSize(label, 0, fontScale=tl / 3, thickness=tf)[0]
        c2 = c1[0] + t_size[0], c1[1] - t_size[1] - 3
        cv2.rectangle(img, c1, c2, color, -1, cv2.LINE_AA)  # filled
        cv2.putText(
            img,
            label,
            (c1[0], c1[1] - 2),
            0,
            tl / 3,
            [225, 255, 255],
            thickness=tf,
            lineType=cv2.LINE_AA,
        )
#生成二维网络
def _make_grid( nx, ny):
        xv, yv = np.meshgrid(np.arange(ny), np.arange(nx))
        return np.stack((xv, yv), 2).reshape((-1, 2)).astype(np.float32)
#对模型检测解码
def cal_outputs(outs,nl,na,model_w,model_h,anchor_grid,stride):
    row_ind = 0
    grid = [np.zeros(1)] * nl
    for i in range(nl):
        h, w = int(model_w/ stride[i]), int(model_h / stride[i])
        length = int(na * h * w)
        if grid[i].shape[2:4] != (h, w):
            grid[i] = _make_grid(w, h)

        outs[row_ind:row_ind + length, 0:2] = (outs[row_ind:row_ind + length, 0:2] * 2. - 0.5 + np.tile(
            grid[i], (na, 1))) * int(stride[i])
        outs[row_ind:row_ind + length, 2:4] = (outs[row_ind:row_ind + length, 2:4] * 2) ** 2 * np.repeat(
            anchor_grid[i], h * w, axis=0)
        row_ind += length
    return outs

#模型检测并输出图像高度及宽度
def post_process_opencv(outputs,model_h,model_w,img_h,img_w,thred_nms,thred_cond):
    conf = outputs[:,4].tolist()
    c_x = outputs[:,0]/model_w*img_w
    c_y = outputs[:,1]/model_h*img_h
    w  = outputs[:,2]/model_w*img_w
    h  = outputs[:,3]/model_h*img_h
    p_cls = outputs[:,5:]
    if len(p_cls.shape)==1:
        p_cls = np.expand_dims(p_cls,1)
    cls_id = np.argmax(p_cls,axis=1)

    p_x1 = np.expand_dims(c_x-w/2,-1)
    p_y1 = np.expand_dims(c_y-h/2,-1)
    p_x2 = np.expand_dims(c_x+w/2,-1)
    p_y2 = np.expand_dims(c_y+h/2,-1)
    areas = np.concatenate((p_x1,p_y1,p_x2,p_y2),axis=-1)

    areas = areas.tolist()
    ids = cv2.dnn.NMSBoxes(areas,conf,thred_cond,thred_nms)
    if len(ids)>0:
        return  np.array(areas)[ids],np.array(conf)[ids],cls_id[ids]
    else:
        return [],[],[]


#模型推断，返回边框置信度及类别标识
def infer_img(img0,net,model_h,model_w,nl,na,stride,anchor_grid,thred_nms=0.4,thred_cond=0.5):
    img = cv2.resize(img0, [model_w,model_h], interpolation=cv2.INTER_AREA)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img = img.astype(np.float32) / 255.0
    blob = np.expand_dims(np.transpose(img, (2, 0, 1)), axis=0)

    outs = net.run(None, {net.get_inputs()[0].name: blob})[0].squeeze(axis=0)
    outs = cal_outputs(outs,nl,na,model_w,model_h,anchor_grid,stride)

    img_h,img_w,_ = np.shape(img0)
    boxes,confs,ids = post_process_opencv(outs,model_h,model_w,img_h,img_w,thred_nms,thred_cond)

    return  boxes,confs,ids


def is_within_circle(new_center, old_center, radius):
    return np.linalg.norm(np.array(new_center) - np.array(old_center)) < radius

if __name__ == "__main__":

    model_pb_path =  "D:\\1030.onnx"
    so = ort.SessionOptions()
    net = ort.InferenceSession(model_pb_path, so, providers=['CPUExecutionProvider'])
    

    dic_labels= {0:'bottle',
            1:'brick',
            2:'carrot',
            3:'cobblestone',
            4:'dianchi',
            5:'porcelain',
            6:'potato',
            7:'smallbottle',
            8:'turnip',
            9:'yao',
            10:'yaoban',
            11:'zhibei',}
    label_counts = {label: 0 for label in dic_labels.values()}#标签个数
    model_h = 320
    model_w = 320
    nl = 3
    na = 3
    stride=[8.,16.,32.]
    anchors = [[10, 13, 16, 30, 33, 23], [30, 61, 62, 45, 59, 119], [116, 90, 156, 198, 373, 326]]
    anchor_grid = np.asarray(anchors, dtype=np.float32).reshape(nl, -1, 2)

    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 520)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 450)

    last_center = None
    centers = []
    radius = 10#像素阈值


    xuhao=0
    geshu=0
    #zhonglei=0
    # 初始化 Pygame
    pygame.init()
    # 设置屏幕大小
    #runing=True
    #video_thread = threading.Thread(target=video_player)
    #video_thread.start()
    flag_det = True
    while True:
        success, img0 = cap.read()
        if success:
            if flag_det:

                det_boxes, scores, ids = infer_img(img0, net, model_h, model_w, nl, na, stride, anchor_grid, thred_nms=0.4, thred_cond=0.5)
                label_counts = {label: 0 for label in dic_labels.values()}
                for id in ids:
                    label_name = dic_labels[id]
                    label_counts[label_name] += 1  
                for label, count in label_counts.items():
                    if count > 0:  # 只打印检测到的标签
                        print(f"{label}: {count}")              
                #绘制边框
                t1 = time.time()
                det_boxes,scores,ids = infer_img(img0,net,model_h,model_w,nl,na,stride,anchor_grid,thred_nms=0.4,thred_cond=0.5)
                #print(det_boxes)
                t2 = time.time()
                label_counts = {label: 0 for label in dic_labels.values()}
                for id in ids:
                    label_name = dic_labels[id]
                    label_counts[label_name] += 1  
                for label, count in label_counts.items():
                    if count > 0:  # 只打印检测到的标签
                        print(f"{label}: {count}")              
                #绘制边框
                for box,score,id in zip(det_boxes,scores,ids):
                    label = '%s:%.2f'%(dic_labels[id],score)
                    print(dic_labels[id])
                    plot_one_box(det_boxes.astype(np.int16), img0, color=(255,0,0), label=label, line_thickness=None)
                    print(dic_labels[id])
                l=len(ids)
                if l > 0:
                    min_val = ids[0] 
                    min_index = 0 
                    for i in range(0,len(ids)):
                        if ids[i] < min_val:
                            min_val = ids[i]
                            #print(min_val)
                            min_index = i
                            #print(min_index)
                    c_x = (det_boxes[min_index,2]-det_boxes[min_index,0])/2+det_boxes[min_index,0]#右下和左上
                    c_y = (det_boxes[min_index,3]-det_boxes[min_index,1])/2+det_boxes[min_index,1]
                    w = det_boxes[min_index,2]-det_boxes[min_index,0]
                    h = det_boxes[min_index,3]-det_boxes[min_index,1]
                    current_center = (c_x, c_y)
                    if w>=h:
                        d = 1
                    elif w<h:
                        d = 2
                    centers.append(current_center)
                    # 如果已记录5个中心坐标，进行检查
                    if len(centers) >= 20:
                        if all(is_within_circle(centers[i], centers[i - 1], radius) for i in range(1, 20)):
                            if min_val == 1:
                                if scores[min_index] >= 0.25:
                                    outuart(5,c_x*0.155,c_y*0.13,d,1)  #ping zi
                                    print(55)
                                    flag_det = False

                            #break
                            if min_val == 0 or min_val ==7 or min_val ==11:
                                if scores[min_index] >= 0.25:
                                    outuart(2,c_x*0.155,c_y*0.13,d,1)  #ke hui shou
                                    print(22)
                                    flag_det = False
                                    zhonglei="可回收垃圾"
                                    geshu=len(det_boxes)
                                    xuhao+=1
                                    display_screen(xuhao,zhonglei, geshu) 
                            #break
                            if min_val == 4 or min_val == 9 or min_val ==10:
                                if scores[min_index] >= 0.75:
                                    outuart(1,c_x*0.165,c_y*0.15,d,1)   #you hai
                                    print(11)
                                    flag_det = False
                                    zhonglei="有害垃圾"
                                    geshu=len(det_boxes)
                                    xuhao+=1
                                    display_screen(xuhao,zhonglei, geshu) 
                            #break
                            if min_val == 2 or min_val == 6 or min_val == 8:
                                if scores[min_index] >= 0.25:
                                    outuart(4,c_x*0.155,c_y*0.15,d,1)   #chu yv
                                    print(44)
                                    flag_det = False
                                    zhonglei="厨余垃圾"
                                    geshu=len(det_boxes)
                                    xuhao+=1
                                    display_screen(xuhao,zhonglei, geshu) 
                            #break
                            if min_val == 1 or min_val == 3 or min_val == 5:
                                if scores[min_index] >= 0.25:
                                    outuart(3,c_x*0.155,c_y*0.13,d,1)   #qi ta
                                    print(33)
                                    flag_det = False
                                    zhonglei="其他垃圾"
                                    geshu=len(det_boxes)
                                    xuhao+=1
                                    display_screen(xuhao,zhonglei, geshu) 
                        # 移除最早的中心坐标
                        centers.pop(0)
        cv2.imshow("video",img0)
        #cv2.moveWindow('video', 1275, 0)
        key=cv2.waitKey(1) & 0xFF  
        if key == ord('q'):
            break
        elif key & 0xFF == ord('s'):
            flag_det = True
            print(flag_det)
        #count1 = ser.inWaiting()
        #if count1 != 0:
            #recv = ser.read(count1)
            #print (recv)
            #if recv == b'\x01':
                #flag_det = True
            #handle_serial_input(recv)   #满载  
            #flag_det = True         
        #ser.flushInput()

    cap.release()