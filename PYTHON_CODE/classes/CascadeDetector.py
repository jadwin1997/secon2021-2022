"""
    Defines the CascadeDetector class, which opens the onboard camera with a specified width and fps and
    finds targets from a given classifier

    ---

    Attributes:
    
    center_x: int - 0 to 100 that represents where in the frame the center of the detected
    object is. Value is -1 if there are no detected objects.

    NOTE: Right now, the center_x value is only for the *last* detected object. If there are
    multiple objects in frame, center_x is only for one of them.
"""

import cv2

class CascadeDetector:
    def __init__(self):
        # default value of center_x
        self.center_x = -1

    def write_to_arduino(self):
        # send self.center_x to arduino however
        pass

    """
        function open_camera: Opens an openCV VideoCapture

        width: int - width of video frame

        NOTE: The width parameter only works if the webcam supports the specified resolution. This
        is a hardware limitation; OpenCV cannot resize a webcam output to any size.

        fps: int - FPS of video stream.

        classifier: str - Path to cascade classifier.

        display: bool - Determines whether the class opens a video window displaying the camera
        and detected objects. Also determines if the class prints center_x to console. Press
        [ESC] to exit the window.
    """
    def open_camera(self, width: int, fps: int, classifier: str, display: bool):
        cascade = cv2.CascadeClassifier(classifier)
        cap = cv2.VideoCapture(0)

        cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        cap.set(cv2.CAP_PROP_FPS, fps)

        # sets width to the actual width of the camera, just in case the change did not work
        width = cap.get(cv2.CAP_PROP_FRAME_WIDTH)

        while cap.isOpened():
            # read frame
            ret, img = cap.read()

            # if frame is bad, skip it
            if not ret:
                continue

            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

            # get detected objects
            try:
                targets = cascade.detectMultiScale(gray, 1.1, 4)
            except:
                print("ERROR: That classifier could not be opened")
                break

            # draw rectangles for every object if displaying and set center_x to the center of the object
            for(x, y, w, h) in targets:
                if display == True:
                    cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
                self.center_x = int(((x + (w / 2)) / width) * 100)
            
            # if there are no targets, center_x = -1
            if targets == ():
                self.center_x = -1

            # display window
            if display == True:
                cv2.imshow("test", img)
                print(self.center_x)

            # exit when the user presses esc
            k = cv2.waitKey(30) & 0xff
            if k == 27:
                break
        
        cap.release()
        cv2.destroyAllWindows()
