"""
    Simple script to test CascadeDetector class.
"""

from classes.CascadeDetector import CascadeDetector


def main():
    cam = CascadeDetector()
    cam.open_camera(640, 10, "CASCADES/net_cascade_v2_400_neg.xml", True)


if __name__ == "__main__":
    main()