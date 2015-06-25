
import os
import os.path

src_dir = "../schematic/postscript/"
target_dir = "../schematic/pdf/"

ps_files = \
   ["osd-camera.ps",
   "osd-mavlink_port.ps",
   "osd.ps",
   "osd-video_circuit.ps",
   "osd-expansion.ps",
   "osd-MCU.ps",
   "osd-psu.ps",
   "osd-video_out.ps"]

for f in ps_files:
  # if os.path.exists(src_dir + f):
   pdf_name = f[0:len(f)-2] + "pdf"
   os.system("ps2pdf " + src_dir + f + " " + target_dir + pdf_name)
  # command = "ps2pdf " + src_dir + f + " " + target_dir + pdf_name
  ##print(command)
   