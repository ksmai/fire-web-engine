XML = $(shell find . -name '*.xml' -type f)
WAV = $(shell find . -name '*.wav' -type f)
PNG = $(shell find . -name '*.png' -type f)
OGG = $(shell find . -name '*.ogg' -type f)

ZIP = resources.zip

.PHONY: default

default: $(ZIP)

$(ZIP): $(XML) $(WAV) $(PNG) $(OGG)
	rm -f $(ZIP)
	zip $(ZIP) $(XML) $(WAV) $(PNG) $(OGG)
