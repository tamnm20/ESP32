idf_component_register(
    SRCS "src/tgpio.c" "src/exti.c" "src/tuart.c" "src/ttimer.c" "src/tbutton.c"
    INCLUDE_DIRS "inc"
    REQUIRES driver esp_rom esp_timer
)