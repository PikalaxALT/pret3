struct incbin {
  unsigned offset;
  unsigned length;
  char file[];
};

struct command {
  const char * name;
  const char * alias;
  const char * help_text;
};

struct script_value {
  unsigned char type; // 0: data buffer, {1, 2, 3}: {8, 16, 32}-bit integer, 4: text, {5, 6, 7}: arrays of integers
  int value;
  void * data;
};

struct script_variables {
  unsigned count;
  char ** names;
  struct script_value * values;
};

struct script_transforms {
  char * (* transform) (struct script_value, int, struct script_value *);
  int parameter;
};

struct transform {
  const char * name;
  char * (* transform) (struct script_value, int, struct script_value *);
};