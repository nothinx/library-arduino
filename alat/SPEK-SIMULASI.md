# Spesifikasi: hasil simulasi & aset di README

Tujuan: setiap README menampilkan **bukti visual** perilaku library: grafik dari simulasi di PC yang
menjalankan **kode library asli** (src/), bukan tiruan. Pembaca langsung melihat library bekerja.

Library sudah dirilis dan ada di registry Arduino. Folder: `D:\deo\projects\library-arduino\<Nama>\`.
Baca dulu README.md, README.en.md, src/, extras/test/ library yang kamu kerjakan.

## Struktur (sama untuk semua library)
```
extras/simulasi/simulasi.cpp   # memakai ../test/Arduino.h tiruan + ../../src, mencetak CSV/data ke stdout
extras/simulasi/gambar.py      # compile + jalankan simulasi.cpp, lalu render semua grafik
extras/gambar/*.svg            # hasil render, di-commit
```
- `gambar.py` dijalankan dari folder `extras/simulasi`: `python gambar.py`. Ia memanggil
  `g++ -std=c++11 -O2 -I../test -I../../src simulasi.cpp ../../src/*.cpp -o <tempdir>/sim` (abaikan *.cpp jika
  header-only), menjalankannya, membaca output, menulis SVG ke `../gambar/`. Executable di direktori temp
  (`tempfile`), bukan di repo. Boleh satu program simulasi dengan beberapa skenario (argumen/penanda bagian).
- Jika `Arduino.h` tiruan di extras/test kurang (mis. butuh Wire/Stream/Print), perluas di extras/test
  (jangan duplikasi), dan pastikan `extras/test/uji.cpp` tetap lolos.
- 2–4 grafik per library. Pilih yang paling meyakinkan dan mudah dipahami pemula. Lebih sedikit tapi jelas
  lebih baik daripada banyak.
- Jika library secara alami lebih cocok ditunjukkan dengan **contoh keluaran teks** (mis. Terbilang,
  PerintahSerial), buat keluaran itu dari program simulasi (dijamin asli) dan tampilkan sebagai blok kode/tabel
  di README; grafik hanya bila benar-benar menambah pemahaman.

## Gaya grafik (wajib seragam di semua library)
```python
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
plt.rcParams.update({
    "figure.figsize": (8, 3.6), "figure.dpi": 100, "savefig.bbox": "tight", "savefig.pad_inches": 0.15,
    "figure.facecolor": "white", "axes.facecolor": "white", "savefig.facecolor": "white",
    "font.size": 10, "axes.titlesize": 11, "axes.titleweight": "bold", "axes.titlelocation": "left",
    "axes.spines.top": False, "axes.spines.right": False, "axes.edgecolor": "#9ca3af",
    "axes.grid": True, "grid.color": "#e5e7eb", "grid.linewidth": 0.8,
    "legend.frameon": False, "svg.fonttype": "path", "svg.hashsalt": "nothinx",
    "lines.linewidth": 1.8,
})
WARNA = {"utama": "#2563eb", "pembanding": "#dc2626", "ketiga": "#16a34a", "keempat": "#9333ea",
         "kelima": "#ea580c", "mentah": "#9ca3af", "target": "#111827"}
# simpan: fig.savefig(path, format="svg", metadata={"Date": None})
```
- Warna: hasil library kita selalu `utama` (biru). Pembanding/kondisi buruk `pembanding` (merah).
  Data mentah/noise `mentah` (abu-abu, garis tipis 1.0 atau titik kecil). Target/setpoint `target` putus-putus.
- Teks di grafik **bahasa Indonesia** (judul, sumbu dengan satuan, legenda). Judul menyatakan temuan, bukan
  sekadar nama ("Anti-windup memangkas overshoot dari 22,5° menjadi 3,0°" lebih baik dari "Respon PID").
  Desimal pakai koma di teks yang ditulis tangan; angka sumbu boleh default.
- Tanpa judul ganda, tanpa kotak legenda, tanpa efek 3D/bayangan. Anotasi langsung pada garis bila memungkinkan.
- Output deterministik: seed tetap, `metadata={"Date": None}`, `svg.hashsalt` tetap → menjalankan ulang
  menghasilkan file identik (cek dengan menjalankan dua kali dan `git status`).
- Ukuran SVG wajar (< ~300 KB per file). Kurangi jumlah titik bila perlu.

## Kejujuran
- Label sebagai **simulasi** (di caption README). Jangan menyiratkan pengukuran hardware.
- Perbandingan dengan pesaing hanya jika algoritma pesaing sudah direproduksi di uji yang ada atau kamu
  reproduksi setia dari source-nya (sebut sumber + versi di komentar kode simulasi). Jika ragu, jangan.
- Angka di judul/caption harus sama dengan angka yang dihasilkan simulasi (ambil dari output, jangan ketik manual
  bila bisa dihitung di gambar.py).

## README
- `README.md`: tambah bagian `## Hasil simulasi` setelah bagian contoh cepat (atau setelah "Fitur" jika lebih pas).
  Tiap grafik: `![alt deskriptif](extras/gambar/<nama>.svg)` + 1–2 kalimat penjelasan di bawahnya.
  Akhiri bagian dengan cara membuat ulang:
  ````
  Grafik dibuat dari simulasi di PC yang menjalankan kode library ini (`extras/simulasi`):
  ```sh
  cd extras/simulasi
  python gambar.py   # butuh g++ dan matplotlib
  ```
  ````
- `README.en.md`: bagian `## Simulation results` dengan gambar yang sama dan caption singkat bahasa Inggris.
- Jangan ubah bagian lain README kecuali angka yang ternyata salah (laporkan).

## Pemeriksaan sebelum commit
1. `python gambar.py` sukses, dua kali berturut-turut tanpa perubahan file (deterministik).
2. `extras/test/uji.cpp` tetap lolos (perintah di README bagian Pengujian).
3. Lint: dari `D:/deo/projects/library-arduino` jalankan
   `D:/deo/projects/library-arduino/alat/arduino-lint.exe --library-manager update --compliance strict <Nama>` → tanpa error/warning.
4. Satu contoh compile di Uno masih lolos (sanity): `arduino-cli compile -b arduino:avr:uno --library <folder> <folder>/examples/<satu contoh> < /dev/null`.
5. Lihat setiap SVG (Read tool bisa membaca gambar; jika tidak bisa membaca SVG, render juga PNG sementara ke
   direktori temp lalu Read) dan pastikan: teks tidak bertumpuk, legenda tidak menutupi data, judul terbaca.

## Git
`git add -A && git -c user.name="Amadeo Wisesa" -c user.email="wisesaamadeo@gmail.com" commit -qm "docs: tambah hasil simulasi di README"`
Tanpa co-author/trailer. **Jangan push.** Jangan ubah library.properties/version. Jangan sentuh folder lain.

## Laporan akhir
Per library: daftar grafik (nama file + temuan utama + angka), perubahan pada Arduino.h tiruan (jika ada),
hasil pemeriksaan 1–5, hash commit, dan hal yang perlu diketahui (mis. angka README lama yang ternyata salah).
