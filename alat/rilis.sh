# rilis.sh <Nama> "<catatan>" : release 1.0.0 (tag = versi library.properties).
v=$(grep '^version=' "D:/deo/projects/library-arduino/$1/library.properties" | cut -d= -f2)
gh release create "$v" -R "nothinx/$1" --title "$1 $v" --notes "$2" 2>&1 | tail -1
