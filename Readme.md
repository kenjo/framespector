# FreameSpector

This is a small project to create video file or display an animation to make it easy to detect lost frames by just looking at them. no need to measure anything.

This is done by alternating between two solid color frames one red and one green. This will produce a somewhat stroboscope like image that is yellow. The beneficial side effect of this is that it will be very easy to detect any interruption in this animation. A single missed frame on a 60 fps video is visible by more or less anyone.

what will be seen is like a short flash of red or green.

## Premade videos.

### youtube
A youtube video exist [here](https://www.youtube.com/watch?v=ygHkHRi6-xI)

If you see any issues try turn on "Stats for nerds" in the player and see if the lost frames you see is actually reported by the stats as dropped or if the browser/player thinks everything is OK.

### Mpeg 4 file

mp4 [video](https://github.com/kenjo/framespector/assets/484396/f0ccd087-3dc8-4ccf-80e6-3ea1e3b07f41
)

this was converted to h265 with
```
ffmpeg -i video_test_60fps.mp4 -c:v libx265 -vtag hvc1  video_test_60fps_.mp4
```

## Create mpeg video

This is done on linux/ubuntu by running the python script called gen_test_vid.py.

Prerequisites

```
pip install pygame
pip install opencv-python
```
Then run it like

```
python3 gen_test_vid.py --size 1k --frames 3600 --text-size 200 --rate 60 --mpeg --flipflop
```

## Run standalone program

This is a Linux/ubuntu C program using SDL2.

Prerequisites
```
apt install libsdl2-dev
apt install libsdl2-ttf-dev
```
then
```
make
./frame_test
```
### Future work
The program needs lot of work so its possible to select rendering system other than SDL2. OpenGL, Vulcan, Xorg, wayland comes to mind just to be able to see what works and what does not.
