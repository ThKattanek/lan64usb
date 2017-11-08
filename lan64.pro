TEMPLATE = subdirs
SUBDIRS =lan64tool \
    konsolen_tools/lan64_send

OTHER_FILES += \
    firmware/Makefile \
    firmware/main.c \
    firmware/usbconfig.h \
    doc/schaltplan.pdf \
    c64/lan64.asm
