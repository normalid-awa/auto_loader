idf_component_register(
    SRCS  "main_rewrite.c" "lvgl_init.c"
    INCLUDE_DIRS "."
    REQUIRES generated_ui
)