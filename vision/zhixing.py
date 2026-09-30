import subprocess

def run_scripts():
    # 启动两个脚本
    process1 = subprocess.Popen(['python', 'FINALsheng.py'])
    process2 = subprocess.Popen(['python', 'video0.py'])

    # 等待两个脚本执行完成
    process1.wait()
    process2.wait()

if __name__ == "__main__":
    run_scripts()
