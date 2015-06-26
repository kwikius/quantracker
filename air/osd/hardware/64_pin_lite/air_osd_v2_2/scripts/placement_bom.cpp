/*
in eeschema do BOM to to ~/kicad_project/components/bom.csv


in pcbnew  do Fabrication Outputs > Module Positions file

to ~/kicad_project/components/placement/
use this app to format BOM 

(Remember to apply correct origin)
in pcbnew do Gerber and drill file to  ~/temp
apply gerber_output_renaming.py 

in pcbnew do Gerber postscript to ~/temp
and do mirrored to ~/temp/mirrored
apply gerber_postscript_renaming.py

in eeschema do schematic output
move schematics to ~/schematic/postscript
apply schematic_to_pdf.py
*/

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <cassert>
#include <initializer_list>

#include <quan/two_d/out/vect.hpp>
#include <quan/out/angle.hpp>
#include <quan/out/length.hpp>
#include <quan/conversion/double_convert.hpp>

struct component{
   std::string reference;
   std::string value;
   std::string package;
   std::string manufacturer;
   std::string part_number;
   std::string description;
   enum side_t{unknown,front,back};
   side_t side;
   quan::two_d::vect<quan::length::in> position;
   quan::angle::deg rotation;
};

std::string proj_dir = "/home/andy/cpp/projects/quantracker/air/osd/"
"hardware/64_pin_lite/air_osd_v2_2/";

std::string bom_output_dir =  "components/placement_bom.csv";

std::map<std::string, component> comp_map;

/*Rotation from the Zero Orientation  of 
the Kicad 

Applies to all IC' footprints
*/
std::map<std::string,quan::angle::deg> custom_footprint_rotation 
  = 
   {
      {"LQFP64",quan::angle::deg{90}},
      {"SC70-5",quan::angle::deg{90}},
      {"DFN2532",quan::angle::deg{90}},
      {"SOT23-5",quan::angle::deg{90}},
      {"SOT23-6",quan::angle::deg{90}},
      {"NSOIC-16",quan::angle::deg{90}},
      {"MSOP-10",quan::angle::deg{90}},
      {"MSOP-8",quan::angle::deg{90}},
      {"LPS4018",quan::angle::deg{90}}
   };

void read_bom();
bool read_pos();

void output_bom(std::string const & filename);

// list of component refs not to appear in PositionFile.csv
std::set<std::string> no_placement_set
   = {"K1","P1","P2","P3","P4","P5","P6","P7","P8","P9","P10","P11","P12"};

int main()
{

   read_bom();
   if (read_pos()){
      std::cout << "success\n";
   }else{
     std::cout << "fail\n";
   }
   output_bom(proj_dir + bom_output_dir);
}

bool read_pos_file(std::string const & filename,component::side_t side);
bool read_pos()
{
   return 
      read_pos_file(proj_dir + "kicad_project/components/placement/osd_Front.pos",component::front)
      && read_pos_file(proj_dir + "kicad_project/components/placement/osd_Back.pos",component::back);
}

bool read_pos_file(std::string const & filename,component::side_t side)
{
   assert ( ((side == component::front)|| (side == component::back) ) && "unknown side");
   std::ifstream pos_in{filename};
   assert(pos_in && "file failed");
   while (pos_in && !pos_in.eof()){
      std::string str;
      std::getline(pos_in,str);
     // bool erc = false;
      if( (str.length() > 0) && ( str.at(0) != '#')){
         auto end = str.find(' ');
         if ( end == std::string::npos){
            //erc = true;
            break;
         }else{
            std::string ref_name = str.substr(0,end);
            
            str = str.substr(end+1,std::string::npos);
            auto got_ref = comp_map.find(ref_name);
            if (got_ref != comp_map.end()){
               auto & ref = got_ref->second;
               std::string ar[4];
               for ( std::size_t i = 0; i < 4; ++i){
                  auto begin = str.find_first_not_of(" ");
                  assert( (begin != std::string::npos) && __LINE__);
                  str = str.substr(begin,std::string::npos);
                  end = str.find(' ');
                  ar[i] = str.substr(0,end);
                  str = str.substr(end,std::string::npos);
               }  
               if ( ref.value != ar[0]){
                  std::cout << ref.reference << ": " ;
                  std::cout <<"Value compare failed\n";
                  return false;
               }
               quan::detail::converter<double,char*> conv;
               double x = conv(ar[1].c_str());
               assert(conv.get_errno() == 0);
               double y = conv(ar[2].c_str());
               assert(conv.get_errno() == 0);
               double rot = conv(ar[3].c_str());
               assert(conv.get_errno() == 0);
               ref.position 
                  = quan::two_d::vect<quan::length::in>{quan::length::in{x},quan::length::in{y}};
               ref.rotation = quan::angle::deg{rot};
               ref.side = side;
            }else{
               std::cout << "couldnt find " << ref_name  << " quitting\n";
              // erc = true;
               return false;
            }
         }
      }
   }
   return true;
}

void read_bom()
{
   // open input
   std::ifstream bom_in{ proj_dir + "kicad_project/components/bom.csv"};
   int line_count = 0;
   while ( bom_in && ! bom_in.eof()){
      std::string str;
      std::getline(bom_in,str);
      ++ line_count;
      if ( line_count > 1){
        
         std::string ar[6];
         for ( std::size_t i = 0; i < 5; ++i){
            auto end = str.find(';');
            if ( end != std::string::npos){
               ar[i] = str.substr(0,end);
               str = str.substr(end+1,std::string::npos);
            }else{
               break;
            }
         }
         component cm;
         cm.reference    = ar[0];
         cm.value        = ar[1];
         cm.package      = ar[2];
         cm.manufacturer = ar[3];
         cm.part_number  = ar[4];
         cm.description  = str;
         if ( cm.reference !=  ""){
            std::cout << "[ref:" << cm.reference  <<"] [val:" << cm.value  <<"] [pkg:" 
            << cm.package << "] [mfc:" << cm.manufacturer << "] [part:" << cm.part_number 
            << "] [descr:" << cm.description <<"]\n";
            comp_map[cm.reference] = cm;
         }
      }
   }
}

void output_bom(std::string const & filename)
{
   std::ofstream out(filename);
   assert (out && "output file failed");
    // units 
   out <<" Designator, Footprint, Mid X (inches) , Mid Y (inches), TB, Rotation (degrees), Value, Manufacturer, Manufacturer Part No., Comment\n";
   for ( auto const & iter : comp_map){
      auto const & comp = iter.second;
      if ( no_placement_set.find(comp.reference) == no_placement_set.end()){
         out << comp.reference << ", ";
         out << comp.package   << ", ";


         out << comp.position.x.numeric_value() << ", ";
         out << comp.position.y.numeric_value() << ", ";

         
         if ( comp.side == component::front){
            out << "Top, ";
         }else{
            out << "Bottom, ";
         }
         quan::angle::deg rotation = comp.rotation ;
          // ignore rotation on Fiducials
         if ( comp.reference.substr(0,3) != "FID" ){
            auto const custom_rotation = custom_footprint_rotation.find(comp.package);
            if ( custom_rotation !=custom_footprint_rotation.end()){
               if (comp.side == component::front){
                   rotation += custom_rotation->second;
               }else{
                   rotation -= custom_rotation->second;
               }
            }else{
            }
         }
         while (rotation >= quan::angle::deg{360}){
            rotation -= quan::angle::deg{360};
         }
         while (rotation < quan::angle::deg{0}){
            rotation += quan::angle::deg{360};
         }
         out << rotation.numeric_value() << ", ";
         out << comp.value << ", ";
         out << comp.manufacturer << ", ";
         out << comp.part_number << ", ";
         out << "\"" << comp.description << "\"\n";
         
      }
    }
}
