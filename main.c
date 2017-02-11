#include <stdio.h>
#include <string.h>

#include <errno.h>

typedef struct {
  double red;
  double green;
  double blue;
} color;

typedef struct {
  color background_color;
  color border_color;
  color window_title_color;
  color icon_color;
} theme;

static const int ColorBase = 0x0000c790;

void dump_color(const char *name, const color c) {
  printf("%s:\n", name);
  printf("  r: %f\n  g: %f\n  b: %f\n\n", c.red, c.green, c.blue);
}

void dump_theme(const int color_index, FILE *binary) {
  const int offset = (color_index + (color_index * 2)) << 5;
  const int base = ColorBase + offset;
  fseek(binary, base, SEEK_SET);
  theme t;
  fread(&t, sizeof(theme), 1, binary);
  rewind(binary);

  printf("%d\n", color_index);
  dump_color("background_color", t.background_color);
  dump_color("border_color", t.border_color);
  dump_color("window_title_color", t.window_title_color);
  dump_color("icon_color", t.icon_color);
}

void apply_theme(const int color_index, const theme t, FILE *binary) {
  const int offset = (color_index + (color_index * 2)) << 5;
  const int base = ColorBase + offset;
  fseek(binary, base, SEEK_SET);
  fwrite(&t, sizeof(theme), 1, binary);
  rewind(binary);
}

void change_color(color c, FILE *binary) {
  fseek(binary, ColorBase, SEEK_SET);
  fwrite(&c, sizeof(color), 1, binary);
  rewind(binary);
}

/// Default Themes

theme default_yellow_theme() {
  color background_color = { 0.996078, 0.956862, 0.611764 };
  color border_color = { 0.737255, 0.662745, 0.007843 };
  color window_title_color = { 0.996078, 0.917647, 0.239216 };
  color icon_color = { 0.858824, 0.772549, 0.011765 };

  theme t = { background_color, border_color, window_title_color, icon_color };
  return t;
}

theme default_blue_theme() {
  color background_color = { 0.678431, 0.956863, 1 };
  color border_color = { 0.007843, 0.737255, 0.843137 };
  color window_title_color = { 0.537255, 0.941176, 1 };
  color icon_color = { 0.141176, 0.815686, 0.913725 };

  theme t = { background_color, border_color, window_title_color, icon_color };
  return t;
}

theme default_green_theme() {
  color background_color = { 0.698039, 1, 0.631373 };
  color border_color = { 0.282353, 0.635294, 0.282353 };
  color window_title_color = { 0.513725, 0.996078, 0.513725 };
  color icon_color = { 0.317647, 0.733333, 0.317647 };

  theme t = { background_color, border_color, window_title_color, icon_color };
  return t;
}

theme default_pink_theme() {
  color background_color = { 1, 0.780392, 0.780392 };
  color border_color = { 0.886275, 0.458824, 0.458824 };
  color window_title_color = { 1, 0.698039, 0.698039 };
  color icon_color = { 0.972549, 0.498039, 0.498039 };

  theme t = { background_color, border_color, window_title_color, icon_color };
  return t;
}

theme default_purple_theme() {
  color background_color = { 0.713725, 0.792157, 1 };
  color border_color = { 0.458824, 0.568627, 0.862745 };
  color window_title_color = { 0.607843, 0.713725, 0.996078 };
  color icon_color = { 0.490196, 0.607843, 0.921569 };

  theme t = { background_color, border_color, window_title_color, icon_color };
  return t;
}

theme default_gray_theme() {
  color background_color = { 0.933333, 0.933333, 0.933333 };
  color border_color = { 0.619608, 0.619608, 0.619608 };
  color window_title_color = { 0.854902, 0.854902, 0.854902 };
  color icon_color = { 0.658824, 0.658824, 0.658824 };

  theme t = { background_color, border_color, window_title_color, icon_color };
  return t;
}

void apply_default_themes(FILE *binary) {
  apply_theme(0, default_yellow_theme(), binary);
  apply_theme(1, default_blue_theme(), binary);
  apply_theme(2, default_green_theme(), binary);
  apply_theme(3, default_pink_theme(), binary);
  apply_theme(4, default_purple_theme(), binary);
  apply_theme(5, default_gray_theme(), binary);
}

color red_color() {
  color c = { 1, 0, 0 };
  return c;
}

int main(int argc, char **argv) {
  const char *binary_path = argv[1];
  FILE *binary = fopen(binary_path, "r+b");
  if (!binary) {
    printf("Can't open %s\n", binary_path);
    return 1;
  }

  apply_default_themes(binary);

  fclose(binary);

  return 0;
}

