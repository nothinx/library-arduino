import json,gzip,urllib.request,sys
want=sys.argv[1:]
d=json.load(gzip.open(urllib.request.urlopen('https://downloads.arduino.cc/libraries/library_index.json.gz')))
have={l['name']:l['version'] for l in d['libraries'] if l['name'] in want}
print(' '.join(f"{n}={have.get(n,'-')}" for n in want)); sys.exit(0 if len(have)==len(want) else 1)
