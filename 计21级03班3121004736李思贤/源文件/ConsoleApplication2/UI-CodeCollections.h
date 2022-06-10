#pragma once

#define _CC_(mode) cobox->Searchinfo.searchmode |= ##mode;

#define CCint(obj,type,mode) \
_CC_(mode)\
Element_update(&cobox->##obj##.input);\
changed = true;\
cobox->Searchinfo.##obj## = cobox->##obj##.data.##type;\

#define CCstr(obj,type,mode) \
_CC_(mode);\
Element_update(&cobox->##obj##.input);\
changed = true;\
strcpy_s(cobox->Searchinfo.##obj## ,15, cobox->##obj##.data.##type);