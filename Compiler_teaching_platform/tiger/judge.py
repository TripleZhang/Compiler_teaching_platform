import os

def judge(dir):
    pipline = os.popen("sh make.sh")
    return pipline.read()