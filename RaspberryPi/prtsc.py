from picamera import PiCamera
import time

"""set camera"""
camera = PiCamera()
camera.resolution = (756,756)
camera.framerate = 60

# 打开预览
camera.start_preview()
camera.capture('testme.jpeg')
camera.stop_preview()