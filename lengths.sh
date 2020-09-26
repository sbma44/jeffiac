for a in $(ls wav/*.wav); do
    sox $a -n stat 2>&1 | grep Length | perl -ple 's/Length \(seconds\):\s+//gi'
done