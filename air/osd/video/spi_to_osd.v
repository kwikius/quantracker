
module main;

   input wire reset;
   input wire spi_clk;
   input wire spi_data;

   reg[1:0] local_clk = 2'b11;
   reg[3:0] shift_reg = 4'b0000;

   output reg [3:0] osd_colour = 4'b0000;
   output reg write = 1'b1;
   output reg video_src = 1'b0;

   always @ (posedge spi_clk) 
   begin
      if ( reset == 1'b1 ) 
      begin
         local_clk <= 2'b11;
         shift_reg <= 4'b0000;
         osd_colour <= 4'b0000;
         video_src <= 1'b0;
      end
      else 
      begin
         local_clk <= local_clk + 1 ;
         shift_reg <= (shift_reg << 1) | spi_data ;
         if(osd_colour == 4'b0000)
         begin
            video_src <= 1'b0;
         end
         else
         begin
            video_src <= 1'b1;
         end
      end
   end

   always @ (negedge spi_clk)  
   begin
      if ( reset == 1 ) 
      begin
         local_clk <= 2'b11;
         shift_reg <= 4'b0000;
         osd_colour <= 4'b0000;
         video_src = 1'b0;
         write <= 1'b1;
      end
      else
      begin
         case(local_clk) 
         2'b01: 
         begin
            if( osd_colour != 0)
            begin
               write = 1'b0;
            end
         end
         2'b10: 
         begin
            if( osd_colour != 0)
            begin
               write = 1'b1;
            end
         end
         2'b11:
         begin
            osd_colour <= shift_reg;
         end
         default:
         begin
         end
         endcase
      end
   end
endmodule
