idf_component_register(
    SRCS "src/tgpio.c" "src/exti.c"
    INCLUDE_DIRS "inc"
    REQUIRES driver esp_rom
)