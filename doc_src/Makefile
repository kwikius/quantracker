# quantracker doc makefile


root_rst = intro.rst firmware_upload.rst firmware_upload_sp.rst \
firmware_upload_st_link.rst osd_getting_started.rst index.rst \
trouble_shooting.rst

root_html = $(patsubst %.rst,../doc/%.html,$(root_rst))

blink_html = ../doc/air/osd/board_test/blink/readme.html

objects = $(root_html) $(blink_html)

all : $(objects)

$(root_html) : ../doc/%.html : %.rst
	rst2html $< $@

$(blink_html) : blink_readme.rst
	rst2html $< $@


