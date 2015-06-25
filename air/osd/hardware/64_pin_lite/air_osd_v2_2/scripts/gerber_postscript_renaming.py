# make pdf versions of files
import os
import os.path

# Put Gerber outputs in this dir to have names changed
src_dir = "../temp/";

#output appears here
postscript_target_dir = "../pcb/postscript_gerbers/"
pdf_target_dir = "../pcb/pdf_gerbers/"

postscript_list = \
   [("osd-Front.ps","osd_v2_2_CopperLayer1-Top.ps"),
    ("osd-Inner3.ps","osd_v2_2_CopperLayer2-Highest_Inner.ps"),
    ("osd-Inner2.ps","osd_v2_2_CopperLayer3-Lowest_Inner.ps"),
    ("osd-Back.ps","osd_v2_2_CopperLayer4-Bottom.ps"),
    ("osd-Mask_Front.ps","osd_v2_2_SolderMask1-Top.ps"),
    ("osd-Mask_Back.ps","osd_v2_2_SolderMask2-Bottom.ps"),
    ("osd-SoldP_Front.ps","osd_v2_2_SolderPaste1-Top.ps"),
    ("osd-SoldP_Back.ps","osd_v2_2_SolderPaste2-Bottom.ps"),
    ("osd-SilkS_Front.ps","osd_v2_2_SilkScreen1-Top.ps"),
    ("osd-SilkS_Back.ps","osd_v2_2_SilkScreen2-Bottom.ps"),
    ("osd-PCB_Edges.ps","osd_v2_2_BoardOutline.ps")]
    
for f in postscript_list:
   if os.path.exists(src_dir + f[0]):
      os.rename(src_dir + f[0] , postscript_target_dir + f[1]) 
   
#Put mirrored versions in mirrored/ subdir prior
mirrored_postscript_list = \
    [("osd-Back.ps","osd_v2_2_CopperLayer4-Bottom-Mirrored.ps"),
    ("osd-Mask_Back.ps","osd_v2_2_SolderMask2-Bottom-Mirrored.ps"),
    ("osd-SoldP_Back.ps","osd_v2_2_SolderPaste2-Bottom-Mirrored.ps"),
    ("osd-SilkS_Back.ps","osd_v2_2_SilkScreen2-Bottom-Mirrored.ps")]
    
# mirrored versions have same name. so put in sub dir
for f in mirrored_postscript_list:
   if os.path.exists(src_dir + "mirrored/" + f[0]):
      os.rename(src_dir + "mirrored/" + f[0] , postscript_target_dir + f[1])

# create the pdfs from the ps files
all_ps_files = postscript_list + mirrored_postscript_list
for f in all_ps_files:
   if os.path.exists(postscript_target_dir + f[1]):
      filename = f[1]
      pdf_name = filename[0:len(filename)-2] + "pdf"
      os.system("ps2pdf " + postscript_target_dir + filename  + " " + pdf_target_dir + pdf_name);


