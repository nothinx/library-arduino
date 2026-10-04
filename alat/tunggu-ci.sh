# tunggu-ci.sh <Nama>... : tunggu run CI terakhir selesai, cetak hasil (+ job gagal).
for n in "$@"; do
  sleep 10
  until r=$(gh run list -R "nothinx/$n" --limit 1 --json status,conclusion,databaseId -q '.[0]|.status+" "+.conclusion+" "+(.databaseId|tostring)' 2>/dev/null); [ "${r%% *}" = completed ]; do sleep 20; done
  echo "$n: $r"
  set -- $r; [ "$2" = success ] || gh run view "$3" -R "nothinx/$n" --log-failed 2>&1 | grep -i "error\|warning" | head -8
done
