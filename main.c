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
  perror("fwrite");
  rewind(binary);
}

theme yellow_theme() {
  color background_color = { 0.996078, 0.956862, 0.611764 };
  color border_color = { 0.737255, 0.662745, 0.007843 };
  color window_title_color = { 0.996078, 0.917647, 0.239216 };
  color icon_color = { 0.858824, 0.772549, 0.011765 };

  theme t = { background_color, border_color, window_title_color, icon_color };
  return t;
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

  apply_theme(0, yellow_theme(), binary);

  fclose(binary);

  return 0;
}

