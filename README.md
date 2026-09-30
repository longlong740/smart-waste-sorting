# 智能垃圾分类｜工创赛

本科工创赛团队项目代码整理稿，包含视觉端、显示端和 STM32 控制程序的比赛版本与开发模块。

## 目录

- firmware：STM32F10x 控制工程与开发模块，保留了不同比赛阶段的代码版本。
- vision：Python 视觉端、显示程序及标注辅助脚本。FINALsheng.py 和 FINALjuesai.py 是两份比赛应用版本。

## 运行与构建

视觉代码使用 Python、OpenCV、NumPy、ONNX Runtime、pySerial 和 Pygame。运行前需要按本机环境配置串口和模型文件位置。训练数据与模型权重没有放入本仓库。

固件使用 Keil/uVision 工程配置。部分 STM32F10x、CMSIS 和板级依赖没有随本仓库发布，需要从相应维护方获取。代码没有在本仓库环境中重新构建，也没有连接实物验证。

## 许可证

团队编写的项目代码按 MIT License 发布。第三方组件继续遵循其自身条款，见 THIRD_PARTY_NOTICES.md。
