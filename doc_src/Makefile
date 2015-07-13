# quantracker doc makefile

root_rst = intro.rst firmware_upload.rst \
firmware_upload_sp.rst firmware_upload_st_link.rst osd_getting_started.rst \
index.rst trouble_shooting.rst v1_board.rst

osd_rst = index.rst craft_features_widgets.rst flash_menu.rst \
firmwares.rst flash_vars_utility.rst

osd_firmwares_rst = osd_example1.rst osd_ac.rst osd_telem_tx.rst osd_telem_rx.rst

osd_software_devel_rst = bitmap_format.rst flash_variables.rst source_code_organisation.rst \
toolchain.rst dependencies_mk.rst

osd_software_devel_tech_rst = index.rst telemetry.rst

osd_maker_rst = index.rst

quan_rst = index.rst i2c.rst drawing_api.rst on_draw.rst

quan_html = $(patsubst %.rst,../doc/osd/software_devel/tech/quan/%.html,$(quan_rst))

root_html = $(patsubst %.rst,../doc/%.html,$(root_rst))
osd_html = $(patsubst %.rst,../doc/osd/%.html,$(osd_rst))
osd_firmwares_html = $(patsubst %.rst,../doc/osd/firmwares/%.html,$(osd_firmwares_rst))
osd_software_devel_html = $(patsubst %.rst,../doc/osd/software_devel/%.html,$(osd_software_devel_rst))
osd_maker_html = $(patsubst %.rst,../doc/osd/osd_maker/%.html,$(osd_maker_rst))
osd_software_devel_tech_html = $(patsubst %.rst,../doc/osd/software_devel/tech/%.html,$(osd_software_devel_tech_rst))

blink_html = ../doc/air/osd/board_test/blink/readme.html

objects = $(root_html) $(blink_html) $(osd_html) $(osd_firmwares_html) \
$(osd_software_devel_html) $(osd_maker_html) $(osd_software_devel_tech_html) \
$(quan_html)

all : $(objects)
	cp -r -u /home/andy/cpp/projects/quantracker/doc/* /home/andy/website/fpv/quantracker/doc/

$(quan_html) :  ../doc/osd/software_devel/tech/quan/%.html : osd/software_devel/tech/quan/%.rst
	rst2html $< $@

$(root_html) : ../doc/%.html : %.rst
	rst2html $< $@

$(osd_html) : ../doc/osd/%.html : osd/%.rst
	rst2html $< $@

$(osd_firmwares_html) : ../doc/osd/firmwares/%.html : osd/firmwares/%.rst
	rst2html $< $@

$(osd_software_devel_html) : ../doc/osd/software_devel/%.html : osd/software_devel/%.rst
	rst2html $< $@

$(osd_maker_html) : ../doc/osd/osd_maker/%.html : osd/osd_maker/%.rst
	rst2html $< $@

$(osd_software_devel_tech_html) : ../doc/osd/software_devel/tech/%.html : osd/software_devel/tech/%.rst
	rst2html $< $@

$(blink_html) : blink_readme.rst
	rst2html $< $@
