"""
    Records every frame of camera to disk
"""

from classes.PictureTaker import PictureTaker


def main():
    cam = PictureTaker()
    cam.open_camera(640, 10, 15, False, True, 1)


if __name__ == "__main__":
    main()
