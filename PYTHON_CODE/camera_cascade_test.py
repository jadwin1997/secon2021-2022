"""
    Simple script to test Camera class.
"""

from camera import Camera


def main():
    cam = Camera()
    cam.open_camera(640, 10, "CASCADES/net_cascade_v2_400_neg.xml", True)


if __name__ == "__main__":
    main()