"""
    Defines the PictureTaker class, which opens the onboard camera and take pictures sequentially
"""

import cv2
from pathlib import Path
from datetime import datetime, timezone
import os


class PictureTaker:
    def __init__(self):
        # default value of center_x
        self.center_x = -1

    """
        function open_camera: Opens an openCV VideoCapture

        width: int - width of video frame

        fps: int - FPS of video stream.

        NOTE: The width and fps parameters only works if the webcam supports them.
        This is a hardware limitation. OpenCV cannot freely change these values.
        
        skip: int - amount of frames to skip
        
        to_gray: bool - whether or not to make image grayscale

        display: bool - Determines whether the class opens a video window displaying the camera
        and detected objects. Also determines if the class prints center_x to console. Press
        [ESC] to exit the window.

        scale: int - Scale to resize output frames to
    """
    def open_camera(self, width: int, fps: int, skip: int, to_gray: bool, display: bool, scale: float = 1):
        cap = cv2.VideoCapture(0)

        # try to change width and fps
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        cap.set(cv2.CAP_PROP_FPS, fps)

        # sets width to the actual width of the camera, just in case the change did not work
        width = cap.get(cv2.CAP_PROP_FRAME_WIDTH)

        # keep track of frame number
        count = 0

        output_dir = str(datetime.now(timezone.utc)).replace(":", "-")
        os.makedirs(output_dir, exist_ok = True)

        while cap.isOpened():
            count += 1

            if count % skip != 0:
                continue

            # read frame
            ret, img = cap.read()

            # if frame is bad, skip it
            if not ret:
                continue

            if scale != 1:
                resized_img = cv2.resize(img, None, fx=scale, fy=scale, interpolation = cv2.INTER_LINEAR)

            if to_gray:
                final = cv2.cvtColor(resized_img, cv2.COLOR_BGR2GRAY)
            else:
                final = img

            cv2.imwrite(output_dir + "/{file}.jpg".format(file = str(count)), final)

            # display window
            if display == True:
                cv2.imshow("test", img)

            # exit when the user presses esc
            k = cv2.waitKey(30) & 0xff
            if k == 27:
                break
        
        cap.release()
        cv2.destroyAllWindows()
