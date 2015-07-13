import urllib
import zipfile
import tarfile
import os
import platform
import sys

install_dir = None

def install_dir_good():
  if (install_dir == None) or (install_dir == ""):
     print("no install directory specified")
     return False
  elif not os.path.exists(install_dir):
     print("install directory \"" + install_dir + "\" not found ( install directory must exist already)")
     return False
  elif not os.path.isdir(install_dir):
     print("\"" + install_dir + "\" is not a directory")
     return False
  else :
     return True

def normalise_install_dir():
   install_dir_temp = os.path.abspath(install_dir)
   if install_dir_temp[len(install_dir_temp) -1] != '/':
      install_dir_temp += '/'
   print ("install directory = " + install_dir_temp)
   return install_dir_temp

rem_restart = "\". Remove and restart to download a fresh copy"

def install_mavlink ():
   dep_dir = "mavlink"
   if not os.path.exists(install_dir + dep_dir):
      exn = "c_library-master"
      if not os.path.exists(exn):
         zipf = "mavlink.zip"
         if not os.path.exists(zipf):
            print("retrieving mavlink ...")
            url = "https://github.com/mavlink/c_library/archive/master.zip"
            try:
               urllib.urlretrieve(url,zipf )
            except:
               print("Couldnt retrieve \"" + url + "\". Are you connected to the internet? ")  
               return False
         print("extracting mavlink ...")
         try:
            z = zipfile.ZipFile(zipf,'r')
            z.extractall()
         except:
            print("Couldnt extract \"" + zipf + " Possibly download corrupted or interrupted.")
            print("Delete it and restart installer to retry")
            return False
      print("installing mavlink ...")
      try:
         os.rename(exn, install_dir + dep_dir)
      except:
         print("Couldnt rename \"" + exn + "\" to \"" + install_dir + dep_dir + "\". Check target directory status")
         return False
      print ("---[mavlink installed]---")
      return True
   else:
      print ("found pre-existing mavlink install")
      return True

def install_arm_gcc():
   dep_dir = "gcc-arm-none-eabi-4_9-2014q4" 
   if not os.path.exists(install_dir + dep_dir):
      exn = dep_dir
      if not os.path.exists(exn):
         if platform.system() == 'Linux':
            tarf = "arm-gcc-linux.tar.bz2"
            if not os.path.exists(tarf):
               print("retrieving arm-gcc linux ...")
               url = "https://launchpad.net/gcc-arm-embedded/4.9/4.9-2014-q4-major/+download/gcc-arm-none-eabi-4_9-2014q4-20141203-linux.tar.bz2"
               try:
                  urllib.urlretrieve(url,tarf)
               except:
                  print("Couldnt retrieve \"" + url + "\". Are you connected to the internet? ")  
                  return False
            print ("extracting arm-gcc ...")
            try:
               t = tarfile.open(tarf, 'r:bz2')
               t.extractall()  
            except:
               print("couldnt extract \"" + tarf + " Possibly download corrupted or interrupted.")
               print("Delete it and restart installer to retry")
               return False
         elif platform.system() == 'Windows':
             zipf = "arm-gcc-win.zip"
             if not os.path.exists(zipf):
                print("retrieving arm-gcc Windows ...")
                url = "https://launchpad.net/gcc-arm-embedded/4.9/4.9-2014-q4-major/+download/gcc-arm-none-eabi-4_9-2014q4-20141203-win32.zip"
                try:
                     urllib.urlretrieve(url,zipf)
                except:
                   print("Couldnt retrieve \"" + url + "\". Are you connected to the internet? ")  
                   return False
             try:
                z = zipfile.ZipFile(zipf,'r')
                z.extractall()
             except:
                print("Couldnt extract \"" + zipf + " Possibly download corrupted or interrupted.")
                print("Delete it and restart installer to retry")
                return False

         else:
            print("install script not yet available for " + platform.system() + " ... quitting");
            exit(1);

      print ("installing arm-gcc ...")
      try:
         os.rename(exn, install_dir + exn)
      except:
         print("Couldnt rename \"" + exn + "\" to \"" + install_dir + dep_dir + "\". Check target directory status")
         return False
      print ("---[arm-gcc installed]---")
      return True
   else:
      print("found pre-existing arm-gcc install")
      return True

def install_simple_dep(tagname,exn,zipf,url):
   if not os.path.exists(install_dir + exn):
      if not os.path.exists(exn):
         if not os.path.exists(zipf):
            try:
               print ("retrieving " + tagname + " ...")
               urllib.urlretrieve(url,zipf)
            except:
               print("Couldnt retrieve \"" + url + "\". Are you connected to the internet? ")  
               return False
          
         try:
            print ("extracting " + tagname + " ...") 
            z = zipfile.ZipFile(zipf,'r')
            z.extractall()
         except:
            print("Couldnt extract \"" + zipf + " Possibly download corrupted or interrupted.")
            print("Delete it and restart installer to retry")
            return False
      try:
         print("installing " + tagname)
         os.rename(exn,install_dir + exn) 
         print( "---[" + tagname + " installed]---")
         return True
      except:
         print("Couldnt rename \"" + exn + "\" to \"" + install_dir + exn + "\". Check target directory status")
         return False
   else:
      print( "found pre-existing " + tagname + " install")
      return True

def install_quan():
   return install_simple_dep(\
      "quan",
      "quan-trunk-master",
      "quan.zip",
      "https://github.com/kwikius/quan-trunk/archive/master.zip")

def install_stm32_lib():
   return install_simple_dep(\
      "stm32 standard peripherals library",
      "STM32F4xx_DSP_StdPeriph_Lib_V1.5.1",
      "stm32f4_dsp_stdperiph_lib.zip",
      "http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32f4_dsp_stdperiph_lib.zip")

def install_freertos():
   return install_simple_dep(\
      "FreeRTOS",
      "FreeRTOSV8.2.0",
      "FreeRTOS.zip",
      "http://downloads.sourceforge.net/project/freertos/FreeRTOS/V8.2.0/FreeRTOSV8.2.0.zip")

# make the Dependencies.mk
def install_dependencies_mk():
   if os.path.exists(install_dir + 'quantracker-master/Dependencies.mk'):
      print('################################################################################')
      print('Warning Previous Dependencies.mk found. Renaming Old version to Dependencies.bak')
      print('################################################################################')
      os.rename(install_dir + 'quantracker-master/Dependencies.mk',
         install_dir + 'quantracker-master/Dependencies.bak')
   try:
      print("writing Dependencies.mk")
      f = open('Dependencies.mk','w')
   except:
      print("failed to open Dependencies.mk. Is it open already?")
      return False
   try: 
      f.write('#Generated by quantracker_deps_installer.py v1.0\n')
      f.write('OPTIMISATION_LEVEL := O3\n')
      f.write('TOOLCHAIN_GCC_VERSION := 4.9.3\n')
      f.write('TOOLCHAIN_PREFIX := ' + install_dir + 'gcc-arm-none-eabi-4_9-2014q4/\n')
      f.write('QUANTRACKER_DEPENDENCY_LIBS_DIR := ' + install_dir + '\n')
      f.write('STM32_STD_PERIPH_LIB_DIR := ' + install_dir + 'STM32F4xx_DSP_StdPeriph_Lib_V1.5.1/Libraries/\n')
      f.write('FREE_RTOS_DIR := ' + install_dir + 'FreeRTOSV8.2.0/FreeRTOS/\n')
      f.write('QUAN_INCLUDE_PATH := ' + install_dir + 'quan-trunk-master/\n')
      f.write('MAVLINK_INCLUDE_PATH := ' + install_dir + '\n')
      if (platform.system() == 'Windows'):
        f.write('STM32FLASH := ' + install_dir + 'quantracker-master/bin/stm32flash.exe\n')
      else:
        f.write('STM32FLASH := ' + install_dir + 'quantracker-master/bin/stm32flash\n')
      f.close()
   except:
      print("failed writing Dependencies.mk.")
      return False

   try:
      print("installing Dependencies.mk")
      os.rename('Dependencies.mk', install_dir + '/quantracker-master/Dependencies.mk')
   except:
      print("install Dependencies.mk failed")
      return False

   return True   

def install_quantracker():
   return install_simple_dep(\
      "Quantracker",
      "quantracker-master",
      "quantracker.zip",
      "https://github.com/kwikius/quantracker/archive/master.zip")
  
# call after successfully installing quantracker
def install_stm32flash():
   if not os.path.exists(install_dir + "quantracker-master"):
      print("stm32flash needs to have Quantracker installed first")
      return False
   
   if platform.system() == 'Linux':
      if not os.path.exists(install_dir + "quantracker-master/bin/stm32flash"):
         exn = "stm32flash"
         if not os.path.exists(exn):
            tarf = "stm32flash-0.4.tar.gz"
            if not os.path.exists(tarf):
               url = "http://sourceforge.net/projects/stm32flash/files/stm32flash-0.4.tar.gz"
               print("retrieving stm32flash ...")
               try:
                  urllib.urlretrieve(url,tarf)
               except:
                   print("Couldnt retrieve \"" + url + "\". Are you connected to the internet? ")  
                   return False
            try:
               print ("extracting stm32flash ...")
               t = tarfile.open(tarf,'r')
               t.extractall()  
            except:
               print("Couldnt extract \"" + tarf + " Possibly download corrupted or interrupted.")
               print("Delete it and restart installer to retry")
               return False
         try: 
            print("building stm32flash ...")
            os.system ("make -C " + exn)
         except:
            printf("unknown error in making stm32flash")
            return False
         if not os.path.exists(exn + "/stm32flash"):
            print("Failed to build stm32flash")
            return False
         try:
            print("installing stm32flash ...")
            os.rename(exn + "/stm32flash", install_dir + "quantracker-master/bin/stm32flash")
            print("---[stm32flash installed]---")
            return True
         except:
            print("Couldnt rename \"" + exn + "\" to \"" + install_dir + exn + "\". Check target directory status")
            return False
         
      else:
         print("found pre-existing stm32flash linux install")
         return True

   elif platform.system() == 'Windows':
      if not os.path.exists( install_dir + "quantracker-master/bin/stm32flash.exe"):
         stm32flash_stub_path = "quantracker-master/bin/stm32flash_win.zip"
         stm32flash_path = install_dir + stm32flash_stub_path
         if not os.path.exists(stm32flash_path):
            print("cant find " + stm32flash_path)
            return False
         print("extracting stm32flash");
         try:
            z = zipfile.Zipfile(stm32flash_path)
            z.extractall(install_dir + "quantracker-master/bin/")
            print("---[stm32flash installed]---")
            return True
         except:
            print("Couldnt extract \"" + stm32flash_path + "\" Possibly download corrupted or interrupted.")
            print("Delete quantracker and restart installer to retry")
            return False
      else:
         print("found pre-existing stm32flash Windows install") 
         return True
   else:
      #shouldnt get here
      print("unknown os .. quitting")
      exit(-1)


#-------------------------main------------------

if ( platform.system() != 'Linux') and ( platform.system() != 'Windows'):
   printf("Sorry.. The script can currently only handle Windows or Linux")
   printf("Please report your OS and we will try to fix that")
   exit(-1)

if (len(sys.argv) > 1):
   install_dir = sys.argv[1]
while not install_dir_good():
   install_dir = raw_input("Please enter the Directory to install dependencies or Q to quit: ")
   if (install_dir == "Q") or (install_dir == "q"):
      print("Quit requested ... quitting install")
      exit()

install_dir = normalise_install_dir()

installed_ok = \
      install_arm_gcc() and \
      install_stm32_lib() and \
      install_freertos() and \
      install_quan() and \
      install_mavlink() and \
      install_quantracker() and \
      install_stm32flash() and \
      install_dependencies_mk()

if installed_ok:
   print("===============================================")
   print("Quantracker installation completed successfully")
   print("===============================================")
   print("===============================================")
   build = raw_input ("If you want the script to build a firmware\n\
   for you now, type \'yes\' ( anything else to exit)\n:")
   if (build == 'yes'):
      os.chdir(install_dir + "quantracker-master")
      os.system("make osd_example1")
      if os.path.exists(install_dir + "quantracker-master/examples/osd_example1/board/bin/main.bin"):
         print("firmware build successful")
         upload = raw_input("If you want to try uploading, attach your board to the usb,\
         apply the prog jumper, power the board and type \'yes\'")
         if (upload == 'yes'):
            os.system("make upload_osd_example1")
   else:
      print("quantracker install script quitting")
   print("===============================================")
 








