# cek.sh <folder-library>: compile semua contoh di 3 board (--warnings all) + -Wdouble-promotion di esp32/F4
L=$1; n=$(basename $L)
for ex in $L/examples/*/; do
  for b in arduino:avr:uno esp32:esp32:esp32 STMicroelectronics:stm32:GenF1:pnum=BLUEPILL_F103C8; do
    o=$(arduino-cli compile -b $b --warnings all --library $L $ex </dev/null 2>&1); rc=$?
    w=$(echo "$o" | grep -ci "warning"); echo "$(basename $ex) $b rc=$rc warnings=$w"
    [ $w -gt 0 ] && echo "$o" | grep -i -A3 warning | head -20
  done
  for b in esp32:esp32:esp32 STMicroelectronics:stm32:GenF4:pnum=BLACKPILL_F411CE; do
    o=$(arduino-cli compile -b $b --warnings all --build-property "compiler.cpp.extra_flags=-Wdouble-promotion" --library $L $ex </dev/null 2>&1); rc=$?
    echo "  double-promotion $b rc=$rc src=$(echo "$o" | grep -c "$n/src.*double-promotion") semua=$(echo "$o" | grep -c "Wdouble-promotion")"
  done
done
