# terbit.sh <Nama> <topic tambahan,...> : buat repo publik, push, pasang topic.
set -e
cd "D:/deo/projects/library-arduino/$1"
desc=$(grep '^sentence=' library.properties | cut -d= -f2-)
gh repo create "nothinx/$1" --public --description "$desc" --source . --push >/dev/null 2>&1
gh repo edit "nothinx/$1" --add-topic "arduino-library,arduino,bahasa-indonesia,$2" >/dev/null
echo "$1 terbit: https://github.com/nothinx/$1"
