#
# Extract cam1 from full image
# Run script using one of the following supported programs:
#  https://en.wikipedia.org/wiki/AviSynth#AviSynth_compatible_programs
#

src_file = "Source/Pictures/3cam_ToStitch2.bmp"

# Load image, one frame only
src_img = ImageSource(src_file, start=0, end=0)

# Crop the top left image
cc1 = Crop(src_img, 0, 0, -640, -(480+64))
ImageWriter(cc1, file="cam1", type="png")
