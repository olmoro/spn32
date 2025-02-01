set(PROJECT_NAME "spn32")

idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS ".")

target_add_binary_data(${COMPONENT_TARGET} E:/PlatformIO/zoo/libs_spn/certs/isrg_root_x1.pem TEXT)
target_add_binary_data(${COMPONENT_TARGET} E:/PlatformIO/zoo/libs_spn/certs/digi_cert_g2.pem TEXT)
target_add_binary_data(${COMPONENT_TARGET} E:/PlatformIO/zoo/libs_spn/certs/api_telegram_org.pem TEXT)
