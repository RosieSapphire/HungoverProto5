GAME=hungover
CFLAGS=-Wall -Wextra -Iinclude -Ofast
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

src = $(wildcard src/*.c src/*/*.c)
assets_wav = $(wildcard assets/*.wav)
assets_png = $(wildcard assets/*.png)
assets_ttf = $(wildcard assets/*.ttf)

assets_conv = $(addprefix filesystem/,$(notdir $(assets_wav:%.wav=%.wav64))) \
              $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite))) \
              $(addprefix filesystem/,$(notdir $(assets_ttf:%.ttf=%.font64)))

AUDIOCONV_FLAGS=--ym-compress true
MKSPRITE_FLAGS=-c 1

SCENEIMP=./tools/sceneimp/sceneimp

all: $(GAME).z64

filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o filesystem $<

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"

filesystem/debug_font.font64: assets/debug_font.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) --size 8 -o filesystem "$<"

filesystem/door_num_font.font64: assets/door_num_font.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) --size 36 -o filesystem "$<"

$(BUILD_DIR)/$(GAME).dfs: $(assets_conv)
$(BUILD_DIR)/$(GAME).elf: $(src:%.c=$(BUILD_DIR)/%.o)

$(GAME).z64: N64_ROM_TITLE="Hungover"
$(GAME).z64: $(BUILD_DIR)/$(GAME).dfs 
$(GAME).z64: N64_ED64ROMCONFIGFLAGS=-w eeprom4k

clean:
	rm -rf $(BUILD_DIR) $(GAME).z64

-include $(wildcard $(BUILD_DIR)/*.d)

betty-style:
	betty-style --allow-global-variables include/*/*.h src/*/*.c src/*.c

betty-doc:
	betty-doc include/*/*.h src/*/*.c src/*.c

betty:
	clear; make betty-doc; make betty-style;

.PHONY: all clean
