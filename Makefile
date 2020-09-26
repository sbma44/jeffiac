wav:
	mkdir wav
	python3 make_wavs.py sayings.txt
	#find wav -type f | xargs -I {} basename {} .aiff | xargs -I {} sox wav/{}.aiff --norm -c 1 -t .wav -b 16 -r 16000 wav/{}.wav
	find wav -type f | xargs -I {} basename {} .aiff | xargs -I {} sox wav/{}.aiff --norm -c 1 -e ms-adpcm -t .wav -b 4 wav/{}.wav
	find wav -type f | grep '\.wav$' | xargs basename | xargs -I {} mv wav/{}.wav wav/{}.ad4
	rm wav/*.aiff

clean:
	rm -rf wav

all: clean wav