// hiptext - Image to Text Converter
// Copyright (c) 2013 Justine Tunney

#include "macterm.h"

#include <cstdint>
#include <array>
#include <utility>

#include "pixel.h"

MactermColor::MactermColor(const Pixel& top, const Pixel& bot) {
  struct Match {
    uint8_t code;
    double dist;
  };
  Match best_fg_top = {0, 1e6};
  Match best_fg_bot = {0, 1e6};
  Match best_bg_top = {0, 1e6};
  Match best_bg_bot = {0, 1e6};
  double dist;
  for (int code = 16; code < 256; ++code) {
    dist = top.Distance(macterm_colors[0][code]);
    if (dist < best_fg_top.dist) {
      best_fg_top.code = code;
      best_fg_top.dist = dist;
    }
    dist = bot.Distance(macterm_colors[0][code]);
    if (dist < best_fg_bot.dist) {
      best_fg_bot.code = code;
      best_fg_bot.dist = dist;
    }
  }
  for (int code = 16; code < 256; ++code) {
    dist = top.Distance(macterm_colors[1][code]);
    if (dist < best_bg_top.dist) {
      best_bg_top.code = code;
      best_bg_top.dist = dist;
    }
    dist = bot.Distance(macterm_colors[1][code]);
    if (dist < best_bg_bot.dist) {
      best_bg_bot.code = code;
      best_bg_bot.dist = dist;
    }
  }
  if (best_fg_top.dist + best_bg_bot.dist <=
      best_bg_top.dist + best_fg_bot.dist) {
    is_upper_ = true;
    fg_ = best_fg_top.code;
    bg_ = best_bg_bot.code;
  } else {
    is_upper_ = false;
    fg_ = best_fg_bot.code;
    bg_ = best_bg_top.code;
  }
}

// Terminal.app on Mac OS X is interesting. First of all, it doesn't follow the
// xterm-256color standard, but that's probably for the best since xterm's
// palette was obviously chosen by engineers rather than designers. The problem
// is I'm not quite sure what Terminal.app is doing. It slightly ajusts the
// color when displaying foregrounds and backgrounds but I'm not sure what
// formula they're using to do it. They also seem to slightly alter colors
// depending on the terminal theme. The following colors are what I scraped
// from Mac OS X 10.8 Mountain Lion Terminal.app with a black background and
// the "Pro" theme.
const Pixel macterm_colors[2][256] = {
  [0][1]   = {192,  55,  41},
  [1][1]   = {151,   4,  12},
  [0][2]   = { 48, 187,  49},
  [1][2]   = { 23, 164,  26},
  [0][3]   = {173, 172,  53},
  [1][3]   = {153, 152,  29},
  [0][4]   = { 73,  76, 224},
  [1][4]   = {  8,  43, 181},
  [0][5]   = {209,  65, 209},
  [1][5]   = {177,  25, 176},
  [0][6]   = { 60, 187, 199},
  [1][6]   = { 26, 166, 177},
  [0][7]   = {203, 204, 205},
  [1][7]   = {191, 191, 191},
  [0][8]   = {154, 155, 156},
  [1][8]   = {132, 132, 132},
  [0][9]   = {249,  59,  44},
  [1][9]   = {227,  10,  23},
  [0][10]  = { 62, 229,  55},
  [1][10]  = { 33, 215,  38},
  [0][11]  = {234, 234,  62},
  [1][11]  = {229, 228,  49},
  [0][12]  = { 89,  63, 251},
  [1][12]  = { 11,  36, 251},
  [0][13]  = {247,  67, 245},
  [1][13]  = {227,  35, 227},
  [0][14]  = { 48, 239, 239},
  [1][14]  = { 39, 229, 228},
  [0][15]  = {233, 235, 235},
  [1][15]  = {230, 229, 230},
  [0][16]  = { 47,  49,  49},
  [1][16]  = {  0,   0,   0},
  [0][17]  = { 60,  46, 142},
  [1][17]  = {  1,   7,  93},
  [0][18]  = { 67,  51, 180},
  [1][18]  = {  3,  14, 133},
  [0][19]  = { 74,  56, 218},
  [1][19]  = {  5,  21, 172},
  [0][20]  = { 82,  62, 248},
  [1][20]  = {  7,  28, 211},
  [0][21]  = { 89,  63, 251},
  [1][21]  = { 11,  36, 251},
  [0][22]  = { 46, 127,  43},
  [1][22]  = {  8,  94,  11},
  [0][23]  = { 46, 127, 127},
  [1][23]  = { 10,  95,  95},
  [0][24]  = { 49, 127, 167},
  [1][24]  = { 11,  96, 133},
  [0][25]  = { 60, 127, 207},
  [1][25]  = { 13,  97, 173},
  [0][26]  = { 63, 127, 241},
  [1][26]  = { 15,  99, 212},
  [0][27]  = { 70, 126, 251},
  [1][27]  = { 18, 101, 251},
  [0][28]  = { 57, 161,  50},
  [1][28]  = { 17, 134,  20},
  [0][29]  = { 47, 161, 122},
  [1][29]  = { 18, 134,  96},
  [0][30]  = { 50, 161, 161},
  [1][30]  = { 18, 135, 134},
  [0][31]  = { 60, 161, 200},
  [1][31]  = { 20, 136, 173},
  [0][32]  = { 53, 161, 237},
  [1][32]  = { 21, 137, 212},
  [0][33]  = { 65, 160, 252},
  [1][33]  = { 23, 138, 251},
  [0][34]  = { 45, 194,  51},
  [1][34]  = { 25, 173,  29},
  [0][35]  = { 58, 194, 118},
  [1][35]  = { 26, 174,  98},
  [0][36]  = { 51, 194, 156},
  [1][36]  = { 26, 174, 136},
  [0][37]  = { 59, 194, 194},
  [1][37]  = { 27, 175, 174},
  [0][38]  = { 48, 194, 233},
  [1][38]  = { 28, 175, 213},
  [0][39]  = { 63, 194, 252},
  [1][39]  = { 30, 176, 252},
  [0][40]  = { 62, 227,  55},
  [1][40]  = { 33, 213,  38},
  [0][41]  = { 55, 227, 115},
  [1][41]  = { 34, 213, 101},
  [0][42]  = { 48, 227, 151},
  [1][42]  = { 34, 214, 137},
  [0][43]  = { 63, 227, 189},
  [1][43]  = { 35, 214, 175},
  [0][44]  = { 52, 227, 227},
  [1][44]  = { 36, 215, 214},
  [0][45]  = { 66, 227, 253},
  [1][45]  = { 37, 215, 253},
  [0][46]  = { 51, 252,  57},
  [1][46]  = { 41, 253,  47},
  [0][47]  = { 53, 252, 111},
  [1][47]  = { 42, 253, 104},
  [0][48]  = { 63, 252, 147},
  [1][48]  = { 42, 253, 140},
  [0][49]  = { 58, 252, 184},
  [1][49]  = { 43, 253, 177},
  [0][50]  = { 59, 252, 222},
  [1][50]  = { 44, 254, 215},
  [0][51]  = { 62, 253, 251},
  [1][51]  = { 45, 255, 254},
  [0][52]  = {138,  49,  43},
  [1][52]  = { 94,   2,   4},
  [0][53]  = {134,  55, 134},
  [1][53]  = { 94,   8,  94},
  [0][54]  = {134,  58, 174},
  [1][54]  = { 94,  15, 133},
  [0][55]  = {133,  61, 213},
  [1][55]  = { 94,  22, 172},
  [0][56]  = {134,  67, 244},
  [1][56]  = { 95,  29, 211},
  [0][57]  = {134,  65, 251},
  [1][57]  = { 95,  36, 251},
  [0][58]  = {125, 124,  46},
  [1][58]  = { 95,  94,  14},
  [0][59]  = {124, 125, 125},
  [1][59]  = { 95,  95,  95},
  [0][60]  = {122, 125, 165},
  [1][60]  = { 95,  96, 134},
  [0][61]  = {123, 126, 205},
  [1][61]  = { 95,  97, 173},
  [0][62]  = {124, 126, 243},
  [1][62]  = { 96,  99, 212},
  [0][63]  = {125, 125, 251},
  [1][63]  = { 96, 101, 251},
  [0][64]  = {120, 159,  47},
  [1][64]  = { 96, 134,  22},
  [0][65]  = {118, 159, 121},
  [1][65]  = { 96, 134,  96},
  [0][66]  = {118, 159, 160},
  [1][66]  = { 96, 135, 135},
  [0][67]  = {117, 160, 199},
  [1][67]  = { 97, 136, 173},
  [0][68]  = {118, 160, 238},
  [1][68]  = { 97, 137, 212},
  [0][69]  = {119, 160, 252},
  [1][69]  = { 97, 138, 252},
  [0][70]  = {115, 193,  53},
  [1][70]  = { 98, 173,  30},
  [0][71]  = {113, 193, 117},
  [1][71]  = { 98, 174,  98},
  [0][72]  = {114, 193, 155},
  [1][72]  = { 98, 174, 136},
  [0][73]  = {111, 193, 193},
  [1][73]  = { 98, 175, 174},
  [0][74]  = {113, 194, 232},
  [1][74]  = { 98, 176, 213},
  [0][75]  = {112, 193, 252},
  [1][75]  = { 99, 177, 252},
  [0][76]  = {110, 226,  53},
  [1][76]  = {100, 213,  39},
  [0][77]  = {108, 226, 114},
  [1][77]  = {100, 213, 101},
  [0][78]  = {110, 226, 151},
  [1][78]  = {100, 214, 138},
  [0][79]  = {106, 226, 189},
  [1][79]  = {100, 214, 176},
  [0][80]  = {105, 227, 227},
  [1][80]  = {101, 215, 214},
  [0][81]  = {105, 226, 252},
  [1][81]  = {101, 215, 253},
  [0][82]  = { 99, 251,  59},
  [1][82]  = {102, 253,  48},
  [0][83]  = {103, 251, 111},
  [1][83]  = {103, 253, 104},
  [0][84]  = {104, 251, 146},
  [1][84]  = {103, 253, 140},
  [0][85]  = { 97, 252, 184},
  [1][85]  = {103, 253, 177},
  [0][86]  = {102, 252, 221},
  [1][86]  = {103, 254, 215},
  [0][87]  = { 98, 254, 250},
  [1][87]  = {104, 255, 254},
  [0][88]  = {175,  54,  40},
  [1][88]  = {133,   3,   9},
  [0][89]  = {172,  58, 130},
  [1][89]  = {133,  10,  94},
  [0][90]  = {170,  61, 170},
  [1][90]  = {134,  16, 133},
  [0][91]  = {170,  66, 210},
  [1][91]  = {134,  23, 172},
  [0][92]  = {169,  67, 245},
  [1][92]  = {134,  30, 212},
  [0][93]  = {168,  69, 251},
  [1][93]  = {134,  37, 251},
  [0][94]  = {164, 123,  47},
  [1][94]  = {134,  94,  18},
  [0][95]  = {162, 123, 124},
  [1][95]  = {134,  95,  96},
  [0][96]  = {161, 124, 163},
  [1][96]  = {134,  96, 134},
  [0][97]  = {161, 124, 203},
  [1][97]  = {134,  97, 173},
  [0][98]  = {160, 125, 238},
  [1][98]  = {135,  99, 212},
  [0][99]  = {160, 125, 251},
  [1][99]  = {135, 101, 251},
  [0][100] = {158, 157,  47},
  [1][100] = {135, 134,  25},
  [0][101] = {157, 158, 120},
  [1][101] = {135, 134,  97},
  [0][102] = {157, 158, 159},
  [1][102] = {135, 135, 135},
  [0][103] = {155, 158, 198},
  [1][103] = {135, 136, 174},
  [0][104] = {155, 159, 236},
  [1][104] = {135, 137, 213},
  [0][105] = {155, 158, 252},
  [1][105] = {136, 138, 252},
  [0][106] = {153, 192,  55},
  [1][106] = {136, 173,  32},
  [0][107] = {152, 192, 117},
  [1][107] = {136, 174,  99},
  [0][108] = {151, 192, 154},
  [1][108] = {136, 174, 136},
  [0][109] = {151, 192, 193},
  [1][109] = {136, 175, 175},
  [0][110] = {150, 192, 231},
  [1][110] = {136, 176, 213},
  [0][111] = {150, 192, 251},
  [1][111] = {137, 177, 252},
  [0][112] = {148, 225,  53},
  [1][112] = {137, 213,  40},
  [0][113] = {147, 225, 114},
  [1][113] = {137, 213, 102},
  [0][114] = {146, 225, 150},
  [1][114] = {138, 214, 138},
  [0][115] = {147, 226, 188},
  [1][115] = {138, 214, 176},
  [0][116] = {145, 226, 226},
  [1][116] = {138, 215, 214},
  [0][117] = {145, 226, 250},
  [1][117] = {138, 216, 253},
  [0][118] = {142, 251,  61},
  [1][118] = {139, 253,  49},
  [0][119] = {141, 251, 111},
  [1][119] = {139, 253, 105},
  [0][120] = {141, 252, 146},
  [1][120] = {139, 253, 140},
  [0][121] = {142, 253, 183},
  [1][121] = {139, 254, 178},
  [0][122] = {139, 254, 221},
  [1][122] = {140, 254, 216},
  [0][123] = {138, 255, 249},
  [1][123] = {140, 255, 254},
  [0][124] = {211,  59,  40},
  [1][124] = {173,   6,  15},
  [0][125] = {209,  63, 126},
  [1][125] = {173,  12,  95},
  [0][126] = {207,  63, 166},
  [1][126] = {173,  18, 134},
  [0][127] = {206,  64, 206},
  [1][127] = {173,  24, 173},
  [0][128] = {205,  69, 243},
  [1][128] = {173,  31, 212},
  [0][129] = {204,  72, 252},
  [1][129] = {174,  38, 251},
  [0][130] = {202, 121,  45},
  [1][130] = {173,  95,  22},
  [0][131] = {201, 122, 122},
  [1][131] = {174,  95,  96},
  [0][132] = {200, 122, 161},
  [1][132] = {174,  96, 135},
  [0][133] = {199, 123, 200},
  [1][133] = {174,  98, 173},
  [0][134] = {199, 124, 238},
  [1][134] = {174,  99, 212},
  [0][135] = {197, 124, 252},
  [1][135] = {174, 101, 252},
  [0][136] = {197, 156,  51},
  [1][136] = {174, 134,  28},
  [0][137] = {195, 156, 119},
  [1][137] = {174, 135,  98},
  [0][138] = {195, 157, 157},
  [1][138] = {174, 135, 136},
  [0][139] = {194, 157, 196},
  [1][139] = {174, 136, 174},
  [0][140] = {193, 157, 234},
  [1][140] = {174, 137, 213},
  [0][141] = {193, 157, 252},
  [1][141] = {175, 139, 252},
  [0][142] = {191, 190,  54},
  [1][142] = {175, 174,  35},
  [0][143] = {190, 190, 116},
  [1][143] = {175, 174, 100},
  [0][144] = {189, 191, 153},
  [1][144] = {175, 174, 137},
  [0][145] = {190, 191, 191},
  [1][145] = {175, 175, 175},
  [0][146] = {188, 191, 229},
  [1][146] = {175, 176, 214},
  [0][147] = {187, 191, 251},
  [1][147] = {175, 177, 253},
  [0][148] = {186, 224,  55},
  [1][148] = {176, 213,  43},
  [0][149] = {185, 224, 113},
  [1][149] = {176, 213, 102},
  [0][150] = {184, 224, 150},
  [1][150] = {176, 214, 139},
  [0][151] = {184, 224, 187},
  [1][151] = {176, 214, 176},
  [0][152] = {184, 225, 225},
  [1][152] = {176, 215, 215},
  [0][153] = {182, 224, 251},
  [1][153] = {176, 216, 253},
  [0][154] = {180, 253,  62},
  [1][154] = {177, 253,  51},
  [0][155] = {180, 253, 111},
  [1][155] = {177, 253, 105},
  [0][156] = {179, 253, 146},
  [1][156] = {177, 253, 141},
  [0][157] = {179, 253, 183},
  [1][157] = {177, 254, 178},
  [0][158] = {179, 254, 220},
  [1][158] = {178, 254, 216},
  [0][159] = {177, 252, 249},
  [1][159] = {178, 255, 254},
  [0][160] = {244,  59,  43},
  [1][160] = {213,   9,  21},
  [0][161] = {243,  62, 123},
  [1][161] = {213,  15,  96},
  [0][162] = {241, 65, 162},
  [1][162] = {213, 20, 135},
  [0][163] = {241, 69, 202},
  [1][163] = {241, 69, 202},
  [0][164] = {240, 70, 238},
  [1][164] = {213, 32, 212},
  [0][165] = {238,  69, 252},
  [1][165] = {213,  39, 251},
  [0][166] = {239, 119, 50},
  [1][166] = {213,  95,  27},
  [0][167] = {238, 120, 120},
  [1][167] = {213,  96,  97},
  [0][168] = {236, 121, 159},
  [1][168] = {213,  97, 135},
  [0][169] = {235, 121, 198},
  [1][169] = {213,  98, 174},
  [0][170] = {235, 123, 236},
  [1][170] = {213, 100, 213},
  [0][171] = {234, 123, 252},
  [1][171] = {213, 102, 252},
  [0][172] = {234, 154,  53},
  [1][172] = {213, 134,  32},
  [0][173] = {233, 154, 118},
  [1][173] = {213, 135,  99},
  [0][174] = {232, 155, 156},
  [1][174] = {213, 135, 136},
  [0][175] = {231, 155, 194},
  [1][175] = {214, 136, 175},
  [0][176] = {231, 156, 233},
  [1][176] = {214, 137, 213},
  [0][177] = {230, 156, 252},
  [1][177] = {214, 139, 252},
  [0][178] = {229, 188,  53},
  [1][178] = {214, 174,  38},
  [0][179] = {228, 189, 115},
  [1][179] = {214, 174, 101},
  [0][180] = {227, 189, 152},
  [1][180] = {214, 175, 138},
  [0][181] = {227, 189, 190},
  [1][181] = {214, 175, 176},
  [0][182] = {226, 189, 228},
  [1][182] = {214, 176, 214},
  [0][183] = {225, 189, 253},
  [1][183] = {214, 177, 253},
  [0][184] = {223, 222,  60},
  [1][184] = {215, 213,  45},
  [0][185] = {223, 223, 113},
  [1][185] = {215, 214, 103},
  [0][186] = {222, 223, 149},
  [1][186] = {215, 214, 139},
  [0][187] = {222, 223, 186},
  [1][187] = {215, 214, 177},
  [0][188] = {222, 223, 224},
  [1][188] = {215, 215, 215},
  [0][189] = {220, 223, 252},
  [1][189] = {215, 216, 254},
  [0][190] = {218, 251,  61},
  [1][190] = {216, 253,  53},
  [0][191] = {217, 251, 109},
  [1][191] = {216, 253, 106},
  [0][192] = {217, 251, 145},
  [1][192] = {216, 253, 141},
  [0][193] = {217, 251, 182},
  [1][193] = {216, 254, 178},
  [0][194] = {216, 251, 219},
  [1][194] = {216, 254, 216},
  [0][195] = {216, 251, 250},
  [1][195] = {216, 255, 255},
  [0][196] = {252,  63,  43},
  [1][196] = {252,  13,  27},
  [0][197] = {252,  64, 120},
  [1][197] = {252,  18,  98},
  [0][198] = {252,  64, 159},
  [1][198] = {252,  22, 135},
  [0][199] = {252,  65, 198},
  [1][199] = {252,  28, 174},
  [0][200] = {252,  67, 236},
  [1][200] = {252,  34, 213},
  [0][201] = {252,  72, 252},
  [1][201] = {252,  40, 252},
  [0][202] = {253, 117,  47},
  [1][202] = {252,  96,  32},
  [0][203] = {253, 118, 118},
  [1][203] = {252,  96,  99},
  [0][204] = {253, 119, 156},
  [1][204] = {252,  97, 136},
  [0][205] = {253, 120, 194},
  [1][205] = {253,  99, 175},
  [0][206] = {253, 120, 233},
  [1][206] = {253, 100, 213},
  [0][207] = {252, 121, 252},
  [1][207] = {253, 102, 252},
  [0][208] = {253, 152,  49},
  [1][208] = {253, 135,  36},
  [0][209] = {252, 152, 116},
  [1][209] = {253, 135, 100},
  [0][210] = {252, 153, 153},
  [1][210] = {253, 136, 137},
  [0][211] = {253, 153, 192},
  [1][211] = {253, 137, 175},
  [0][212] = {252, 154, 229},
  [1][212] = {253, 138, 214},
  [0][213] = {251, 154, 251},
  [1][213] = {253, 139, 253},
  [0][214] = {253, 186,  56},
  [1][214] = {253, 174,  42},
  [0][215] = {251, 187, 114},
  [1][215] = {253, 174, 102},
  [0][216] = {251, 187, 151},
  [1][216] = {253, 175, 138},
  [0][217] = {252, 187, 188},
  [1][217] = {253, 175, 176},
  [0][218] = {252, 188, 226},
  [1][218] = {253, 176, 215},
  [0][219] = {251, 188, 251},
  [1][219] = {254, 177, 253},
  [0][220] = {251, 221,  61},
  [1][220] = {254, 213,  48},
  [0][221] = {250, 221, 112},
  [1][221] = {254, 214, 105},
  [0][222] = {250, 221, 148},
  [1][222] = {254, 214, 140},
  [0][223] = {250, 221, 185},
  [1][223] = {254, 215, 177},
  [0][224] = {251, 222, 222},
  [1][224] = {254, 215, 216},
  [0][225] = {251, 222, 251},
  [1][225] = {254, 216, 254},
  [0][226] = {251, 250,  58},
  [1][226] = {255, 253,  56},
  [0][227] = {250, 250, 109},
  [1][227] = {255, 253, 108},
  [0][228] = {249, 250, 144},
  [1][228] = {255, 253, 142},
  [0][229] = {247, 251, 181},
  [1][229] = {255, 254, 179},
  [0][230] = {247, 253, 218},
  [1][230] = {255, 254, 217},
  [0][231] = {254, 255, 255},
  [1][231] = {255, 255, 255},
  [0][232] = { 52,  53,  53},
  [1][232] = {  8,   8,   8},
  [0][233] = { 57,  58,  59},
  [1][233] = { 18,  18,  18},
  [0][234] = { 66,  67,  67},
  [1][234] = { 28,  28,  28},
  [0][235] = { 75,  76,  76},
  [1][235] = { 38,  38,  38},
  [0][236] = { 84,  85,  85},
  [1][236] = { 48,  48,  48},
  [0][237] = { 92,  93,  94},
  [1][237] = { 58,  58,  58},
  [0][238] = {101, 102, 102},
  [1][238] = { 68,  68,  68},
  [0][239] = {109, 111, 111},
  [1][239] = { 78,  78,  78},
  [0][240] = {118, 119, 119},
  [1][240] = { 88,  88,  88},
  [0][241] = {126, 127, 128},
  [1][241] = { 98,  98,  98},
  [0][242] = {134, 136, 136},
  [1][242] = {108, 108, 108},
  [0][243] = {143, 144, 145},
  [1][243] = {118, 118, 118},
  [0][244] = {151, 152, 153},
  [1][244] = {128, 128, 128},
  [0][245] = {159, 161, 161},
  [1][245] = {138, 138, 138},
  [0][246] = {167, 169, 169},
  [1][246] = {148, 148, 148},
  [0][247] = {176, 177, 177},
  [1][247] = {158, 158, 158},
  [0][248] = {184, 185, 186},
  [1][248] = {168, 168, 168},
  [0][249] = {192, 193, 194},
  [1][249] = {178, 178, 178},
  [0][250] = {200, 201, 202},
  [1][250] = {188, 188, 188},
  [0][251] = {208, 209, 210},
  [1][251] = {198, 198, 198},
  [0][252] = {216, 218, 218},
  [1][252] = {208, 208, 208},
  [0][253] = {224, 226, 226},
  [1][253] = {218, 218, 218},
  [0][254] = {232, 234, 234},
  [1][254] = {228, 228, 228},
  [0][255] = {240, 242, 242},
  [1][255] = {238, 238, 238},
};

// For Emacs:
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:2
// c-basic-offset:2
// c-file-style: nil
// End:
// For VIM:
// vim:set expandtab softtabstop=2 shiftwidth=2 tabstop=2:
