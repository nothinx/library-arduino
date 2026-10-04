# Spesifikasi tambahan: library sensor

Berlaku untuk setiap library sensor/modul, **di atas** `SPEK.md` (standar umum) dan `SPEK-BENCH.md`
(benchmark). Diputuskan 3 Okt 2026. Tujuan: library kita terbukti lebih unggul dari pesaing, dan setiap
klaim "unggul" punya angka atau uji yang bisa diulang.

## Target

| Dimensi | Target | Bukti |
|---|---|---|
| Kemudahan | **Wajib menang** | Baris contoh minimal sampai angka pertama, API seragam, pesan error Indonesia |
| Waktu terblokir | **Wajib menang** | Blokir maksimum per panggilan (µs), diukur di simavr |
| Ketahanan | **Wajib menang** | Uji PC: kabel dicabut, data korup, sensor lambat, pulih sendiri |
| RAM | **Wajib menang** | RAM per objek (sizeof) dan RAM global, vs pesaing |
| Akurasi | Sama atau lebih baik | Uji PC dengan sensor tiruan; uji hardware bila tersedia |
| Flash, kecepatan mentah | Boleh seri/kalah tipis | Angka + alasan ditulis jujur di README |

Jika satu target "wajib menang" tidak tercapai, jangan rilis: perbaiki, atau tulis alasannya dan minta keputusan.

## Datasheet (wajib, sebelum menulis kode)

1. Unduh datasheet resmi pabrikan chip (bukan blog/toko) ke `alat/riset/datasheet/<chip>_<pabrikan>.pdf`
   (tidak dipublikasikan) dan ekstrak teksnya ke `.txt` di sebelahnya.
2. Di `extras/riset.md` repo library: tautan datasheet resmi, lalu tabel **parameter yang dipakai kode**
   (timing, rentang, format data, waktu settling, mode daya) dengan nomor halaman/tabel datasheet.
3. Setiap konstanta di `src/` yang berasal dari datasheet diberi komentar sumbernya
   (mis. `// datasheet HX711 hlm. 5: PD_SCK tinggi > 60 µs = power down`).
4. Sensor tiruan di `extras/test` meniru perilaku datasheet (timing, saturasi, settling), dan ada uji
   yang gagal jika batas datasheet dilanggar (mis. pulsa clock terlalu panjang).
5. README: bagian **Spesifikasi sensor** ringkas (rentang, resolusi, kecepatan sampel, tegangan) dengan tautan
   datasheet, dan catatan bila modul di pasaran berbeda dari datasheet (mis. pin RATE terhubung ke GND = 10 SPS).

## Riset berbasis issue (sebelum menulis kode)

1. Ambil 2–3 pesaing teratas dari index (nama/sentence/paragraph memuat nama chip). Unduh source rilisnya ke
   `alat/bench/pesaing/` (zip dari `downloads.arduino.cc`), catat versi.
2. Kumpulkan issue terbanyak/teratas pesaing di GitHub (`gh issue list -R <repo> --state all --limit 100
   --search "sort:reactions-+1-desc"`, plus `gh search issues "<chip>" --limit 50`). Catat di
   `extras/riset.md` di repo library: judul issue, tautan, akar masalah dari source.
3. Setiap masalah nyata menjadi: (a) keputusan desain, (b) kasus uji di `extras/test`, (c) baris di tabel
   README **Gejala → Penyebab → Solusi**. Jangan mengklaim masalah yang tidak ditemukan di source/issue.

## API seragam

Semua library sensor memakai nama yang sama untuk peran yang sama (sesuaikan bila sensor tidak butuh):

| Fungsi | Arti |
|---|---|
| `bool mulai(...)` | Siapkan pin/bus, cek sensor menjawab. `false` + `status()` jika gagal |
| `bool perbarui()` | Panggil di setiap `loop()`. Non-blocking. `true` jika ada bacaan baru |
| `float nilai()` / nama spesifik (`suhu()`, `jarakCm()`, `beratGram()`) | Bacaan terakhir yang valid, satuan SI/umum |
| `bool siap()` | Sudah ada bacaan valid |
| `Status status()` | Enum: `OK`, `TIDAK_TERHUBUNG`, `DATA_RUSAK`, `DI_LUAR_RENTANG`, `BELUM_SIAP`, ... |
| `const char *alasan()` | Penjelasan status dalam bahasa Indonesia + saran (PROGMEM/`F()` di AVR) |
| `aturX(...)` / `x()` | Pengaturan & kalibrasi; nilai kalibrasi bisa dibaca dan diisi ulang |

- **Jalur data mentah** seperti `TombolPintar.perbarui(bool)`: logika (filter, kalibrasi, status) terpisah dari
  pembacaan kabel, sehingga bisa dipakai lewat expander/multiplexer dan diuji di PC.
- **Kalibrasi sebagai nilai**, bukan disimpan sendiri ke EEPROM: pengguna memilih tempat simpan. Contoh
  `SimpanKalibrasi` menunjukkan cara menyimpan ke EEPROM (AVR) / Preferences (ESP32).
- Satuan di nama fungsi bila ambigu (`jarakCm()`, `beratGram()`).
- Nilai tidak valid: `NAN` untuk float, plus `status()`; jangan pernah angka palsu seperti 0 atau -127.

## Kemudahan

- Contoh minimal (`Dasar...`) sependek mungkin; hitung barisnya (tanpa komentar/kosong) vs contoh minimal pesaing.
- Pengaturan bawaan langsung bekerja untuk modul yang paling umum dijual.
- Deteksi otomatis bila sensor memungkinkan (tipe chip, alamat I2C), dengan cara menimpa manual.
- Contoh wajib: `Dasar<Sensor>`, `CekSambungan` (diagnosa kabel: mencetak apa yang salah dan cara
  memperbaikinya), plus contoh kalibrasi bila sensor perlu kalibrasi.

## Waktu terblokir

- Ukur blokir maksimum setiap fungsi publik di simavr (Siklus.h), termasuk saat sensor tidak terhubung
  (timeout). Bandingkan dengan pesaing pada skenario yang sama.
- Protokol yang butuh timing ketat (DHT, 1-Wire) boleh mematikan interrupt hanya selama satu bit/byte,
  bukan satu frame penuh, dan durasinya dicantumkan.
- Tanpa `delay()`, `pulseIn()` panjang, atau loop menunggu tanpa batas waktu.

## Ketahanan (uji PC wajib)

- Sensor tiruan di level protokol di `extras/test/` (pola `ArahMPU6050/extras/test/Wire.h`): timing bit,
  register, atau pulsa sesuai datasheet, dengan pengatur waktu palsu.
- Kasus wajib: sensor tidak terhubung sejak awal; dicabut lalu dipasang lagi (harus pulih tanpa reset);
  data korup/checksum salah; bacaan nyasar (lonjakan); luapan `millis()`/`micros()`; banyak objek sekaligus.
- `perbarui()` tidak boleh hang di kasus mana pun.

## Fleksibilitas (checklist di README)

Pin bebas · bus/Stream pilihan (`TwoWire&`, `Stream&`) · banyak sensor sekaligus · 7 board CI · jalur data
mentah · kalibrasi bisa diisi ulang. Fitur tambahan hanya menambah flash jika dipakai (buktikan dengan angka).

## Uji hardware

- Bila modul tersedia: jalankan contoh `CekSambungan` dan `Dasar...` di minimal satu board, bandingkan dengan
  alat ukur acuan (multimeter, timbangan, termometer), catat di README bagian **Status**: board, modul,
  galat, tanggal. Pemilik menjalankan uji; agent menyiapkan sketch uji dan format catatannya.
- Bila belum: tulis jujur "belum diuji di hardware" seperti sekarang.

## README (tambahan untuk struktur SPEK.md)

- Tabel **Rapor** di dekat atas: dimensi | kita | pesaing A | pesaing B (angka, bukan centang saja).
- **Gejala → Penyebab → Solusi** dari riset issue.
- `## Kecepatan & memori` sesuai SPEK-BENCH, ditambah kolom waktu terblokir maksimum.
- `sentence`/`paragraph` di `library.properties` memuat semua nama/varian chip (mis. "HX711, YZC-131, load cell")
  agar mudah dicari.
