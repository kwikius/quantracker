#include "flash.hpp"
#include <quan/stm32/detail/flash.hpp>
#include <quan/three_d/vect.hpp>
#include <cstring>
#include <cctype>
#include <quan/conversion/float_convert.hpp>
#include <quan/conversion/itoa.hpp>
#include <quan/dynarray.hpp>
#include "../serial_ports.hpp"

 
void user_message (const char* str)
{
   sliprings::serial_port::write (str) ;
}

void flush_sp_tx()
{
  while (!sliprings::serial_port::tx_reg_empty()){;}
}
 
namespace {
   // expect <symbol name> = <value>
   bool parse_symbol_assignment (
      quan::dynarray<char> const & input,
      quan::dynarray<char> & symbol, 
      quan::dynarray<char> & value)
   {
      char* const symbol_part = strtok (input.get()," \t");
      char* const value_part = strtok (nullptr," \t");

      if (symbol_part == nullptr) {
         user_error ("expected symbol");
         return false;
      }
      if (value_part == nullptr) {
         user_error ("expected value");
         return false;
      }
      // show values here
      size_t const symbol_len = strlen (symbol_part);
     // if (symbol.get_num_elements() < (symbol_len +1)) {
         if (! symbol.realloc (symbol_len +1)) {
            quan::error (fn_parse_input,quan::detail::out_of_heap_memory);
            return false;
         }
      //}
      size_t const value_len = strlen (value_part);
     // if (value.get_num_elements() < (value_len +1)) {
         if (! value.realloc (value_len +1)) {
            quan::error (fn_parse_input,quan::detail::out_of_heap_memory);
            return false;
         }
     // }
      strcpy (symbol.get(), symbol_part);
      strcpy (value.get(), value_part);
      return true;
   }

   bool set_symbol_value (quan::dynarray<char> const & input)
   {
      quan::dynarray<char> symbol {20,main_alloc_failed};
      quan::dynarray<char> value {20,main_alloc_failed};
      
      if (! (symbol.good() && value.good())) {
         return false;
      }
      if (!parse_symbol_assignment (input,symbol,value)) {
         return get_num_errors() == 0;
      }
      
      int32_t const symbol_index = flash_symtab::get_index (symbol);
      if (symbol_index == -1) {
         user_error ("symbol not found");
         return get_num_errors() == 0;
      }
     bool is_readonly = false;
      if (!flash_symtab::get_readonly_status (symbol_index,is_readonly)) {
         //shouldnt get here
         return false;
      }
      if (is_readonly) {
         user_error ("symbol is readonly");
         return true;
      }

      bool const result =  flash_symtab::write_from_string (symbol_index,value) ;

      user_message("write ");
      if (result) {
         user_message("succeeded\n");
      } else {
         user_message ("failed\n");
      }
      return get_num_errors() == 0;
   }

 
/*
   show list of flash symbols with values
   n.b symbol could be valid but not set
*/
 
   bool local_show_symbol (uint16_t symbol_index)
   {
      if (flash_symtab::exists (symbol_index)) {
      
         quan::dynarray<char> value {1,main_alloc_failed};
         
         if (!value.good()) {
            return false;
         }
         if (flash_symtab::read_to_string (symbol_index,value)) {
            user_message (flash_symtab::get_name (symbol_index));
            user_message (" : ");
            user_message( value.get());
            user_message ("\n");
            return true;
         } else {
            return get_num_errors() == 0;
         }
      } else {
         user_message (flash_symtab::get_name (symbol_index));
         user_message (" : #undef#\n");
         return true;
      }
   }

 
bool show_symbols (quan::dynarray<char> const & opt_symbol)
{
   if (strlen (opt_symbol.get()) > 0) {
      int32_t symbol_index = flash_symtab::get_index (opt_symbol);
      if (symbol_index == -1) {
         user_error ("symbol not found");
         return true;
      } else {
         return local_show_symbol (symbol_index);
      }
   } else {
      user_message ("------flash symbols-------\n");
      for (uint16_t i = 0; i < flash_symtab::get_num_elements(); ++i) {
         if (! local_show_symbol (i)) {
            return false;
         }
      }
      user_message ("------------------------\n");
      return true;
   }
}
 
bool help (quan::dynarray<char> const &);
bool command_exit (quan::dynarray<char> const &);
 
struct command {
   const char* const name;
   bool (*function) (quan::dynarray<char> const &);
   const char * info;
};
 
command command_array [] =
{
   {"help",help, "help [<opt_command>] : list available commands or opt_command info if specified."}
   ,{"set", set_symbol_value, "set <symbol> <value> : set flash symbol to value."}
   ,{"show", show_symbols, "show [<opt_symbol>] : list all symbols or opt_symbol if specified."}
   ,{"exit", command_exit,"exit : exit this level."}
};
 
bool help (quan::dynarray<char> const & cmd_in)
{
   if (strlen (cmd_in.get()) > 0) {
      bool found = false;
      for (auto const & cmd : command_array) {
         if (strcmp (cmd.name, cmd_in.get()) == 0) {
            found = true;
            user_message (cmd.info);
            user_message ("\n");
         }
      }
      if (found == false) {
         user_message ("command not found\n");
      }
   } else {
      user_message ("------available commands------\n");
      for (auto const & cmd : command_array) {
         user_message (cmd.info);
         user_message ("\n");
      }
      user_message ("------------------------------\n");
   }
   return true;
}
 
bool command_exit (quan::dynarray<char> const & cmd_in)
{
   user_message ("quitting\n");
   return true;
}
}// namespace 
 
//return false if bad error(e.g out of memory etc)
bool flash_menu()
{
   for (;;) {
      user_message ("\n");
      char buffer[100];
      int32_t idx = 0;
      while (1) {
         if (sliprings::serial_port::in_avail()) {
            char ch = sliprings::serial_port::get();
            if (ch == '\r') {
               buffer[idx] = '\0';
               ++idx;
               break;
            } else {
               if (idx > 99) {
                  sliprings::serial_port::write ("input too long\n");
                  idx = 0;
               } else {
                  buffer[idx] = ch;
                  ++idx;
               }
            }
         }
      }
      if (idx > 0) {
         quan::dynarray<char> str1 (idx,main_alloc_failed);
         if (!str1.good()) {
            return false;
         }
         strcpy (str1.get(),buffer);
         const char* cmd_in = strtok (str1.get()," \t");
         const char* rest = strtok (nullptr,"");
         if ( (cmd_in != nullptr)) {
            bool command_found = false;
            for (command const & cmd : command_array) {
               if (strcmp (cmd_in, cmd.name) == 0) {
                  command_found = true;
                  quan::dynarray<char> rest_out {1,main_alloc_failed};
                  if (!rest_out.good()) {
                     return false;
                  }
                  *rest_out.get() = '\0';
                  if (rest) {
                     if (! rest_out.realloc (strlen (rest) + 1, main_alloc_failed)) {
                        return false;
                     }
                     strcpy (rest_out.get(),rest);
                  }
         
                  if (! cmd.function (rest_out)) {
                     return false;
                  }
               }
            }
            if (command_found == false) {
               user_message ("command not found\n");
            } else {
               if (strcmp (cmd_in,"exit") == 0) {
                  return true;
               } else {
                  continue;
               }
            }
         }
      }
   }
   return true;
}


 
