import cv2
import numpy as np
import onnxruntime as ort
import serial
import time
import struct
import pygame
import sys
import random


def handle_serial_input(recv):
    global garbage_info_list  # 在函数开头声明为全局变量
    if recv == b'\x04':
        #info = "有害垃圾满载"
        #garbage_info_list.append(info)
        man_adisplay_screen()
    if recv == b'\x05':
        #info = "其他垃圾满载"
        #garbage_info_list.append(info)
        man_adisplay_screen()
    if recv == b'\x06':
        #info = "可回收垃圾满载"
        #garbage_info_list.append(info)
        man_adisplay_screen()
    if recv == b'\x07':
        #info = "厨余垃圾满载"
        #garbage_info_list.append(info)
        man_adisplay_screen()
#import tkinter as tk
#import threading
#ser = serial.Serial('/dev/ttyAMA0',115200)
#ser1 = serial.Serial('/dev/ttyAMA1',115200)

# 存储垃圾信息的列表
garbage_info_list = []

# 定义最大显示行数
max_lines = 4  # 可以根据需要调整显示的最大行数 

# Pygame窗口设置
pygame.init()
screen_width = 500
screen_height = 350
screen = pygame.display.set_mode((screen_width, screen_height),pygame.NOFRAME)
pygame.display.set_caption("垃圾桶")
#font = pygame.font.SysFont("SimSun", 15)  # 这里使用了宋体字体，字号由参数传入
font = pygame.font.SysFont("wenquanyizenhei", 15) 

def display_screen(geshu_counts):
    global garbage_info_list, xuhao
    categories = ["厨余垃圾", "可回收垃圾", "有害垃圾", "其他垃圾"]

    # 更新垃圾信息列表
    for category in categories:
        count = geshu_counts.get(category, 0)
        info = f"{xuhao}     {category}   {count}        x"
        garbage_info_list.append(info)
        xuhao += 1  # 递增序号

    # 如果信息超过最大行数，删除最旧的信息
    while len(garbage_info_list) > max_lines:
        garbage_info_list.pop(0)

    # 更新显示
    adisplay_screen()


def man_adisplay_screen():
    screen.fill((0, 0, 0))  # 清屏
    categories = ["厨余垃圾", "可回收垃圾", "有害垃圾", "其他垃圾"]

    # 绘制固定行
    fixed_text = "序号  垃圾名称   已投入  满载状态"
    text_x = 20  # 起始 x 坐标
    text_y = 30  # 起始 y 坐标
    fixed_text_surface = font.render(fixed_text, True, (255, 255, 255))
    screen.blit(fixed_text_surface, (text_x, text_y))

    # 更新 y 坐标以避免固定文本和动态文本重叠
    text_height = font.size(fixed_text)[1]  # 固定文本的高度
    text_y += text_height  # 将 y 坐标下移

    # 列宽分隔点（假设 4 列，对应序号、垃圾名称、已投入、满载状态）
    col_positions = [20, 65, 150, 215, 600]  # 每列的起始 x 坐标

    # 渲染每行动态垃圾信息，并在每行的列之间绘制垂直分隔线
    for index, line in enumerate(garbage_info_list):
        # 检查是否是第三行，并且是否需要标红
        if index == 2 and "有害垃圾" in line:
            line = line.replace("x", "v")  # 替换 'x' 为 'v'
            text_surface = font.render(line, True, (255, 0, 0))  # 红色显示
        else:
            text_surface = font.render(line, True, (255, 255, 255))  # 默认白色显示

        if index == 3 and "其他垃圾" in line:
            line = line.replace("x", "v")  # 替换 'x' 为 'v'
            text_surface = font.render(line, True, (255, 0, 0))  # 红色显示
        else:
            text_surface = font.render(line, True, (255, 255, 255))  # 默认白色显示

        if index == 1 and "可回收垃圾" in line:
            line = line.replace("x", "v")  # 替换 'x' 为 'v'
            text_surface = font.render(line, True, (255, 0, 0))  # 红色显示
        else:
            text_surface = font.render(line, True, (255, 255, 255))  # 默认白色显示

        if index == 0 and "厨余垃圾" in line:
            line = line.replace("x", "v")  # 替换 'x' 为 'v'
            text_surface = font.render(line, True, (255, 0, 0))  # 红色显示
        else:
            text_surface = font.render(line, True, (255, 255, 255))  # 默认白色显示

        screen.blit(text_surface, (text_x, text_y))




        # 在每列信息之间绘制竖线
        for col_x in col_positions:
            pygame.draw.line(screen, (255, 255, 255), (col_x, text_y), (col_x, text_y + text_height), 1)

        # 在每条信息下方绘制一条白色的水平线
        pygame.draw.line(screen, (255, 255, 255), (col_positions[0], text_y + text_height + 2), (col_positions[-1], text_y + text_height + 2), 1)

        # 将下一行文本的起始位置下移一个字体高度
        text_y += text_height + 10  # 加10个像素作为信息和线之间的间隔

    # 刷新屏幕
    pygame.display.flip()


def adisplay_screen():
    screen.fill((0, 0, 0))  # 清屏
    categories = ["厨余垃圾", "可回收垃圾", "有害垃圾", "其他垃圾"]

    # 绘制固定行
    fixed_text = "序号  垃圾名称   已投入  满载状态"
    text_x = 20  # 起始 x 坐标
    text_y = 30  # 起始 y 坐标
    fixed_text_surface = font.render(fixed_text, True, (255, 255, 255))
    screen.blit(fixed_text_surface, (text_x, text_y))

    # 更新 y 坐标以避免固定文本和动态文本重叠
    text_height = font.size(fixed_text)[1]  # 固定文本的高度
    text_y += text_height  # 将 y 坐标下移

    # 列宽分隔点（假设 4 列，对应序号、垃圾名称、已投入、满载状态）
    col_positions = [20, 65, 150, 215, 600]  # 每列的起始 x 坐标

    # 渲染每行动态垃圾信息，并在每行的列之间绘制垂直分隔线
    for index, line in enumerate(garbage_info_list):
        text_surface = font.render(line, True, (255, 255, 255))  # 渲染文本信息
        #text_surface = font.render(line, True, (255, 0, 0))
        screen.blit(text_surface, (text_x, text_y))

        # 在每列信息之间绘制竖线
        for col_x in col_positions:
            pygame.draw.line(screen, (255, 255, 255), (col_x, text_y), (col_x, text_y + text_height), 1)

        # 在每条信息下方绘制一条白色的水平线
        pygame.draw.line(screen, (255, 255, 255), (col_positions[0], text_y + text_height + 2), (col_positions[-1], text_y + text_height + 2), 1)

        # 将下一行文本的起始位置下移一个字体高度
        text_y += text_height + 10  # 加10个像素作为信息和线之间的间隔

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




if __name__ == "__main__":

    model_pb_path =  "D:\\11.1.onnx"
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



    geshu_counts = {
                        "厨余垃圾": 0,
                        "可回收垃圾": 0,
                        "有害垃圾": 0,
                       "其他垃圾": 0
                                   }


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
                #初始化计数
                label_counts = {label: 0 for label in dic_labels.values()}

                xuhao=1
#                geshu_counts = {
#                        "可回收垃圾": 0,
#                        "厨余垃圾": 0,
#                        "有害垃圾": 0,
#                        "其他垃圾": 0
#                                    }
                t1 = time.time()
                det_boxes,scores,ids = infer_img(img0,net,model_h,model_w,nl,na,stride,anchor_grid,thred_nms=0.4,thred_cond=0.5)
                #print(det_boxes)
                t2 = time.time()
     
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
                    if w>=h:
                        d = 1
                    elif w<h:
                        d = 2

                    if min_val == 0 or min_val==7 or min_val==11:
                        if scores[min_index] >= 0.25:
                            outuart(2,c_x*0.155,c_y*0.13,d,1)  #ke hui shou
                            print(22)
                            flag_det = False
                            #break
                    if min_val == 4 or min_val == 9 or min_val == 10:
                        if scores[min_index] >= 0.25:
                            outuart(1,c_x*0.165,c_y*0.15,d,1)   #you hai
                            print(11)
                            flag_det = False                  
                            #break
                    if min_val == 2 or min_val == 6 or min_val == 8:
                        if scores[min_index] >= 0.25:
                            outuart(4,c_x*0.155,c_y*0.15,d,1)   #chu yv
                            print(44)
                            flag_det = False               
                            #break
                    if min_val == 1 or min_val == 3 or min_val==5:
                        if scores[min_index] >= 0.25:
                            outuart(3,c_x*0.155,c_y*0.13,d,1)   #qi ta
                            print(33)
                            flag_det = False

                    for id in ids:
                        label_name = dic_labels[id]
                        label_counts[label_name] += 1  
                    for label, count in label_counts.items():
                        if count > 0:  # 只打印检测到的标签
                            print(f"{label}: {count}")
                            if label in ['bottle', 'smallbottle', 'zhibei']:
                                geshu_counts["可回收垃圾"] += count
                            elif label in ['carrot', 'potato', 'turnip']:
                                geshu_counts["厨余垃圾"] += count
                            elif label in ['dianchi', 'yaoban', 'yao']:
                                geshu_counts["有害垃圾"] += count
                            elif label in ['brick', 'cobblestone', 'porcelain']:
                                geshu_counts["其他垃圾"] += count




                    display_screen(geshu_counts)  # 更新显示    
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