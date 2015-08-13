#
# Extract cam3 from full image
# Run script using one of the following supported programs:
#  https://en.wikipedia.org/wiki/AviSynth#AviSynth_compatible_programs
#

src_file = "Source/Pictures/3cam_ToStitch2.bmp"

# Load image, one frame only
src_img = ImageSource(src_file, start=0, end=0)

# Crop the bottom right image
cc3 = Crop(src_img, 640, 480, 0, -64)
ImageWriter(cc3, file="cam3", type="png")
