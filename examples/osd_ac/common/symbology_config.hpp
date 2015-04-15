#ifndef QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED
#define QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED

#define CHAR_SPACING_X 12       // default size of one character including inter-character space
#define CHAR_SPACING_Y 15       // default size of one character including inter-character space

#define OSD_MODE_VALUE_PAGE_1  1100      // This mode if input value less than this value (and not less than the value above
#define OSD_MODE_VALUE_PAGE_2  1300      // This mode if input value less than this value (and not less than the value above
#define OSD_MODE_VALUE_PAGE_3  1500      // This mode if input value less than this value
#define OSD_MODE_VALUE_PAGE_4  1700      // This mode if input value less than this value

#define OSD_MODE_PAGE_1  0x01     // Code to indicate OSD mode is page 1  (as a bit mask)
#define OSD_MODE_PAGE_2  0x02      // Code to indicate OSD mode is page 2  (as a bit mask)
#define OSD_MODE_PAGE_3  0x04      // Code to indicate OSD mode is page 3  (as a bit mask)
#define OSD_MODE_PAGE_4  0x08      // Code to indicate OSD mode is page 4  (as a bit mask)

#define ENABLE_OFF        0x00
#define ENABLE_ON_PAGE_1  0x01
#define ENABLE_ON_PAGE_2  0x02
#define ENABLE_ON_PAGE_3  0x04
#define ENABLE_ON_PAGE_4  0x04

#define ENABLE_ON_PAGE_ALL (ENABLE_ON_PAGE_1 | ENABLE_ON_PAGE_2 | ENABLE_ON_PAGE_3 | ENABLE_ON_PAGE_4)
#define ENABLE_ON_PAGE_1_2_3 (ENABLE_ON_PAGE_1 | ENABLE_ON_PAGE_2 | ENABLE_ON_PAGE_3)


#endif // QUANTRACKER_SYMBOLOGY_CONFIG_HPP_INCLUDED

