#kicad gerber file renaming
# Gerber options
# dont forget  local origin for gerbers and drills
import os
import os.path

# Put Gerber outputs in this dir to have names changed
src_dir = "../temp/";

#output appears here
target_dir = "../pcb/gerbers/"

# list of renames
gerbers_list = \
   [("osd-Front.gtl","osd_v2_2_CopperLayer1-Top.GTL"),
   ("osd-Inner3.gbr","osd_v2_2_CopperLayer2-Highest_Inner.G2L"),
   ("osd-Inner2.gbr","osd_v2_2_CopperLayer3-Lowest_Inner.G3L"),
   ("osd-Back.gbl","osd_v2_2_CopperLayer4-Bottom.GBL"),
   ("osd-Mask_Front.gts","osd_v2_2_SolderMask1-Top.GTS"),
   ("osd-Mask_Back.gbs","osd_v2_2_SolderMask2-Bottom.GBS"),
   ("osd-SoldP_Front.gtp","osd_v2_2_SolderPaste1-Top.GTP"),
   ("osd-SoldP_Back.gbp","osd_v2_2_SolderPaste2-Bottom.GBP"),
   ("osd-SilkS_Front.gto","osd_v2_2_SilkScreen1-Top.GTO"),
   ("osd-SilkS_Back.gbo","osd_v2_2_SilkScreen2-Bottom.GBO"),
   ("osd-PCB_Edges.gbr","osd_v2_2_BoardOutline.GKO"),
   ("osd.drl","osd_v2_2_DrillFile.XLN")]
   
for f in gerbers_list:
   if os.path.exists(src_dir + f[0]):
      os.rename(src_dir + f[0] , target_dir + f[1])



