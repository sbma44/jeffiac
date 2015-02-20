wav:
	mkdir wav
	python3 make_wavs.py sayings.txt
	find wav -type f | xargs -I {} basename {} .aiff | xargs -I {} sox wav/{}.aiff wav/{}.wav	
	rm wav/*.aiff

clean:
	rm -rf wav

all: clean wav