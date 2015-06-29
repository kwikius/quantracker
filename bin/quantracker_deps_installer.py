import urllib
import zipfile
import tarfile
import os
import platform
import sys

url_list = [\
("mavlink",
"https://github.com/mavlink/c_library/archive/master.zip",
 "mavlink.zip",
 "mavlink"),
("arm-gcc-linux",
"https://launchpad.net/gcc-arm-embedded/4.9/4.9-2014-q4-major/+download/gcc-arm-none-eabi-4_9-2014q4-20141203-linux.tar.bz2",
"arm-gcc-linux.tar.bz2",
"gcc-arm-none-eabi-4_9-2014q4"),
("arm-gcc-win",
"https://launchpad.net/gcc-arm-embedded/4.9/4.9-2014-q4-major/+download/gcc-arm-none-eabi-4_9-2014q4-20141203-win32.zip",
"arm-gcc-win.zip",
"gcc-arm-none-eabi-4_9-2014q4"),
("quan",
"https://github.com/kwikius/quan-trunk/archive/master.zip",
"quan.zip",
"quan-trunk-master"),
("freertos",
"http://downloads.sourceforge.net/project/freertos/FreeRTOS/V8.2.0/FreeRTOSV8.2.0.zip",
"freertos.zip",
"FreeRTOSV8.2.0"),
("quantracker",
"https://github.com/kwikius/quantracker/archive/master.zip",
"quantracker.zip",
"quantracker-master"),
("stm32_lib",
"http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32f4_dsp_stdperiph_lib.zip",
"stm32f4_dsp_stdperiph_lib.zip",
"STM32F4xx_DSP_StdPeriph_Lib_V1.5.1"),
("stm32flash",
"http://sourceforge.net/projects/stm32flash/files/stm32flash-0.4.tar.gz",
"stm32flash-0.4.tar.gz",
"stm32flash")]

target_deps_dir = None
if (len(sys.argv) > 1):
   target_deps_dir = sys.argv[1]
else:
   while 1:
     target_deps_dir = raw_input("Please enter the Directory to install dependencies, enter for none ")
     target_good = (target_deps_dir == "") or ( (os.path.exists(target_deps_dir)) and (os.path.isdir(target_deps_dir)))
     if target_good:
        break
     else:
        print("The chosen directory must be an existing directory")

print("This may take a while so please be patient! ")

for info in url_list:
   invalid1 = ((platform.system() == 'Linux') and (info[0] == "arm-gcc-win"))
   invalid2 = ((platform.system() == 'Windows') and (info[0] == "arm-gcc-linux"))
   invalid = invalid1 or invalid2
   # todo mac
   if (not invalid):
      url = info[1]
      to_file = info[2]
      if  not os.path.exists(info[2]):
         print ("retrieving \""+ url + "\"")
         urllib.urlretrieve(url,filename = to_file)
         print ("extracting  \"" + to_file + "\" ...");
         if info[0] == "arm-gcc-linux":
            t = tarfile.open(to_file, 'r:bz2')
            t.extractall()
         elif info[0] == "stm32flash":
            print("extracting stm32flash...")
            t = tarfile.open(to_file, 'r')
            t.extractall()
         else:
            z = zipfile.ZipFile(to_file,'r')
            z.extractall()

         if info[0] == "mavlink":
            print("renaming mavlink dir from c_library-master to mavlink")
            os.rename("c_library-master","mavlink")

         if info[0] == "stm32flash":
           print ("building stm32flash...")
           os.system ("make -C stm32flash")

      else:
           print(info[2] + " appears to exist already. To replace it, delete it and restart the script")
 
print("download and extraction of Quantracker dependencies complete")

if (target_deps_dir != ""):
   print("moving dependencies to "+ target_deps_dir)
   for info in url_list:
      invalid1 = ((platform.system() == 'Linux') and (info[0] == "arm-gcc-win"))
      invalid2 = ((platform.system() == 'Windows') and (info[0] == "arm-gcc-linux"))
      invalid3 = info[0] == "stm32flash"
      invalid = invalid1 or invalid2 or invalid3
# add checks for target dir not empty
      if (not invalid):
         if target_deps_dir[len(target_deps_dir) -1] != '/':
            target_deps_dir += '/'

         target_path = target_deps_dir + info[3]
         if os.path.exists(target_path):
           print("\"" + target_path + "\" seems to be installed already. To replace it delete it and restart the script")
         else:
           os.rename(info[3], target_path )
if not os.path.exists(target_deps_dir + "bin/"):
   os.mkdir(target_deps_dir + "bin/")
os.rename("stm32flash/stm32flash",target_deps_dir + "bin/stm32flash")
print("Quantracker Dependencies installed")

print("writing Dependencies.mk")
# open a file
f = open('Dependencies.mk','w')

f.write('TOOLCHAIN_GCC_VERSION := 4.9.3\n')
f.write('STM32FLASH := ' + target_deps_dir + 'bin/stm32flash')
f.write('OPTIMISATION_LEVEL := O3\n')
f.write('QUANTRACKER_DEPENDENCY_LIBS_DIR := ' + target_deps_dir + '\n')
f.write('TOOLCHAIN_PREFIX := $(QUANTRACKER_DEPENDENCY_LIBS_DIR)gcc-arm-none-eabi-4_9-2014q4/\n')
f.write('QUAN_INCLUDE_PATH := $(QUANTRACKER_DEPENDENCY_LIBS_DIR)quan-trunk-master/\n')
f.write('MAVLINK_INCLUDE_PATH := $(QUANTRACKER_DEPENDENCY_LIBS_DIR)\n')
f.write('FREE_RTOS_DIR := $(QUANTRACKER_DEPENDENCY_LIBS_DIR)/FreeRTOSV8.2.0/FreeRTOS/\n')
f.write('STM32_STD_PERIPH_LIB_DIR := $(QUANTRACKER_DEPENDENCY_LIBS_DIR)STM32F4xx_DSP_StdPeriph_Lib_V1.5.1/Libraries/\n')
f.close()
os.rename('Dependencies.mk',  target_deps_dir + '/quantracker-master/Dependencies.mk')






   

