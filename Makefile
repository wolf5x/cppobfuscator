SRC_DIR = src

.PHONY: default all clean rebuild $(SRC_DIR)

default: all

all clean rebuild:
	$(MAKE) $(SRC_DIR) TARGET=$@

$(SRC_DIR): print
	$(MAKE) -C $@ $(TARGET)

print:
	@echo "---- TARGET = $(TARGET)"

