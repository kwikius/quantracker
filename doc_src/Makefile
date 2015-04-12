# quantracker doc makefile

root_rst = intro.rst firmware_upload.rst firmware_upload_sp.rst \
firmware_upload_st_link.rst osd_getting_started.rst index.rst \
trouble_shooting.rst
osd_rst = index.rst craft_features_widgets.rst flash_menu.rst
osd_software_devel_rst = bitmap_format.rst flash_variables.rst
osd_maker_rst = index.rst

root_html = $(patsubst %.rst,../doc/%.html,$(root_rst))
osd_html = $(patsubst %.rst,../doc/osd/%.html,$(osd_rst))
osd_software_devel_html = $(patsubst %.rst,../doc/osd/software_devel/%.html,$(osd_software_devel_rst))
osd_maker_html = $(patsubst %.rst,../doc/osd/osd_maker/%.html,$(osd_maker_rst))

blink_html = ../doc/air/osd/board_test/blink/readme.html

objects = $(root_html) $(blink_html) $(osd_html) $(osd_software_devel_html) $(osd_maker_html)

all : $(objects)

$(root_html) : ../doc/%.html : %.rst
	rst2html $< $@

$(osd_html) : ../doc/osd/%.html : osd/%.rst
	rst2html $< $@

$(osd_software_devel_html) : ../doc/osd/software_devel/%.html : osd/software_devel/%.rst
	rst2html $< $@

$(osd_maker_html) : ../doc/osd/osd_maker/%.html : osd/osd_maker/%.rst
	rst2html $< $@

$(blink_html) : blink_readme.rst
	rst2html $< $@


